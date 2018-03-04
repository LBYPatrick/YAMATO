//
// Created by LBYPatrick on 2/6/2018.
//

#ifndef SS_MANAGER_UTILS_H
#define SS_MANAGER_UTILS_H
#include <string>
#include "constants.h"
#include <cstdio>

using std::string;

namespace Utils {
    void RemoveLetter(string & original_string, char letter);
    Json GetJson(string raw_json_line);
    void ReportError(string message);
    void RemoveFile(string filename);
    void ShowHelp(std::vector<Help> option_list);
}

#endif //SS_MANAGER_UTILS_H
