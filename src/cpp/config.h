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
#include<vector>

class Config {

private:
    std::unordered_map<std::string_view,std::string> map_;
    Config * parent_ = NULL;
    std::mutex mutex_;
    std::vector<Config *> child_;
    bool is_ram_aggressive = false;


public:
    void AddChild(Config * child);
    std::vector<Config *> GetChildren();
    int Get(std::string_view key,std::string & buffer);
    int Set(std::string_view key,std::string value);
    static Config * GetRoot();
    bool IsKeyValid(std::string_view key);
    Config();

    //By setting is_ram_aggressive to true,
    // The map of children would be populated when the value is taken from its parent,
    //Which inherently will make Get() an O(1) operation instead of O(h), where h \in \mathbb{N} stands for the tree height
    explicit Config(Config * parent,bool is_ram_aggressive);

    //is_ram_aggressive is set to true by default
    explicit Config(Config * parent) : Config(parent,true) {};
    explicit Config(std::unordered_map<std::string_view,std::string> key_map_);
};


#endif //YAMATO_CONFIG_H
