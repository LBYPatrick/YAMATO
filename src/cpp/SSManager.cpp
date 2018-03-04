
#include "SSManager.h"
#define DEBUG_MODE 0


bool SSManager::readFile(string filename, vector<string> & file_buffer) {
    
    ifstream reader;
    string   read_buffer;

    reader.open(filename);

    if(!reader.is_open()){
        Utils::ReportError("Cannot open file. Please check filename/file access");
        return false;
    }

    //Read file
    while(getline(reader,read_buffer)) { file_buffer.push_back(read_buffer); }

    reader.close(); //Close file session
    return true;
}

string SSManager::MakeUser(string method,string port, string password, string nameserver, string redirect){
    string returnBuffer;
    returnBuffer += "{\n"
            "\t\"server\" : \"0.0.0.0\",\n"
            "\t\"server_port\" : ";
    returnBuffer += port;

    returnBuffer += ",\n"
            "\t\"local_port\": 1080,\n"
            "\t\"password\" : \"";
    returnBuffer += password;

    returnBuffer += "\",\n"
            "\t\"timeout\" : 3600,\n"
            "\t\"method\" : \"";
    returnBuffer += method;

    returnBuffer += "\",\n"
            "\t\"fast_open\" : true,\n"
            "\t\"nameserver : \"";
    returnBuffer += nameserver;

    returnBuffer += "\",\n"
            "\t\"redirect\" : \"";
    returnBuffer += redirect + "\"\n}";

    return returnBuffer;

}

void SSManager::ExecuteFile(string filename) {

    std::vector<string>     file_buffer;
    Json                    json_read_buffer;
    std::vector<User>       temp_user_list;
    std::string             temp_encryption = "chacha20-ietf";
    std::string             temp_nameserver = "127.0.0.1";
    std::string             temp_redirect   = "bing.com";
    ofstream writer;


    bool isInUserList = 0;

    //Read file, quit if failed
    if(!readFile(filename,file_buffer)) {return;}    

    for(int i = 0; i < file_buffer.size(); ++i) {

        if(file_buffer[i].find("}") != string::npos) {
            isInUserList = false;

            //User read complete, start processing
            for (int n = 0; n < temp_user_list.size(); ++n) {

                string current_user_buffer = MakeUser(temp_encryption,
                                                      temp_user_list[n].port,
                                                      temp_user_list[n].password,
                                                      temp_nameserver,
                                                      temp_redirect);

                if(DEBUG_MODE) {
                    printf("USER: %s, PASS: %s \n", temp_user_list[n].port.c_str(), temp_user_list[n].password.c_str());
                    printf(current_user_buffer.c_str());
                    printf("\n");
                }

                writer.open("PROTECTED_USER.conf");
                writer<<current_user_buffer;
                writer.close();

                if(!DEBUG_MODE) system((std::string("ss-server -c PROTECTED_UESR.conf -f ss-pid/") + temp_user_list[n].port + ".pid").c_str());

            }
            temp_user_list.clear();
            continue;
        }

        json_read_buffer = Utils::GetJson(file_buffer[i]);

        if (isInUserList) {
            temp_user_list.push_back({json_read_buffer.element,json_read_buffer.key});
            continue;
        }
        else if (json_read_buffer.element == "user_group") {

            Utils::RemoveLetter(json_read_buffer.key,'{');
            Utils::RemoveLetter(json_read_buffer.key, ':');

            temp_encryption = json_read_buffer.key;
            isInUserList = 1;
            continue;
        }
    }
}