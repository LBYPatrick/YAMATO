# YAMATO

[![Build Status](https://travis-ci.org/LBYPatrick/YAMATO.svg?branch=master)](https://travis-ci.org/LBYPatrick/ss-manager)

![README_PIC](./resources/readme_pic.jpg)
Artist: [@四騎](https://www.pixiv.net/member.php?id=1845467)

[中文说明](./README.zhCN.md)

Use shadowsocks-libev in a more elegant way, just like YAMATO leading all IJN ships.

## Build

```bash
g++ -std=c++11 -static src/cpp/*.cpp -o yamato
```

Or use CMake with ``CMakeLists.txt``.

## Usage

### Console

Parameters
```bash

-i or --input <filename>        : specify input file
-a or --action <action>         : specify action (raw_log,stat, load, unload,log,info)
-pn or --profile-name           : specify profile name(For creating sharable SS:// link)
-s or --server-address          : Specify server address(For creating sharable SS:// link)
/?, -h or --help                : show this help message
-e or --extra-parameter <param> : specify additional parameters, you can do things like UDP relay or HTTP/TLS OBFS here
-p or --port                    : specify a port for checking status
-li or --log-input              : specify source syslog file (Not required, this is for analyzing log in devices other than your server)
-o or --output                  : specify output file name (For stat and log specified with --action, the default output filename is yamato_analyzed.log)


```

### Config(YAML!)

```yaml
group: Contosco
nameserver: 8.8.8.8
method: chacha20-ietf
timeout: 1440
redirect: pornhub.com
fastopen: true
server: 0.0.0.0
tunnel_mode: udp            //or you can set it to "tcp" or "both". Please keep in mind that it's NOT "tcp_only" or "tcp_and_udp" because I hate to do it that way.
    8388 : "foo"
    2468 : "bar"

group: MSFT
//The rest can be omitted, and the config for "Contosco" will be used(But not user credentials)
//Actually everything (obviously not user-pass) can be omitted since I have defaults hard-coded in the program
//Oh and I hate using "HASH" for code commenting. Instead, double-slash can be used
    4567 : "Baz"
    5678 : "Qux"
```

## FAQs

### 1. Can this software work on Windows?

**WARNING**: Don't ever run this software on Windows even though it passes compilation. All functions except ``load`` requires linux system commands.

### 2. Why re-inventing the wheel?

Why did Debian/Fedora/Ubuntu/ArchLinux appeared?

### 3. Why is the user experience so bad?

Did not count in the audience, lo siento.

### 4. Why putting an anime picture in README?

See, I changed the project name from ``ss-manager`` to ``YAMATO``. A picture from **Arpeggio of Blue Steel** is needed (Also I need a good one for YAMATO, now it's I-401)