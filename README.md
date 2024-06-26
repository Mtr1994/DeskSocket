# DeskSocket

![Platform](https://img.shields.io/badge/paltform-win10_x64-brightgreen)
![Qt Version](https://img.shields.io/badge/_Qt_-5.15.2-yellowgreen)
![Build](https://img.shields.io/badge/build-MSVC_2019_x64-blue)

#### 一、简介

`DeskSocket` 是一款用于测试网络通信的桌面（`Windows 10`）工具软件，软件支持以下几种功能

* `TCP 服务端`
* `TCP 客户端` 
* `UDP 服务端` 
* `UDP 客户端`
* `UDP 广播端`

同时，软件也提供了一些日常工作中常用的计算工具；满足程序员日常工作中对网络数据通信测试的需求。

#### 二、依赖说明

* 软件使用 `HP Socket` 作为通信库，在实际的运行过程中，需要将动态库（`./Libs/hpsocket/bin/HPSocket.dll`）添加到系统环境变量，或者将其拷贝到可执行文件夹所在目录

* 软件使用 `spdlog` 作为日志库，在实际的运行过程中，需要将动态库（`./Libs/spdlog/bin/spdlog.dll`）添加到系统环境变量，或者将其拷贝到可执行文件夹所在目录

#### 三、功能展示

​	![](Resource/help/p1.png)
