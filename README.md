# ss-manager

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

```json
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

```json
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