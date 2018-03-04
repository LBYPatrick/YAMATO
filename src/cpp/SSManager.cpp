
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

bool SSManager::readFile(string filename, vector<string> & file_buffer) {
    
    ifstream reader;
    string   read_buffer;

    reader.open(filename);

    if(!reader.is_open()){
        Utils::reportError("Cannot open file. Please check filename/file access");
        return false;
    }

    //Read file
    while(getline(reader,read_buffer)) { file_buffer.push_back(read_buffer); }

    reader.close(); //Close file session
    return true;
}

string SSManager::makeUser(string name, string port, string password, string fastopen, string nameserver, string redirect){
    string returnBuffer;
}

void SSManager::ExecuteFile(string filename) {

    std::vector<string>     file_buffer;
    Json                    json_read_buffer;
    std::vector<User>       temp_user_list;
    std::string             temp_encryption;
    std::string             nameserver = "127.0.0.1";
    std::string             fastopen   = "false";
    


    bool isInUserList = 0;

    //Read file, quit if failed
    if(!readFile(filename,file_buffer)) {return;}    

    for(int i = 0; i < file_buffer.size(); ++i) {

        if(file_buffer[i].find("}") != string::npos) {
            isInUserList = false;

            //User read complete, start processing
            for (int n = 0; n < temp_user_list.size(); ++n) {
                
            }
            continue;
        }

        json_read_buffer = Utils::GetJson(file_buffer[i]);


        if (isInUserList) {
            temp_user_list.push_back({json_read_buffer.element,json_read_buffer.key});
            continue;
        }
        else if (json_read_buffer.element == "user_group") {
            temp_encryption = json_read_buffer.key;
            isInUserList = 1;
            continue;
        }
    }
}