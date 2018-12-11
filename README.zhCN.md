# YAMATO

[![Build Status](https://travis-ci.org/LBYPatrick/YAMATO.svg?branch=master)](https://travis-ci.org/LBYPatrick/ss-manager)

![README_PIC](./resources/readme_pic.jpg)
画师: [@四騎](https://www.pixiv.net/member.php?id=1845467)

[中文说明](./README.zhCN.md)

用YAML来控制shadowsocks-libev，多用户，无常驻后台，C++编写，轻便快捷

## 编译

```bash
g++ -std=c++11 -static src/cpp/*.cpp -o yamato
```

## 使用方法

### 控制台

参数
```bash

    -i or --input <filename>        : specify input file
    -a or --action <action>         : specify action (status, load, unload)
    /?, -h or --help                : show this help message
    -e or --extra-parameter <param> : specify additional parameters, you can do things like UDP relay or HTTP/TLS OBFS here
    -p or --port                    : specify a port for checking status
```

> 因为实在不方便翻译，所以在此举个例子：

```bash
yamato -a load -i 测试配置.yaml -e "-u" //加载测试配件.yaml并通过额外参数-u开启UDP转发

yamato -a status -i 测试配置.yaml -p 8388 //检查测试配置.yaml中端口8388的系统日志
```

### 配置文件(YAML)

```yaml
group: Contosco
nameserver: 8.8.8.8
method: chacha20-ietf
timeout: 1440
redirect: pornhub.com           //有时候你得学着邪恶一点
fastopen: true
server: 0.0.0.0
tunnel_mode: tcp                //或者写成udp/both,注意不是原版的"tcp_and_udp"或者"udp_only"这样的，因为太智障
    8388 : "用户1的密码"
    2468 : "用户2的密码"

group: MSFT
//其他的配置项可以留空，此时会根据上一个组使用过的信息进行配置
//其实除了用户的端口和密码之外你都可以不写，因为我都有缺省值(但是缩进是必须的！)
//我比较讨厌用警号注释代码，所以我支持了双斜杠注释
    4567 : "Baz"
    5678 : "Qux"
```

## FAQs

### 1. 这个软件可以在Windows上跑吗

不行，除非你大刀阔斧去改util.cpp, 让一部分的API支持Windows,因为很多功能都是靠着Linux的一些命令实现的

> 当然 ``1.2.0`` 之后某些暂未公开的功能实际上是支持Windows的，但经过测试发现并没有直接在服务器上跑快 (我指的是至强E3的VPS, 如果你服务器是树莓派的话当我没说)

### 2. 为什么你要造轮子？

你怕是不知道Gaclib...

### 3. 为何XX功能如此残废？

上班族砍产品经理的需求，我直接砍用户

### 4. 为啥说明书里有伊欧娜？

为了这个我连项目名称都从``ss-manager``改成了``yamato``, 目的够明显了吧...
