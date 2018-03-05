# ss-manager

[![Build Status](https://travis-ci.org/LBYPatrick/ss-manager.svg?branch=master)](https://travis-ci.org/LBYPatrick/ss-manager)

![README_PIC](./resources/readme_pic.jpg)
Artist: @Bison仓鼠

Use shadowsocks-libev in a more elegant way.

Designed to be cross-platform, but since libev does not support Windows,
so it is Unix only then...


## Usage

### Console

```bash
    ss-manager <config_file>
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

### 1. Why not providing options for "TCP fast open" and "UDP overlay"?

Because there's no point playing around with them. Enabling TCP fastopen does not give you any side-effect. And disabling UDP overlay avoids package loss. (Yes, UDP performs actually WORSE than TCP in the real world)

### 2. Why re-inventing the wheel? 

Because I love to do so.

### 3. Why is the user experience so bad?

Because this software is for myself, but you are always welcome to make suggestions by opening issues.

### 4. Why putting an anime picture in README?

Is there any one who hates anime? I force all of you to like anime.