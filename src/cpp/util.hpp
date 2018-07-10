//
// Created by LBYPatrick on 2/6/2018.
//

#ifndef SS_MANAGER_UTILS_H
#define SS_MANAGER_UTILS_H
#include <string>
#include "constants.hpp"
#include <cstdio>
#include <fstream>

using namespace std;

namespace util {
    void RemoveLetter(string & original_string, char letter);
    Json GetJson(string raw_json_line);
    void ReportError(string message);
    void RemoveFile(string filename);
    void ShowHelp(vector<Help> option_list);
    bool IsFileExist(string filename);
    bool IsProcessAlive(int pid);
    vector<string> SysExecute(string cmd);

    int Search(string str, vector<string> match_list);

    vector<string> GetFileList();
    vector<string> GetFolderList(string directory);

    int Search(string str, vector<string> match_list, bool precise);
}

#endif //SS_MANAGER_UTILS_H
