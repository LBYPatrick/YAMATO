# ss-manager

[![Build Status](https://travis-ci.org/LBYPatrick/ss-manager.svg?branch=master)](https://travis-ci.org/LBYPatrick/ss-manager)

![README_PIC](./resources/readme_pic.jpg)
Artist: @Bison仓鼠

[中文说明](./README.zhCN.md)

Use shadowsocks-libev in a more elegant way.

System Requirement: any unix-based system with systemd support.

## Build

```bash
g++ -std=c++11 -static src/cpp/* -o ss-manager
```

Or use CMake with ``CMakeLists.txt``.

## Usage

### Console

Parameters
```bash
    -h                                    :
    --help                                :
    /?                                    : Show help
    load <config_file_name>               : loads users based on the config file specified
    unload <config_file_name>,            : unloads users based on the config file specified (Need to load first)
    status <config_file_name> <user_port> : get status of a port loaded with a specific config

```

### Config

Format:

```javascript
{
    "<group_name>" : "<method_name>" {
    <optional_nameserver> : <nameserver>,
    <optional_redirect>   : <redirect_site>,
    <user_port_1>         : <user_password>,
    <user_port_2>         : <user_password2>
    }
}
```

Example:

```javascript
{

    #Group 1 (Yes we support COMMENTS IN JSON, Yay!)

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

### 1. Can this software work on Windows?

WARNING: Don't ever run this software on Windows even though it passes compilation. All functions except ``load`` requires linux system commands.

### 2. Why not providing options for "TCP fast open" and "UDP overlay"?

Because there's no point playing around with them. Enabling TCP fastopen does not give you any side-effect. And disabling UDP overlay avoids package loss. (Yes, UDP performs actually WORSE than TCP in the real world)

### 3. Why re-inventing the wheel? 

Because I love to do so.

### 4. Why is the user experience so bad?

Because this software is for myself, but you are always welcome to make suggestions by opening issues. Plus, what do you expect from a less-than-1000-line program?

### 5. Why putting an anime picture in README?

Is there any one who hates anime? I force all of you to like anime.