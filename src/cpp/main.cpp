#pragma once

#include <vector>
#include "constants.h"
#include "Utils.h"


using std::vector;
using std::string;

int main(int argc, char*const argv[]) {
	
	string param_buffer;
	vector<string> ConfigFileList;

	string a = "wang :";

	Json output = Utils::GetJson(a);
	printf(output.element.c_str());
	printf("\n%s\n",output.key.c_str());

	for(int i = 1; i < argc; ++i) {
		
		param_buffer = argv[i];
		if (param_buffer.find("-i") != string::npos || param_buffer.find("--input-file") != string::npos) {
			if(i+2 < argc) {
				ConfigFileList.push_back(argv[i+1]);
			}
		}
	}

		
}