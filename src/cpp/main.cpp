#include <vector>
#include "constants.h"
#include "Utils.h"
#include "SSManager.h"

#define SOFTWARE_VERSION "0.0.1"


using std::vector;
using std::string;

const vector<Help> option_list = {
        {"-h", " "},
        {"--help", " "},
        {"/?", "Show help"},
        {"load <config_file_name>", "loads users based on the config file specified"},
        {"unload <filename>,","unloads users based on the config file specified (Need to load first)"}
};

int main(int argc, char*const argv[]) {

    if(argc <= 2) {

        if(argc == 2 && (string(argv[1]) == "--help" || string(argv[1]) == "-h" || string(argv[1]) == "/?")) {
            Utils::ShowHelp(option_list);
            return 0;
        }
        else { //Print software info
            printf(
                    "SS-Manager"SOFTWARE_VERSION" by LBYPatrick\n");
            Utils::ShowHelp(option_list);
            return 0;
        }
    }
    else if(argc > 2) {
        const int func_number = Utils::StringToEnum({"load","unload"},argv[1]);

        switch(func_number) {
            case 0:
                SSManager::RunConfig(argv[2]);
                break;
            case 1:
                SSManager::StopConfig(argv[2]);
                break;
            default :
                Utils::ReportError(std::string("Unknown Option: \"") + argv[1] + "\"");
                break;
        }
    }
    else {
        Utils::ReportError("Failed to execute the program");
        return 0;
    }
}