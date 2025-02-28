﻿/*
 * MIT License
 *
 * Copyright (c) 2016-2019 xiongziliang <771730766@qq.com>
 *
 * This file is part of ZLMediaKit(https://github.com/xiongziliang/ZLMediaKit).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Common/config.h"
#include "RtpReceiver.h"

#define POP_HEAD(trackidx) \
		auto it = _rtp_sort_cache_map[trackidx].begin(); \
		onRtpSorted(it->second, trackidx); \
		_rtp_sort_cache_map[trackidx].erase(it);

#define AV_RB16(x)                           \
    ((((const uint8_t*)(x))[0] << 8) |          \
      ((const uint8_t*)(x))[1])

#define RTP_MAX_SIZE (10 * 1024)

namespace mediakit {

RtpReceiver::RtpReceiver() {}
RtpReceiver::~RtpReceiver() {}

bool RtpReceiver::handleOneRtp(int track_index,SdpTrack::Ptr &track, unsigned char *rtp_raw_ptr, unsigned int rtp_raw_len) {
    auto rtp_ptr = _rtp_pool.obtain();
    auto &rtp = *rtp_ptr;
    auto length = rtp_raw_len + 4;

    rtp.interleaved = track->_interleaved;
    rtp.mark = rtp_raw_ptr[1] >> 7;
    rtp.PT = rtp_raw_ptr[1] & 0x7F;
    //序列号
    memcpy(&rtp.sequence,rtp_raw_ptr+2,2);//内存对齐
    rtp.sequence = ntohs(rtp.sequence);
    //时间戳
    memcpy(&rtp.timeStamp, rtp_raw_ptr+4, 4);//内存对齐

    if(!track->_samplerate){
        //无法把时间戳转换成毫秒
        return false;
    }
    //时间戳转换成毫秒
    rtp.timeStamp = ntohl(rtp.timeStamp) * 1000LL / track->_samplerate;
    //ssrc
    memcpy(&rtp.ssrc,rtp_raw_ptr+8,4);//内存对齐
    rtp.ssrc = ntohl(rtp.ssrc);
    rtp.type = track->_type;

    if (track->_ssrc != rtp.ssrc) {
        if (track->_ssrc == 0) {
            //保存SSRC至track对象
            track->_ssrc = rtp.ssrc;
        }else{
            //ssrc错误
            WarnL << "ssrc错误:" << rtp.ssrc << " != " << track->_ssrc;
            if (_ssrc_err_count[track_index]++ > 10) {
                //ssrc切换后清除老数据
                WarnL << "ssrc更换:" << track->_ssrc << " -> " << rtp.ssrc;
                _rtp_sort_cache_map[track_index].clear();
                track->_ssrc = rtp.ssrc;
            }
            return false;
        }
    }

    //ssrc匹配正确，不匹配计数清零
    _ssrc_err_count[track_index] = 0;

    //获取rtp中媒体数据偏移量
    rtp.offset 	= 12 + 4;
    int csrc     	= rtp_raw_ptr[0] & 0x0f;
    int ext      	= rtp_raw_ptr[0] & 0x10;
    rtp.offset 	+= 4 * csrc;
    if (ext && rtp_raw_len >= rtp.offset) {
        /* calculate the header extension length (stored as number of 32-bit words) */
        ext = (AV_RB16(rtp_raw_ptr + rtp.offset - 2) + 1) << 2;
        rtp.offset += ext;
    }

    if(length <= rtp.offset){
        WarnL << "无有效负载的rtp包:" << length << "<=" << (int)rtp.offset;
        return false;
    }

    if(length > RTP_MAX_SIZE){
        WarnL << "超大的rtp包:" << length << ">" << RTP_MAX_SIZE;
        return false;
    }

    //设置rtp负载长度
    rtp.setCapacity(length);
    rtp.setSize(length);
    uint8_t *payload_ptr = (uint8_t *)rtp.data();
    payload_ptr[0] = '$';
    payload_ptr[1] = rtp.interleaved;
    payload_ptr[2] = rtp_raw_len >> 8;
    payload_ptr[3] = (rtp_raw_len & 0x00FF);
    //拷贝rtp负载
    memcpy(payload_ptr + 4, rtp_raw_ptr, rtp_raw_len);
    //排序rtp
    sortRtp(rtp_ptr,track_index);
    return true;
}

void RtpReceiver::sortRtp(const RtpPacket::Ptr &rtp,int track_index){
    if(rtp->sequence != _last_seq[track_index] + 1 && _last_seq[track_index] != 0){
        //包乱序或丢包
        _seq_ok_count[track_index] = 0;
        _sort_started[track_index] = true;
        if(_last_seq[track_index] > rtp->sequence && _last_seq[track_index] - rtp->sequence > 0xFF){
            //sequence回环，清空所有排序缓存
            while (_rtp_sort_cache_map[track_index].size()) {
                POP_HEAD(track_index)
            }
            ++_seq_cycle_count[track_index];
        }
    }else{
        //正确序列的包
        _seq_ok_count[track_index]++;
    }

    _last_seq[track_index] = rtp->sequence;

    //开始排序缓存
    if (_sort_started[track_index]) {
        _rtp_sort_cache_map[track_index].emplace(rtp->sequence, rtp);
        GET_CONFIG(uint32_t,clearCount,Rtp::kClearCount);
        GET_CONFIG(uint32_t,maxRtpCount,Rtp::kMaxRtpCount);
        if (_seq_ok_count[track_index] >= clearCount) {
            //网络环境改善，需要清空排序缓存
            _seq_ok_count[track_index] = 0;
            _sort_started[track_index] = false;
            while (_rtp_sort_cache_map[track_index].size()) {
                POP_HEAD(track_index)
            }
        } else if (_rtp_sort_cache_map[track_index].size() >= maxRtpCount) {
            //排序缓存溢出
            POP_HEAD(track_index)
        }
    }else{
        //正确序列
        onRtpSorted(rtp, track_index);
    }
}

void RtpReceiver::clear() {
    CLEAR_ARR(_last_seq)
    CLEAR_ARR(_ssrc_err_count)
    CLEAR_ARR(_seq_ok_count)
    CLEAR_ARR(_sort_started)
    CLEAR_ARR(_seq_cycle_count)

    _rtp_sort_cache_map[0].clear();
    _rtp_sort_cache_map[1].clear();
}

void RtpReceiver::setPoolSize(int size) {
    _rtp_pool.setSize(size);
}

int RtpReceiver::getJitterSize(int track_index){
    return _rtp_sort_cache_map[track_index].size();
}

int RtpReceiver::getCycleCount(int track_index){
    return _seq_cycle_count[track_index];
}


}//namespace mediakit
