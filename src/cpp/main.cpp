#pragma once

#include <vector>
#include "constants.h"
#include "Utils.h"
#include "SSManager.h"


using std::vector;
using std::string;

int main(int argc, char*const argv[]) {

    if(argc > 1) SSManager::ExecuteFile(argv[1]);
    system("pause");
    return 0;
		
}