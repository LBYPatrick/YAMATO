//
// Created by LBYPatrick on 3/16/2022.
//

#ifndef YAMATO_CONFIG_H
#define YAMATO_CONFIG_H

#define ERR_CONFIG_KEY_INVALID 1
#define ERR_CONFIG_KEY_PROTECTED 2

#include<string>
#include<string_view>
#include<unordered_map>
#include<mutex>



class Config {

private:
    std::unordered_map<std::string_view,std::string> map_;
    Config * parent_ = NULL;
    std::mutex mutex_;
    bool is_ram_aggressive = false;

public:
    int Get(std::string_view key,std::string & buffer);
    int Set(std::string_view key,std::string value);
    bool IsKeyValid(std::string_view key);
    Config();
    explicit Config(Config * parent,bool is_ram_agressive);
    explicit Config(Config * parent) : Config(parent,false) {};
    explicit Config(std::unordered_map<std::string_view,std::string> key_map_);
};


#endif //YAMATO_CONFIG_H
