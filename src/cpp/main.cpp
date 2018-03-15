#include <vector>
#include "constants.h"
#include "Utils.h"
#include "SSManager.h"

#define SOFTWARE_VERSION "0.1.2"


using std::vector;
using std::string;

const vector<Help> option_list = {
        {"-h", " "},
        {"--help", " "},
        {"/?", "Show help"},
        {"load <config_file_name>", "loads users based on the config file specified"},
        {"unload <config_file_name>,","unloads users based on the config file specified (Need to load first)"},
        {"status <config_file_name> <user_port>","get status of a port loaded with a specific config"}
};

int main(int argc, char*const argv[]) {

    if(argc <= 2) {
        
        if(argc == 2 && (string(argv[1]) == "--help" || string(argv[1]) == "-h" || string(argv[1]) == "/?")) {
            Utils::ShowHelp(option_list);
            return 0;
        }
        else { //Print software info
            printf(
                    "SS-Manager v" SOFTWARE_VERSION " by LBYPatrick\n");
            Utils::ShowHelp(option_list);
            return 0;
        }
    }
    else if(argc > 2) {
        const int func_number = Utils::StringToEnum({"load","unload", "status"},argv[1]);

        switch(func_number) {
            case 0:
                SSManager::RunConfig(argv[2]);
                break;
            case 1:
                SSManager::StopConfig(argv[2]);
                break;
            case 2:
                if(argc < 4) { Utils::ReportError("Need to specify user port"); }
                else { SSManager::CheckPort(argv[2],argv[3]); }
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