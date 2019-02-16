#include <vector>

#include "includes.hpp"
#include "util.hpp"
#include "ymt.hpp"

enum Action {
    EXPORT_RAW_LOG,
    LOAD,
    UNLOAD,
    EXPORT_LOG,
    EXPORT_STAT,
    UNKNOWN,
    INFO
};

int main(int argc, char *const argv[]) {

    //Variables
    string input_file;
	string out_file;
    string input_log_file;
	vector<string> out_temp;
    Action action = UNKNOWN;
    string port;
	bool is_developer_mode = false;
	bool is_quiet_mode = false;

#pragma region Collect parameters
    for (int a = 1; a < argc; ++a) {

        if (util::MatchWithWords(argv[a], {"-i", "--input"}, true) != -1) {
            if (a + 1 < argc) input_file = argv[a + 1];
            else {
                util::ReportError("You need to specify filename when using -i or --input.");
                return 0;
            }
            a += 1;
        } else if (util::MatchWithWords(argv[a], {"-a", "--action"}, true) != -1) {

            //Parameter pre-check
            if (a + 1 >= argc) {
                util::ReportError("You need to specify action when using -a or --action.");
                break;
            }

            if (util::MatchWithWords(argv[a + 1], {"stat"}, true) != -1) {
                action = EXPORT_STAT;
            } else if (util::MatchWithWords(argv[a + 1], {"load"}, true) != -1) {
                action = LOAD;
            } else if (util::MatchWithWords(argv[a + 1], {"unload"}, true) != -1) {
                action = UNLOAD;
            } else if (util::MatchWithWords(argv[a + 1], {"log"}, true) != -1) {
                action = EXPORT_LOG;
            } else if (util::MatchWithWords(argv[a + 1], {"raw_log"}, true) != -1) {
                action = EXPORT_RAW_LOG;
            } else if (util::MatchWithWords(argv[a + 1], {"info", "user-information"}, true) != -1) {
                action = INFO;
            } else {
                util::ReportError("Unknown action: " + string(argv[a + 1]) + ".");
                break;
            }
            a += 1;
        } else if (util::MatchWithWords(argv[a], {"/?", "-h", "--help"}, true) != -1) {
            printf("YAMATO " SOFTWARE_VERSION " by LBYPatrick\n");
            util::ShowHelp({
                                   {"-i or --input <filename>",        "Specify input file"},
                                   {"-a or --action <action>",         "Specify action (raw_log,stat, load, unload,log,info)"},
                                   {"-pn or --profile-name",           "Specify profile name(For creating sharable SS:// link)"},
                                   {"-s or --server-address",          "Specify server address(For creating sharable SS:// link)"},
                                   {"/?, -h or --help",                "Show this help message"},
                                   {"-e or --extra-parameter <param>", "Specify additional parameters, you can do things like UDP relay or HTTP/TLS OBFS here"},
                                   {"-p or --port",                    "Specify a port for checking status"},
                                   {"-li or --log-input",              "Specify source syslog file (Not required, this is for analyzing log in devices other than your server)"},
                                   {"-o or --output",                  "Specify output file name (For stat and log specified with --action)"},
								   {"-d or --dev",					   "Enable Developer Mode (Only log would be printed)"},
								   {"-q or --quiet",				   "Enable Quiet Mode (No output would be printed)"}

                           });
            printf("\n");
            return 0;
        } else if (util::MatchWithWords(argv[a], {"-e", "--extra-parameter"}, true) != -1) {

            if (a + 1 >= argc) {
                util::ReportError(
                        "Your need to specify extra parameters (With quote if needed) when using -e or --extra-parameter.");
                return 0;
            }

            ymt::SetExtraParam(argv[a + 1]);
            a += 1;
        } else if (util::MatchWithWords(argv[a], {"-o", "--output"}, true) != -1) {
            if (a + 1 >= argc) {
                util::ReportError("You need to specify a filename when using -o or --output.");
                return 0;
            }

            out_file = argv[a + 1];

            a += 1;
        } else if (util::MatchWithWords(argv[a], {"-li", "--log-input"}, true) != -1) {
            if (a + 1 >= argc) {
                util::ReportError("You need to specify a filename when using -li or --log-input.");
                return 0;
            }

            input_log_file = argv[a + 1];
            ymt::SetAttribute(LOG_INPUT_FILENAME, input_log_file);

            a += 1;
        } else if (util::MatchWithWords(argv[a], {"-p", "--port"}, true) != -1) {

            if (a + 1 >= argc) {
                util::ReportError("You need to specify a port when using -p");
                return 0;
            }

            port = argv[a + 1];
            ymt::SetAttribute(PORT, port);

            a += 1;
        } else if (util::MatchWithWords(argv[a], {"-pn", "--profile-name"}, true) != -1) {

            if (a + 1 >= argc) {
                util::ReportError("You need to specify a profile name when using -pn");
                return 0;
            }

            ymt::SetAttribute(PROFILE_NAME, argv[a + 1]);

            a += 1;
		} else if (util::MatchWithWords(argv[a], { "-s", "--server-address" }, true) != -1) {

			if (a + 1 >= argc) {
				util::ReportError("You need to specify a server address when using -s");
				return 0;
			}

			ymt::SetAttribute(SERVER_ADDR, argv[a + 1]);

			a += 1;
		} else if (util::MatchWithWords(argv[a], { "-q", "--quiet" }, true) != -1) {
			
			util::SetVisualizing(false);
			is_quiet_mode = true;

		} else if (util::MatchWithWords(argv[a], { "-d","--dev"}, true) != -1) {
			
			util::SetVisualizing(false);
			is_developer_mode = true;

		} else {
            util::ReportError("Unknown element: " + string(argv[a]) + ".");
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

    //Set Input File Name
    ymt::SetFileName(input_file);

    //Start Execution
    switch (action) {
        case EXPORT_RAW_LOG :

            if (input_file.empty()) {
                vector<string> file_list = util::GetFileList("./");

                //TODO: FIX THIS
                for (string &file : file_list) {
                    if (file.find(".pidmap") != -1) {
                        input_file = util::SubString(file, 0, file.size() - 7);
                        break;
                    }
                }

                if (input_file.empty()) {
                    util::ReportError("Need to specify input file.");
                    exit(0);
                }

                ymt::SetFileName(input_file);
            }
            util::PrintLines(ymt::GetPortLog());
            break;

        case LOAD :
            if (input_file.empty()) {
                util::ReportError("Need to specify input file with -i or --input.");
                return 0;
            }
            ymt::RunConfig();
            break;

        case UNLOAD :
            if (input_file.empty()) {
                util::ReportError("Need to specify input file with -i or --input.");
                return 0;
            }
            ymt::StopConfig();
            break;
        case EXPORT_LOG :
			out_temp = ymt::GetFormattedStringData();
			if (out_file.empty()) {
				util::PrintLines(out_temp);
			}
			else {
				util::WriteFile(out_file, out_temp);
				util::Print("Log saved to \"" + out_file + "\".\n");
			}
            break;
        case EXPORT_STAT :
			out_temp = ymt::GetStatistics();
			if (out_file.empty()) {
				util::PrintLines(out_temp);
			}
			else {
				util::WriteFile(out_file, out_temp);
				util::Print("Statistics saved to \"" + out_file + "\".\n");
			}
            break;
        case INFO :
            util::PrintLines(ymt::GetUserInfo());
        default:
            break;

    }

	//Developer Mode Output (It is a hacking-like way)
	if (out_file.empty() && is_developer_mode && !is_quiet_mode) {
		util::SetVisualizing(true);
		util::PrintLines(out_temp);
	}

    return 1;
#pragma endregion

}