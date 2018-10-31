
#ifndef YMT_HPP
#define YMT_HPP

#include <string>
#include <vector>
#include <fstream>
#include "util.hpp"
#include "constants.hpp"
#include "parser.hpp"

using namespace std;

struct PIDInfo {
	string port;
	string pid;
};

enum YMTAttributes {
	CONFIG_FILENAME,
	LOG_OUTPUT_FILENAME,
	LOG_INPUT_FILENAME,
	EXTRA_PARAM
};

enum LogBehavior {
	CONNECT,
	HANDSHAKE_FAIL
};

struct SSLog {
	string time;
	string port;
	string pid;
	string destination;
	LogBehavior behavior;
};

struct InquiryData {
	string key;
	long int value;
};

namespace ymt {

	void RunConfig();
	//string MakeUserConfig(string method, string port, string password, string nameserver, string redirect);
	string RunUser(Parser p);
    void StopConfig();
    void CheckPort(string port);
    void SetExtraParam(string extra_param);
	void SetAttribute(YMTAttributes attribute, string value);
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

#endif //YMT_HPP