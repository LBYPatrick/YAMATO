#pragma once

#include "includes.hpp"
#include "parser.hpp"

namespace ymt {

	void RunConfig();
	//string MakeUserConfig(string method, string port, string password, string nameserver, string redirect);
	string RunUser(Parser &p);
    void StopConfig();
    void CheckPort(string port);
    void SetExtraParam(string extra_param);
	void SetAttribute(YMTAttributes attribute, string &value);
	vector<string> GetLog(string pid);
	vector<PIDInfo> GetPIDTable();
	void UpdatePIDTable();
	vector<SSLog> GetFormattedData();
	vector<string> GetFormattedStringData();
	vector<string> GetStatisics();
	void SetFileName(string filename);
	void UpdateLog();
	void CleanSyslog();
};