//
// Created by LBYPatrick on 2/6/2018.
//

#pragma once
#include "includes.hpp"

namespace util {
    void RemoveLetter(string & original_string, char letter);
    void ReportError(string message);
    void RemoveFile(string filename);
	void AppendStringVector(vector<string> & left, vector<string> & right);
    void ShowHelp(vector<Help> option_list);
	void PercentageBar(int current, int total);
    bool IsFileExist(string filename);
    bool IsProcessAlive(string pid);
	bool IsTheSame(string str, string key, bool is_precise, bool is_case_sensitive);

    vector<string> SysExecute(string cmd);
	vector<string> ReadFile(string path);
	vector<string> ReadFile(string path, bool is_parsed);
    vector<string> GetFileList(string directory);
    vector<string> GetFolderList(string directory);
		
	YAML GetYaml(string line);
	
	string SubString(string str, int left, int stop);
	string GetMachineIP();

    int Search(string str, vector<string> match_list, bool precise);
	
	//Overloaded Methods
	vector<string> GetFileList();
	bool WriteFile(string filename, vector<string> content);
	vector<int> SearchString(string str, char key);
	vector<string> GetFolderList();
	int Search(string str, vector<string> match_list);
}