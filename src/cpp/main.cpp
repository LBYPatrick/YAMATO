#include <vector>
#include "constants.hpp"
#include "util.hpp"
#include "ssm.hpp"

using std::vector;
using std::string;

enum Action {
    STATUS,
    LOAD,
    UNLOAD,
    UNKNOWN
};

int main(int argc, char*const argv[]) {

#if DEBUG
	printf("IP: %s\n", util::GetMachineIP().c_str());
#endif

    //Variables
    string input_file;

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
                        break;
                    }
                }

                if(input_file.size() == 0) {
                    util::ReportError("Need to specify action with -a or --action.");
					return 0;
                }
            }
            if(input_file.size() == 0) {
                vector<string> file_list = util::GetFileList("./");

                for(string & file : file_list) {
                    if(file.find(".pidmap") != -1) {
                        input_file = util::SubString(file,0,file.size()-7);
                        break;
                    }
                }

                if(input_file.size() == 0) {
                    util::ReportError("Need to specify action with -a or --action.");
                    exit(0);
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

}