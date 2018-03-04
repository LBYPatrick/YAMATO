#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Utils.h"

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
	string makeUser(string name, string port, string password, string fastopen, string nameserver, string redirect);
};