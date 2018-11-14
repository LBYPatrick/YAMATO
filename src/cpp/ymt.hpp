#pragma once

#include "includes.hpp"
#include "parser.hpp"

class ymt {
public:
	static void RunConfig();
    static void StopConfig();
    static void CheckPort(string port);
    static void SetExtraParam(string extra_param);
	static void SetAttribute(YMTAttributes attribute, string &value);
	static vector<string> GetLog(string pid);
	static vector<SSLog> GetFormattedData();
	static vector<string> GetFormattedStringData();
	static vector<string> GetStatisics();
	static void SetFileName(string filename);
private:
    static string RunUser(Parser &p);
    static void UpdateLog();
    static void CleanSyslog();
    static vector<PIDInfo> GetPIDTable();
    static void UpdatePIDTable();
};