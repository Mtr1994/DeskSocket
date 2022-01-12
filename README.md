# DeskSocket

![Platform](https://img.shields.io/badge/paltform-win10--64-brightgreen)
![Qt Version](https://img.shields.io/badge/_Qt_-6.x.x、5.x.x-yellowgreen)
![Build](https://img.shields.io/badge/build-MSVC_2019_x64-blue)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/Mtr1994/DeskSocket)
![GitHub](https://img.shields.io/github/license/Mtr1994/DeskSocket)

`DeskSocket` 是一款用于测试网络通信的桌面（`Windows 10`）工具软件，软件支持以下几种功能

* `TCP 服务端`
* `TCP 客户端` 
* `UDP 服务端` 
* `UDP 客户端`
* `局域网广播`

同时，软件也提供了一些日常工作中常用的计算工具；满足程序员日常工作中对网络数据通信测试的需求。

#### 软件界面

![主界面示例](https://raw.githubusercontent.com/Mtr1994/ImageFactory/master/DeskSocket/main_page.png)

#### 第三方依赖说明

* 软件使用 `HP Socket` 作为通信库，在实际的运行过程中，需要将动态库（./Libs/hpsocket/bin/HPSocket_U.dll）添加到系统环境变量，或者将其拷贝到可执行文件夹所在目录

* 软件使用 `spdlog` 作为日志库，在实际的运行过程中，需要将动态库（./Libs/spdlog/bin/spdlog.dll）添加到系统环境变量，或者将其拷贝到可执行文件夹所在目录
