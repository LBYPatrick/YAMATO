
#include "SSManager.h"

const vector<string> optionList = {
        "timeout",
        "fastopen",
        "method",
        "nameserver",
        "local_port"

};

struct User {
    string port;
    string password;
};

void SSManager::GenerateCommand(string filename, vector<string> & output_buffer) {

    ifstream reader;
    string read_buffer;
    vector<string> file_buffer;
    vector<User>   userlist;
    bool is_in_userlist = false;

    /**
     * Read
     */
    reader.open(filename.c_str());
    while (getline(reader, read_buffer)) file_buffer.push_back(read_buffer);
    reader.close();

    const int lineCount = file_buffer.size();

    /**
     * Process line-by-line
     */
    for (int i = 0; i < lineCount; ++i) {
        string json_string;
        string json_value;
        /**
         * Get string and value for the current line
         */

        Utils::RemoveLetter(file_buffer[i], ' ');
        Utils::RemoveLetter(file_buffer[i], '\"');

        if(file_buffer[i][0] == '}' && is_in_userlist) {
            is_in_userlist = false;
            continue;
        }

        for (int n = 0; n < file_buffer[i].find_first_of(":"); ++n) {
            json_string += file_buffer[i][n];
        }

        for (int n = file_buffer[i].find_first_of(":") + 1;
             n < file_buffer[i].size();
             ++n) {
            json_value += file_buffer[i][n];
        }

        if(json_string == "port_password") {
            is_in_userlist = true;
            continue;
        }
        else if(is_in_userlist) {
            userlist.push_back({json_string,json_value});
        }
        else {
            const int endPosition = optionList.size() - 1;
            for(int i = 0; i <= endPosition; ++i) {
                if(json_string == optionList[i]) {
                }
            }
        }
    }
}