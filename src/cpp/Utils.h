//
// Created by LBYPatrick on 2/6/2018.
//

#ifndef SS_MANAGER_UTILS_H
#define SS_MANAGER_UTILS_H
#include <string>
#include "constants.h"
#include <cstdio>
#include <fstream>

namespace Utils {
    void RemoveLetter(std::string & original_string, char letter);
    Json GetJson(std::string raw_json_line);
    void ReportError(std::string message);
    void RemoveFile(std::string filename);
    void ShowHelp(std::vector<Help> option_list);
    int  StringToEnum(std::vector<std::string> fake_string_enum, std::string input);
    bool IsFileExist(std::string filename);
    std::vector<std::string> SysExecute(std::string cmd);
}

#endif //SS_MANAGER_UTILS_H
