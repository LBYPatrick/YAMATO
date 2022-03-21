//
// Created by LBYPatrick on 3/18/2022.
//

#ifndef YAMATO_LOG_H
#define YAMATO_LOG_H

#include<string>
#include<string_view>
#include<unordered_map>
#include<mutex>
#include<vector>

struct Log {
    int pid = -1,
        user_port = -1,
        site_port = -1;
    std::string group_name;
    time_t time = (time_t)(-1);
    std::string site;

    std::string ToString(std::string & buffer);
    std::vector<std::string> ToVector();
    std::unordered_map<std::string,int> GetColumnMapping();
};


#endif //YAMATO_LOG_H
