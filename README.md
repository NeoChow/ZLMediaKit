# A lightweight ,high performance and stable stream server and client framework based on C++11.

 [![Build Status](https://travis-ci.org/xiongziliang/ZLMediaKit.svg?branch=master)](https://travis-ci.org/xiongziliang/ZLMediaKit)

[中文](https://github.com/xiongziliang/ZLMediaKit/blob/master/README_CN.md)

## Why ZLMediaKit?
- Developed based on C++ 11, the code is stable and reliable, avoiding the use of raw pointers, cross-platform porting is simple and convenient, and the code is clear and concise.
- Support rich streaming media protocols(`RTSP/RTMP/HLS/HTTP-FLV`),and support Inter-protocol conversion.
- Multiplexing asynchronous network IO based on epoll and multi thread，extreme performance.
- Well performance and stable test,can be used commercially.
- Support linux, macos, ios, android, Windows Platforms.
- Very low latency(lower then one second), video opened immediately.

## Features

- RTSP
  - RTSP[S] server,support rtsp push.
  - RTSP player and pusher.
  - RTP Transport : `rtp over udp` `rtp over tcp` `rtp over http` `rtp udp multicast` .
  - Basic/Digest/Url Authentication.
  - H265/H264/AAC codec.
  - Recorded as mp4.
  - Vod of mp4.
  
- RTMP
  - RTMP server,support player and pusher.
  - RTMP player and pusher.
  - Support HTTP-FLV player.
  - H264/AAC codec.
  - Recorded as flv or mp4.
  - Vod of mp4.
  
- HLS
  - RTSP RTMP can be converted into HLS,built-in HTTP server.
  - Play authentication base on cookie.

- HTTP[S]
  - HTTP server,suppor directory meun、RESTful http api.
  - HTPP client,downloader,uploader,and http api requester.
  - Cookie supported.
  - WebSocket Server.
  - File access authentication.
  
- Others
  - Support stream proxy by ffmpeg.
  - RESTful http api and http hook event api.
  - Config file hot loading.
  - Vhost supported.
  - Auto close stream when nobody played.  
  - Play and push authentication.
  - Pull stream on Demand.



- Protocol conversion:

|          protocol/codec          | H264 | H265 | AAC  | other |
| :------------------------------: | :--: | :--: | :--: | :---: |
| RTSP[S] --> RTMP/HTTP[S]-FLV/FLV |  Y   |  N   |  Y   |   N   |
|         RTMP --> RTSP[S]         |  Y   |  N   |  Y   |   N   |
|         RTSP[S] --> HLS          |  Y   |  Y   |  Y   |   N   |
|           RTMP --> HLS           |  Y   |  N   |  Y   |   N   |
|         RTSP[S] --> MP4          |  Y   |  N   |  Y   |   N   |
|           RTMP --> MP4           |  Y   |  N   |  Y   |   N   |
|         MP4 --> RTSP[S]          |  Y   |  N   |  Y   |   N   |
|           MP4 --> RTMP           |  Y   |  N   |  Y   |   N   |

- Stream generation：

| feature/codec | H264 | H265 | AAC  | other |
| :-----------: | :--: | :--: | :--: | :---: |
| RTSP[S] push  |  Y   |  Y   |  Y   |   Y   |
|  RTSP proxy   |  Y   |  Y   |  Y   |   N   |
|   RTMP push   |  Y   |  Y   |  Y   |   Y   |
|  RTMP proxy   |  Y   |  N   |  Y   |   N   |

- RTP transport:

|  feature/transport  | tcp  | udp  | http | udp_multicast |
| :-----------------: | :--: | :--: | :--: | :-----------: |
| RTSP[S] Play Server |  Y   |  Y   |  Y   |       Y       |
| RTSP[S] Push Server |  Y   |  Y   |  N   |       N       |
|     RTSP Player     |  Y   |  Y   |  N   |       Y       |
|     RTSP Pusher     |  Y   |  Y   |  N   |       N       |


- Server supported:

|       Server        | Y/N  |
| :-----------------: | :--: |
| RTSP[S] Play Server |  Y   |
| RTSP[S] Push Server |  Y   |
|        RTMP         |  Y   |
|  HTTP[S]/WebSocket  |  Y   |

- Client supported:

|   Client    | Y/N  |
| :---------: | :--: |
| RTSP Player |  Y   |
| RTSP Pusher |  Y   |
| RTMP Player |  Y   |
| RTMP Pusher |  Y   |
|   HTTP[S]   |  Y   |



## System Requirements

- Compiler support c++11，GCC4.8/Clang3.3/VC2015 or above.
- cmake3.1  or above.
- All Linux , both 32 and 64 bits
- Apple OSX(Darwin), both 32 and 64bits.
- All hardware with x86/x86_64/arm/mips cpu.
- Windows.
- **You must use git to clone the complete code. Do not download the source code by downloading zip package. Otherwise, the sub-module code will not be downloaded by default.**



## How to build

It is recommended to compile on Ubuntu or MacOS，compiling on windows is cumbersome, and some features are not compiled by default.

### Build on linux

- My environment
  - Ubuntu16.04 64 bit and gcc5.4
  - cmake 3.5.1
- Guidance
  
  ```
	# If it is on centos6.x, you need to install the newer version of GCC and cmake first, 
	# and then compile manually according to the script "build_for_linux.sh".
	# If it is on a newer version of a system such as Ubuntu or Debain,
	# step 4 can be manipulated directly.
	
	# 1、Install GCC5.2 (this step can be skipped if the GCC version is higher than 4.7)
	sudo yum install centos-release-scl -y
	sudo yum install devtoolset-4-toolchain -y
	scl enable devtoolset-4 bash
	
	# 2、Install cmake (this step can be skipped if the cmake version is higher than 3.1)
	tar -xvf cmake-3.10.0-rc4.tar.gz #you need download cmake source file manually
	cd cmake-3.10.0-rc4
	./configure
	make -j4
	sudo make install
	
	# 3、Switch to high version GCC
	scl enable devtoolset-4 bash
	
	# 4、build
	cd ZLMediaKit
	./build_for_linux.sh
  ```

### Build on macOS

- My environment
  - macOS Sierra(10.12.1) + xcode8.3.1
  - Homebrew 1.1.3
  - cmake 3.8.0
- Guidance
  
  ```
  cd ZLMediaKit
  ./build_for_mac.sh
  ```

### Build on iOS

  This build method is no longer recommended.It is recommended that make Xcode project by yourself.

- My environment

  Same with Build on macOS

- Guidance
  
  ```
  cd ZLMediaKit
  ./build_for_ios.sh
  ```
  
- You can also generate Xcode projects and recompile them:

  ```
  cd ZLMediaKit
  mkdir -p build
  cd build
  # Generate Xcode project, project file is in build directory
  cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/iOS.cmake -DIOS_PLATFORM=SIMULATOR64 -G "Xcode"
  ```
  



### Build on Android

  Now you can open android sudio project in `Android` folder,this is a `aar library` and damo project.

- My environment
  - macOS Sierra(10.12.1) + xcode8.3.1
  - Homebrew 1.1.3
  - cmake 3.8.0
  - [android-ndk-r14b](https://dl.google.com/android/repository/android-ndk-r14b-darwin-x86_64.zip)
- Guidance 

  ```
  cd ZLMediaKit
  export ANDROID_NDK_ROOT=/path/to/ndk
  ./build_for_android.sh
  ```
### Build on Windows

- My environment
  - windows 10
  - visual studio 2017
  - [cmake-gui](https://cmake.org/files/v3.10/cmake-3.10.0-rc1-win32-x86.msi)
  
- Guidance
```
1 Enter the ZLMediaKit directory and execute git submodule update -- init downloads the code for ZLToolKit
2 Open the project with cmake-gui and generate the vs project file.
3 Find the project file (ZLMediaKit.sln), double-click to open it with vs2017.
4 Choose to compile Release version. Find the target file and run the test case.
```
## Usage

- As server：
	```
	TcpServer::Ptr rtspSrv(new TcpServer());
	TcpServer::Ptr rtmpSrv(new TcpServer());
	TcpServer::Ptr httpSrv(new TcpServer());
	TcpServer::Ptr httpsSrv(new TcpServer());
	
	rtspSrv->start<RtspSession>(mINI::Instance()[Config::Rtsp::kPort]);
	rtmpSrv->start<RtmpSession>(mINI::Instance()[Config::Rtmp::kPort]);
	httpSrv->start<HttpSession>(mINI::Instance()[Config::Http::kPort]);
	httpsSrv->start<HttpsSession>(mINI::Instance()[Config::Http::kSSLPort]);
	```

- As player：
	```
    MediaPlayer::Ptr player(new MediaPlayer());
    weak_ptr<MediaPlayer> weakPlayer = player;
    player->setOnPlayResult([weakPlayer](const SockException &ex) {
        InfoL << "OnPlayResult:" << ex.what();
        auto strongPlayer = weakPlayer.lock();
        if (ex || !strongPlayer) {
            return;
        }

        auto viedoTrack = strongPlayer->getTrack(TrackVideo);
        if (!viedoTrack) {
            WarnL << "none video Track!";
            return;
        }
        viedoTrack->addDelegate(std::make_shared<FrameWriterInterfaceHelper>([](const Frame::Ptr &frame) {
            //please decode video here
        }));
    });

    player->setOnShutdown([](const SockException &ex) {
        ErrorL << "OnShutdown:" << ex.what();
    });

    //rtp transport over tcp
    (*player)[Client::kRtpType] = Rtsp::RTP_TCP;
    player->play("rtsp://admin:jzan123456@192.168.0.122/");
	```
- As proxy server：
	```
	//support rtmp and rtsp url
	//just support H264+AAC
	auto urlList = {"rtmp://live.hkstv.hk.lxdns.com/live/hks",
			"rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov"};
	map<string , PlayerProxy::Ptr> proxyMap;
	int i=0;
	for(auto url : urlList){
		PlayerProxy::Ptr player(new PlayerProxy("live",to_string(i++).data()));
		player->play(url);
		proxyMap.emplace(string(url),player);
	}
	```
	
- As puser：
	```
	PlayerProxy::Ptr player(new PlayerProxy("app","stream"));
	player->play("rtmp://live.hkstv.hk.lxdns.com/live/hks");
	
	RtmpPusher::Ptr pusher;
	NoticeCenter::Instance().addListener(nullptr,Config::Broadcast::kBroadcastRtmpSrcRegisted,
			[&pusher](BroadcastRtmpSrcRegistedArgs){
		const_cast<RtmpPusher::Ptr &>(pusher).reset(new RtmpPusher(app,stream));
		pusher->publish("rtmp://jizan.iok.la/live/test");
	});
	
	```

## Mirrors

[ZLToolKit](http://git.oschina.net/xiahcu/ZLToolKit)

[ZLMediaKit](http://git.oschina.net/xiahcu/ZLMediaKit)


## Licence

```
/*
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
```



## Contact
 - Email：<771730766@qq.com>
 - QQ chat group：542509000


