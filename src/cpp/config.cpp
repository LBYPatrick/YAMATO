//
// Created by LBYPatrick on 3/16/2022.
//

#include "config.h"

using std::string;
using std::string_view;

const std::unordered_map<string_view,string> kDefaultMap = {
        {"server" , "0,0,0,0"},
        {"local_port","1080"},
        {"remote_port","65535"},
        {"tcp_fastopen","true"},
        {"tunnel_mode","both"},
        {"method","chacha20-ietf-poly1305"},
        {"tag","default"},
        {"password","this_is_an_example_password"}

};

Config default_(kDefaultMap);

Config::Config() {
    parent_ = &default_;
    parent_->AddChild(this);
}

Config::Config(Config * parent, bool is_ram_aggressive) {
    parent_ = parent;
    is_ram_aggressive = is_ram_aggressive;
    if(parent) parent_->AddChild(this);
}

Config::Config(std::unordered_map<std::string_view, std::string> key_map_) {
    map_ = std::move(key_map_);
}


bool Config::IsKeyValid(std::string_view key) {
    return kDefaultMap.find(key) != kDefaultMap.end();
}

int
Config::Get(string_view key, string &buffer) {

    int ret = 0;

    mutex_.lock();

    if(map_.find(key) == map_.end()) {
        if(!parent_)
            ret = ERR_CONFIG_KEY_INVALID;
        else {
            ret = parent_->Get(key, buffer);
            if(ret == 0 && is_ram_aggressive) map_[key] = buffer;
        }
    }
    else
        buffer = map_[key];

    mutex_.unlock();

    return ret;
}

int
Config::Set(string_view key, string value) {

    int ret = 0;

    mutex_.lock();

    if(IsKeyValid(key))
        map_[key] = value;
    else ret = ERR_CONFIG_KEY_INVALID;

    mutex_.unlock();

    return ret;
}

void Config::AddChild(Config * child) {
    child_.push_back(child);
}

std::vector<Config *> Config::GetChildren() {
    return child_;
}

Config * Config::GetRoot() {
    return &default_;
}



