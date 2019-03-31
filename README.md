# YAMATO

[中文说明](./README.zhCN.md)

Want to use shadowsocks with multiple users and never get bothered by sketchy auto-install scripts? YAMATO is here for you.

## I. Installation

1. Install **shadowsocks-libev** and required components

```bash

echo deb http://deb.debian.org/debian stretch-backports main >> /etc/apt/sources.list
apt update
apt install -t stretch-backports -y shadowsocks-libev g++ git

```

2. Clone our project and compile

```bash
git clone https://github.com/LBYPatrick/YAMATO.git
cd YAMATO
sh build_linux.sh
mv bin/yamato ../
```

3. Write a configuration (See III), for now we can use this file for example:

```yaml
//tutorial.yml

group: group_one
method: chacha20-ietf
tunnel_mode: both
    1234 : "Password1"
    2234 : "Password2"

```
4. Run the program with our configuration

```bash
./yamato -a load -i tutorial.yml
```

5. At this point you should be able to access to this shadowsocks server with the two ports as shown in ``tutorial.yml`` using ``chacha20-ietf`` encryption.
But I know you want to go beyond to make your life better, so you can then do:

```bash
./yamato -a info -i tutorial.yml -p 1234
```

which would get you something like this:

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

See the SS Link? Now you can quickly configure shadowsocks on your client devices using the SS Link(Some software called it "URI")!

> Note: The link shown above does not actually work because it is just an exmaple (i.e. I made the server address to be ``google.com``)

## II. Usage

### Console

Parameters
```bash

-i or --input <filename>        : Specify input file
-a or --action <action>         : Specify action (raw_log,stat, load, unload,log,info,backup_log)
-pn or --profile-name           : Specify profile name(For creating sharable SS:// link)
-s or --server-address          : Specify server address(For creating sharable SS:// link)
/?, -h or --help                : Show this help message
-e or --extra-parameter <param> : Specify additional parameters, you can do things like UDP relay or HTTP/TLS OBFS here
-p or --port                    : Specify a port for checking status
-li or --log-input              : Specify source syslog file (Not required, this is for analyzing log in devices other than your server)
-o or --output                  : Specify output file name (For stat and log specified with --action)
-q or --quiet                   : Enable Quiet Mode (No output, even including error notifications)
-d or --dump                    : Dump log when stopping a configuration

```

### III. Configuration Exmaple

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

## FAQs

### 1. Can this software work on Windows?

Partially. You can try to export your ``/var/log/syslog`` and **configuration's pidmap** from your shadowsocks server and use ``-li`` to locate your syslog and put the pidmap and the configuration itself in the same directory as ``yamato``.

> As of now, everything except **load** and **unload** works on Windows (Maybe MacOS and other linux distributions? Use at your own risk).

### 2. Why did you make such software?

Because I am in charge of providing shadowsocks service to my clients in places where internet is not fully open. Since I need to quickly and conveniently transfer and synchronize configurations to new servers when needed, I created this project for fulfilling my demands.