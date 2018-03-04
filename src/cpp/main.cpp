#pragma once

#include <vector>
#include "constants.h"
#include "Utils.h"
#include "SSManager.h"


using std::vector;
using std::string;

const vector<Help> option_list = {
        {"-h", " "},
        {"--help", " "},
        {"/?", " "}
};

int main(int argc, char*const argv[]) {

    if(argc >= 2) {

        if(string(argv[1]) == "--help" || string(argv[1]) == "-h" || string(argv[1]) == "/?") {
            printf("Simple format: ss-manager <filename>\n"
            "More options available: \n\n");
            Utils::ShowHelp(option_list);
            return 0;
        }
        else { SSManager::RunConfig(argv[1]);}

        return 1;
    }
    else {
        Utils::ReportError("Need to Specify a config file");
        return 0;
    }
}