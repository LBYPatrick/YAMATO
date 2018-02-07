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
	void GenerateCommand(string filename, vector<string> & output_buffer);
};