#pragma once

#include "constants.h"

using std::string;

int main(int argc, char*const argv[]) {
	
	string param_buffer;
	
	for(int i = 1; i < argc; ++i) {
		
		param_buffer = argv[i];
		if (param_buffer.find("-i") != string::npos || param_buffer.find("--input-file") != string::npos) {
			if(i+2 < argc) {
				ConfigFileList.push_back(argv[i+1]);
			}
		}
	}

		
}