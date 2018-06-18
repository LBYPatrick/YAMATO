# ss-manager

Travis CI编译状态: [![Build Status](https://travis-ci.org/LBYPatrick/ss-manager.svg?branch=master)](https://travis-ci.org/LBYPatrick/ss-manager)

![README_PIC](./resources/readme_pic.jpg)
画师~~dalao~~: @Bison仓鼠

用一种优雅的方式食用shadowsocks-libev

系统要求: Unix (需要支持Systemd)

~~不检查配置文件语法，跑崩了不负责~~

## 编译

```bash
g++ -std=c++11 -static src/cpp/* -o ss-manager
```

或者你可以用根目录的``compile_release.sh``来进行编译，程序会输出到bin目录里

或者配合 ``CMakeLists.txt`` 使用CMake编译.

## 使用方法

### Console

参数
```bash

    -i or --input <filename>        : 指定配置文件
    -a or --action <action>         : 指定程序行为 (status 对应检查端口状态, load 对应加载配置, unload 对应卸载配置)
    /?, -h or --help                : 显示这则帮助信息
    -e or --extra-parameter <param> : 指定额外的ss-server参数, 你可以结合这个开启UDP转发和HTTP/TLS混淆，因为一些功能只有少数人需要，所以加了这个功能
    -p or --port                    : 指定要检查的端口 (在-a status的时候要用到，具体行为就是显示用户进程的PID和完整的ss-server日志)

```

### Config

Format:

```javascript
{
    "<用户组名>" : "<加密方式全名>" {
    <nameserver> : <可选项_域名解析服务器(DNS)>,
    <redirect>     : <可选项_重定向网站(增强穿墙效果)>,
    <用户端口1>            : <user_password>,
    <用户端口2>            : <user_password2>
    }
}
```

Example:

```javascript
{

    //第一组(可以使用注释)
    "example_group" : "chacha20" : {
        443 : "password1",
        80  : "DoNotBruteForceMePLZ"
    }

    //第二组
    "example_group_2" : "aes-256-cfb" {
        "nameserver" : "8.8.8.8",
        "redirect"   : "isSeptaFucked.com",
        8388 : "test_account"
    }
}
```

## FA♂Q

### 1. 这个软件可以在Windows上跑吗？

不行，即使Windows下能过编译也不要。除了 ``-a load``功能外，其他功能都会用到Linux的命令。

### 2. 为什么要造轮子？

你听说过知乎大佬@vczh么？

### 3. 为什么使用体验极差 ?

因为我是写给自己用的...我把自己的梯子便宜租给其他同学了，写了个工具方便管理用户信息和配置 ~~他们很喜欢上E站和DMM舔小姐姐~~

### 4. 为什么要在使用说明里面放二刺螈？

不喜欢萌妹纸的都是坏蜀黍，哼 (手动傲娇)