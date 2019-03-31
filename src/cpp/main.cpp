#include <vector>

#include "includes.hpp"
#include "util.hpp"
#include "ymt.hpp"

enum Action {
    RAW_LOG,
    LOAD,
    UNLOAD,
    READABLE_LOG,
    STATISTICS,
	BACKUP_LOG,
    UNKNOWN,
    INFO
};

void ShowUsage() {
	cout << ("YAMATO " SOFTWARE_VERSION " by LBYPatrick\n");
	util::ShowHelp({
						   {"-i or --input <filename>",        "Specify input file"},
						   {"-a or --action <action>",         "Specify action (raw_log,stat, load, unload,log,info,backup_log)"},
						   {"-pn or --profile-name",           "Specify profile name(For creating sharable SS:// link)"},
						   {"-s or --server-address",          "Specify server address(For creating sharable SS:// link)"},
						   {"/?, -h or --help",                "Show this help message"},
						   {"-e or --extra-parameter <param>", "Specify additional parameters, you can do things like UDP relay or HTTP/TLS OBFS here"},
						   {"-p or --port",                    "Specify a port for checking status"},
						   {"-li or --log-input",              "Specify source syslog file (Not required, this is for analyzing log in devices other than your server)"},
						   {"-o or --output",                  "Specify output file name (For stat and log specified with --action)"},
						   {"-q or --quiet",				   "Enable Quiet Mode (No output, even including error notifications)"},
						   {"-d or --dump",					   "Dump log when stopping a configuration"}
		});
	cout << ("\n");
}

int main(int argc, char *const argv[]) {

	std::ios_base::sync_with_stdio(false);

    //Variables
    string input_file;
	string out_file;
    string input_log_file;
	vector<string> out_temp;
    Action action = UNKNOWN;
	YFile file_buffer;
    string port;
	bool is_developer_mode = false;
	bool is_quiet_mode = false;

	if (argc == 1) {
		ShowUsage();
		return 1;
	}

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
                action = STATISTICS;
            } else if (util::MatchWithWords(argv[a + 1], {"load"}, true) != -1) {
                action = LOAD;
            } else if (util::MatchWithWords(argv[a + 1], {"unload"}, true) != -1) {
                action = UNLOAD;
            } else if (util::MatchWithWords(argv[a + 1], {"log"}, true) != -1) {
                action = READABLE_LOG;
            } else if (util::MatchWithWords(argv[a + 1], {"raw_log"}, true) != -1) {
                action = RAW_LOG;
            } else if (util::MatchWithWords(argv[a + 1], {"info", "user-information"}, true) != -1) {
                action = INFO;
			} else if (util::MatchWithWords(argv[a + 1], {"backup_log"},true) != -1) {
				action = BACKUP_LOG;
			}
			else {
                util::ReportError("Unknown action: " + string(argv[a + 1]) + ".");
                break;
            }
            a += 1;
        } else if (util::MatchWithWords(argv[a], {"/?", "-h", "--help"}, true) != -1) {
			ShowUsage();
            return 1;
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
		}
		else if (util::MatchWithWords(argv[a], { "-q", "--quiet" }, true) != -1) {

			util::SetVisualizing(false);
			is_quiet_mode = true;
		}
		else if (util::MatchWithWords(argv[a], { "-d" ,"--dump" }, true) != -1) {

			ymt::SetAttribute(NEED_DUMP_BEFORE_LOAD, "true");

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
        case RAW_LOG :

			if (ymt::GetPortLog(file_buffer)) {
				if (out_file.empty()) { 
					util::PrintFile(file_buffer); 
				}
				else {
					util::DirectWriteFile(file_buffer, out_file);
				}
			}
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
        case READABLE_LOG :
		case BACKUP_LOG :	

			if (out_file.empty()) {
				ymt::PrintFormattedData(action == READABLE_LOG ? 1 : 0);
			}
			else {
				if (ymt::PrintFormattedData(action == READABLE_LOG ? 1 : 0, out_file)) {
					util::Print("Log saved to \"" + out_file + "\".\n");
				}
				else {
					util::Print("Failed to save to \"" + out_file + "\". Please check file access and try again\n");
				}
			}
            break;
        case STATISTICS :
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
			out_temp = ymt::GetUserInfo();
            util::PrintLines(out_temp);
			break;
        default:
            break;

    }

    return 1;
#pragma endregion

}