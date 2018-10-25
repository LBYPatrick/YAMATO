
#ifndef SS_MANAGER_SS_MANAGER_H
#define SS_MANAGER_SS_MANAGER_H

#include <string>
#include <vector>
#include <fstream>
#include "util.hpp"
#include "constants.hpp"
#include "parser.hpp"

using namespace std;

namespace ymt {
	/**
	* @param file_list:  
	*/
	void RunConfig(string filename);
	//string MakeUserConfig(string method, string port, string password, string nameserver, string redirect);
	string RunUser(Parser p);
    void StopConfig(string filename);
    void CheckPort(string filename, string port);
    void SetExtraParam(string extra_param);
	vector<string> GetLog(string pid);
};

#endif //SS_MANAGER_SS_MANAGER_H