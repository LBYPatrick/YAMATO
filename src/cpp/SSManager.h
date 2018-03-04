#pragma once
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
	void ExecuteFile(string filename);
	bool readFile(string filename, vector<string> & file_buffer);
	string MakeUser(string method, string port, string password, string nameserver, string redirect);
};