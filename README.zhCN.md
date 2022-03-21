# YAMATO

[中文说明](./README.zhCN.md)

一个用来代替ss一键安装脚本的魔法软件，甚至还能分析和统计用户数据

## I. 安装

1. 安装 **shadowsocks-libev** 和其他必备组件

```bash

echo deb http://deb.debian.org/debian stretch-backports main >> /etc/apt/sources.list
apt update
apt install -t stretch-backports -y shadowsocks-libev g++ git

```

2. 克隆并编译YAMATO

```bash
git clone https://github.com/LBYPatrick/YAMATO.git
cd YAMATO
sh build_linux.sh
mv bin/yamato ../
```

1. 编写一个配置文件(见第三部分)。这里我们以``tutorial.yml``举例:

```yaml
//tutorial.yml

group: group_one
method: chacha20-ietf
tunnel_mode: both
    1234 : "Password1"
    2234 : "Password2"

```
4. 使用创建好的配置文件运行YAMATO

```bash
./yamato -a load -i tutorial.yml
```

5. 到这一步其实已经可以用了，但如果你想简化在客户端上的配置流程，你还可以这么做(比如说我们要获取端口1234的信息)

```bash
./yamato -a info -i tutorial.yml -p 1234
```

这个时候YAMATO会输出以下内容:

```bash
Remote Port        : 1234
Local  Port        : 1080
PID                : 10411
Method(Encryption) : chacha20-ietf
Password           : Password1
TCP Fast Open      : true
Tunnel Mode        : tcp_and_udp
Timeout            : 1440
Verbose            : true
SS Link            : ss://Y2hhY2hhMjAtaWV0ZjpQYXNzd29yZDFAZ29vZ2xlLmNvbToxMjM0#group_one-1234
```

看到``SS Link``了吗? 这个链接可以在很多Shadowsocks客户端里快速导入配置(在土豆丝和shadowsocks-qt5里面这个功能叫URI), 直接复制粘贴就可以用了

> 注: 范例里面的URI是用不了的，因为我指向的服务器是google.com而不是你真实的服务器地址，但在你服务器上获得的新链接是可以用的

## II. 用法

### Bash

一些可以使用的参数(鸟语预警):
```bash

-i or --input <filename>        : Specify input file
-a or --action <action>         : Specify action (raw_log,stat, load, unload,log,info,backup_log)
-pn or --profile-name           : Specify profile name(For creating sharable SS:// link)
-s or --server-address          : Specify server address(For creating sharable SS:// link)
/?, -h or --help                : Show this help message
-e or --extra-parameter <param> : Specify additional parameters, you can do things like UDP relay or HTTP/TLS OBFS here
-p or --user_port                    : Specify a user_port for checking status
-li or --log-input              : Specify source syslog file (Not required, this is for analyzing log in devices other than your server)
-o or --output                  : Specify output file name (For stat and log specified with --action)
-q or --quiet                   : Enable Quiet Mode (No output, even including error notifications)
-d or --dump                    : Dump log when stopping a configuration


```

### III. 配置范例

```yaml
group: Contosco
nameserver: 8.8.8.8
method: chacha20-ietf
timeout: 1440
redirect: pornhub.com
fastopen: true
server: 0.0.0.0
tunnel_mode: both            //or you can set it to "tcp" or "both". Please keep in mind that it's NOT "tcp_only" or "tcp_and_udp" because I hate to do it that way.
    8388 : "foo"
    2468 : "bar"

group: MSFT
method: chacha20
    4567 : "Baz"
    5678 : "Qux"
```
