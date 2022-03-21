//
// Created by LBYPatrick on 3/16/2022.
//

#ifndef YAMATO_CORE_H
#define YAMATO_CORE_H

#include <string>
#include <string_view>
#include <vector>

namespace core {

    int StartSS(std::string_view config_name);
    int StopSS(std::string_view config_name);
    int GetParsedLog(std::string_view config_name, std::vector<std::string> & buffer);
    int StartRPC(std::string_view config_name);
    int StopRPC(std::string_view config_name);
}

#endif //YAMATO_CORE_H
