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

#ifndef ZLMEDIAKIT_RTPRECEIVER_H
#define ZLMEDIAKIT_RTPRECEIVER_H


#include <map>
#include <string>
#include <memory>
#include "Rtsp.h"
#include "RtspMuxer/RtpCodec.h"
#include "RtspMediaSource.h"

using namespace std;
using namespace toolkit;

namespace mediakit {

class RtpReceiver {
public:
    RtpReceiver();
    virtual ~RtpReceiver();
protected:

    /**
     * 输入数据指针生成并排序rtp包
     * @param track_index track下标索引
     * @param track  sdp track相关信息
     * @param rtp_raw_ptr rtp数据指针
     * @param rtp_raw_len rtp数据指针长度
     * @return 解析成功返回true
     */
    bool handleOneRtp(int track_index,SdpTrack::Ptr &track, unsigned char *rtp_raw_ptr, unsigned int rtp_raw_len);

    /**
     * rtp数据包排序后输出
     * @param rtp rtp数据包
     * @param track_index track索引
     */
    virtual void onRtpSorted(const RtpPacket::Ptr &rtp, int track_index){}
    void clear();
    void setPoolSize(int size);
    int getJitterSize(int track_index);
    int getCycleCount(int track_index);
private:
    void sortRtp(const RtpPacket::Ptr &rtp , int track_index);
private:
    //ssrc不匹配计数
    uint32_t _ssrc_err_count[2] = { 0, 0 };
    //上次seq
    uint16_t _last_seq[2] = { 0 , 0 };
    //seq连续次数计数
    uint32_t _seq_ok_count[2] = { 0 , 0};
    //seq回环次数计数
    uint32_t _seq_cycle_count[2] = { 0 , 0};
    //是否开始seq排序
    bool _sort_started[2] = { 0 , 0};
    //rtp排序缓存，根据seq排序
    map<uint16_t , RtpPacket::Ptr> _rtp_sort_cache_map[2];
    //rtp循环池
    RtspMediaSource::PoolType _rtp_pool;
};

}//namespace mediakit


#endif //ZLMEDIAKIT_RTPRECEIVER_H
