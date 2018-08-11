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
    void ReportError(string message);
    void RemoveFile(string filename);
    void ShowHelp(vector<Help> option_list);
    bool IsFileExist(string filename);
    bool IsProcessAlive(string pid);
    vector<string> SysExecute(string cmd);
	vector<string> ReadFile(string filename);
    vector<string> GetFileList(string directory);
    vector<string> GetFolderList(string directory);
    
    int Search(string str, vector<string> match_list, bool precise);
	
	//Overloaded Methods
	vector<string> GetFileList();
	vector<string> GetFolderList();
	int Search(string str, vector<string> match_list);
	YAML GetYaml(string line);
	string SubString(string str, int left, int stop);
}

#endif //SS_MANAGER_UTILS_H
