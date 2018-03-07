//
// Created by LBYPatrick on 2/6/2018.
//

#ifndef SS_MANAGER_UTILS_H
#define SS_MANAGER_UTILS_H
#include <string>
#include "constants.h"
#include <cstdio>
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

namespace Utils {
    void RemoveLetter(string & original_string, char letter);
    Json GetJson(string raw_json_line);
    void ReportError(string message);
    void RemoveFile(string filename);
    void ShowHelp(vector<Help> option_list);
    int  StringToEnum(vector<string> fake_string_enum, string input);
    bool IsFileExist(string filename);
    vector<string> SysExecute(string cmd);
}

#endif //SS_MANAGER_UTILS_H
