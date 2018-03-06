
#ifndef SS_MANAGER_SS_MANAGER_H
#define SS_MANAGER_SS_MANAGER_H

#include <string>
#include <vector>
#include <fstream>
#include "Utils.h"
#include "constants.h"

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

namespace SSManager {
	/**
	* @param file_list:  
	*/
	void RunConfig(string filename);
	bool ReadFile(string filename, vector<string> & file_buffer);
	string MakeUserConfig(string method, string port, string password, string nameserver, string redirect);
	void RunUsers(std::vector<User> & user_list, string & encryption, string & nameserver, string & redirect,std::vector<std::string> & pid_list_buffer);
    void WritePidMap(string filename, std::vector<string> & pid_list,std::vector<string> & port_list);
    void StopConfig(string filename);
};

#endif //SS_MANAGER_SS_MANAGER_H