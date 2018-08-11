#include <vector>
#include "constants.hpp"
#include "util.hpp"
#include "ssm.hpp"

using std::vector;
using std::string;

#if LEGACY
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
#endif

enum Action {
    STATUS,
    LOAD,
    UNLOAD,
    UNKNOWN
};

int main(int argc, char*const argv[]) {

#if LEGACY
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
#else
    //Variables
    string
           input_file;
    Action action = UNKNOWN;
    string port;

#pragma region Collect parameters
    for(int a = 1; a < argc; ++a) {

        if (util::Search(argv[a], {"-i", "--input"}, true) != -1) {
            if (a + 1 < argc) input_file = argv[a + 1];
            else {
                util::ReportError("You need to specify filename when using -i or --input.");
                exit(0);
            }
            a += 1;
        }

        else if (util::Search(argv[a], {"-a", "--action"}, true) != -1) {

            //Parameter pre-check
            if (a + 1 >= argc) {
                util::ReportError("You need to specify action when using -a or --action.");
                break;
            }

            if (util::Search(argv[a + 1], {"status"},1) != -1) {
                action = STATUS;
            } else if (util::Search(argv[a + 1], {"load"},1) != -1) {
                action = LOAD;
            } else if (util::Search(argv[a + 1], {"unload"},1) != -1) {
                action = UNLOAD;
            } else {
                util::ReportError("Unknown action: " + string(argv[a + 1]) + ".");
                break;
            }
            a += 1;
        }

        else if (util::Search(argv[a], {"/?", "-h", "--help"}, true) != -1) {
            printf("YAMATO " SOFTWARE_VERSION " by LBYPatrick\n");
            util::ShowHelp({
                                   {"-i or --input <filename>",        "specify input file"},
                                   {"-a or --action <action>",         "specify action (status, load, unload)"},
                                   {"/?, -h or --help",                "show this help message"},
                                   {"-e or --extra-parameter <param>", "specify additional parameters, you can do things like UDP relay or HTTP/TLS OBFS here"},
                                   {"-p or --port",                    "specify a port for checking status"}

                           });
            printf("\n");
            return 0;
        }

		else if (util::Search(argv[a], { "-e", "--extra-parameter" }, true) != -1) {

			if (a + 1 >= argc) {
				util::ReportError("Your need to specify extra parameters (With quote if needed) when using -e or --extra-parameter.");
				return 0;
			}

			ssm::SetExtraParam(argv[a + 1]);
			a += 1;
		}

        else if (util::Search(argv[a], {"-p","--port"},true) != -1) {

            if(a+1 >= argc) {
                util::ReportError("You need to specify a port when using -p");
                return 0;
            }

            port = argv[a+1];

            a += 1;
        }

        else {
            util::ReportError("Unknown option: " + string(argv[a]) + ".");
            return 0;
        }
    }
#pragma endregion

#pragma region parameter check & execute
    //Finish collecting parameters, run pre-check
    if (action == UNKNOWN) {
        util::ReportError("Need to specify action with -a or --action.");
		return 0;
    }

    //Start Execution
    switch(action) {
        case STATUS :

            if(port.size() == 0) {
                util::ReportError("You need to specify a port for checking status");
				return 0;
            }
            if(input_file.size() == 0) {
                vector<string> file_list = util::GetFileList("./");

                for(string & file : file_list) {
                    if(file.find(".pidmap") != -1) {
                        input_file = file.substr(0,file.size()-7);
#if DEBUG
                        printf("INPUT_FILE: %s\n",input_file.c_str());
#endif
                        break;
                    }
                }

                if(input_file.size() == 0) {
                    util::ReportError("Need to specify action with -a or --action.");
					return 0;
                }
            }
            ssm::CheckPort(input_file,port);
            break;

        case LOAD :
            if (input_file.size() == 0) {
                util::ReportError("Need to specify input file with -i or --input.");
				return 0;
            }
            ssm::RunConfig(input_file);
            break;

        case UNLOAD :
            if (input_file.size() == 0) {
                util::ReportError("Need to specify input file with -i or --input.");
				return 0;
            }
            ssm::StopConfig(input_file);
            break;
    }

    return 1;
#pragma endregion

#endif
}