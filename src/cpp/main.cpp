#include <vector>
#include "constants.hpp"
#include "util.hpp"
#include "ssm.hpp"

#define SOFTWARE_VERSION "0.1.2"


using std::vector;
using std::string;

const vector<Help> option_list = {
        {"-h", " "},
        {"--help", " "},
        {"-v"," "},
        {"--version", " "},
        {"/?", "Show help"},
        {"load <config_file_name>", "loads users based on the config file specified"},
        {"unload <config_file_name>","unloads users based on the config file specified (Need to load first)"},
        {"status <config_file_name> <user_port_or_process_pid>","get status of a port loaded with a specific config"}
};

int main(int argc, char*const argv[]) {

    if(argc <= 2) {
        
        if(argc == 2 && (util::Search(argv[1], {"--help", "-h", "/?", "-v", "--version"}) != -1)) {
            util::ShowHelp(option_list);
            return 0;
        }
        else { //Print software info
            printf(
                    "SS-Manager v" SOFTWARE_VERSION " by LBYPatrick\n");
            util::ShowHelp(option_list);
            return 0;
        }
    }
    else if(argc > 2) {
        const int func_number = util::Search(argv[1],{"load","unload", "status"});

        switch(func_number) {
            case 0:
                ssm::RunConfig(argv[2]);
                break;
            case 1:
                ssm::StopConfig(argv[2]);
                break;
            case 2:
                if(argc < 4) { util::ReportError("Need to specify user port"); }
                else { ssm::CheckPort(argv[2],argv[3]); }
                break;

            default :
                util::ReportError(std::string("Unknown Option: \"") + argv[1] + "\"");
                break;
        }
    }
    else {
        util::ReportError("Failed to execute the program");
        return 0;
    }
}