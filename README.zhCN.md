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

或者配合 ``CMakeLists.txt`` 使用CMake编译.

## 使用方法

### Console

参数
```bash
    -h                                    :
    --help                                :
    /?                                    : 显示帮助(英文)
    load <config_file_name>               : 加载指定的ss-manager配置
    unload <config_file_name>,            : 卸载指定的ss-manager配置(需要先挂载过)
    status <config_file_name> <user_port> : 获得在指定ss-manager配置下的特定用户的状态信息(系统信息)

```

### Config

Format:

```javascript
{
    "<用户组名>" : "<加密方式全名>" {
    <可选项_域名解析服务器> : <nameserver>,
    <可选项_重定向网页>     : <redirect_site>,
    <用户端口1>            : <user_password>,
    <用户端口2>            : <user_password2>
    }
}
```

Example:

```javascript
{

    #用户组1 (嘿嘿，JSON里加注释这种功能肯定要搞一个出来啦）

    "example_group" : "chacha20" : {
        443 : "password1",
        80  : "DoNotBruteForceMePLZ"
    }

    "example_group_2" : "aes-256-cfb" {
        "nameserver" : "8.8.8.8",
        "redirect"   : "isSeptaFucked.com",
        8388 : "test_account"
    }
}
```

## FAQs

### 1. 这个软件可以在Windows上跑吗？

不行，即使Windows下能过编译也不要。除了 ``load``功能外，其他功能都会用到Linux的命令~~到时候你电脑炸了我可不负责，哼~~

### 2. 为什么不提供 "TCP fast open" 和 "UDP overlay" 的选项?

因为瞎折腾这两个选项没屌用。fastopen是开的,udp重传是关的（因为实际使用效果真的不如TCP，网络二等公民，天天丢包）

### 3. 为什么要造轮子？ 

你听说过知乎大佬@vczh么？

### 4. 为什么使用体验极差 ?

因为我是写给自己用的，当然你们可以开issue提建议或者汇报bug. 另外，1000行以内的小程序不要抱有太大的期望值。

### 5. 为什么要在使用说明里面放二刺猿？

不喜欢萌妹子的都是坏人。