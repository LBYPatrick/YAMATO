
#include "SSManager.h"
#define DEBUG_MODE 0


bool SSManager::ReadFile(string filename, vector<string> & file_buffer) {
    
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

string SSManager::MakeUserConfig(string method,string port, string password, string nameserver, string redirect){
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
            "\t\"nameserver\" : \"";
    returnBuffer += nameserver;

    returnBuffer += "\",\n"
            "\t\"redirect\" : \"";
    returnBuffer += redirect + "\"\n}";

    return returnBuffer;

}

void SSManager::RunConfig(string filename) {

    std::vector<string>     file_buffer;
    Json                    json_read_buffer;
    std::vector<User>       temp_user_list;
    std::string             temp_encryption = "chacha20-ietf";
    std::string             temp_nameserver = "127.0.0.1";
    std::string             temp_redirect   = "bing.com";
    std::string             temp_groupname  = "DEFAULT GROUP";


    bool isInUserList = 0;

    //Read file, quit if failed
    if(!ReadFile(filename,file_buffer)) {return;}

    for(string current_file_line : file_buffer) {

        if(current_file_line[0] == '#') continue;

        if(current_file_line.find("}") != string::npos) {
            isInUserList = false;
            if(temp_user_list.size() != 0) {RunUsers(temp_user_list,temp_encryption,temp_nameserver,temp_redirect);}
            temp_user_list.clear();
            continue;
        }

        json_read_buffer = Utils::GetJson(current_file_line);

        if (isInUserList) {
            if(json_read_buffer.element == "nameserver")    {temp_nameserver = json_read_buffer.key; }
            else if(json_read_buffer.element == "redirect") {temp_redirect = json_read_buffer.key; }
            else                                            { temp_user_list.push_back({json_read_buffer.element, json_read_buffer.key}); }
            continue;
        }
        else {
            temp_groupname = json_read_buffer.element;

            Utils::RemoveLetter(json_read_buffer.key,'{');
            Utils::RemoveLetter(json_read_buffer.key, ':');

            temp_encryption = json_read_buffer.key;
            isInUserList = 1;
            continue;
        }
    }
}

void SSManager::RunUsers(std::vector<User> & user_list, string & encryption, string & nameserver, string & redirect) {

    ofstream writer;

    //User read complete, start processing
    for (User current_user : user_list) {

        string current_user_buffer = MakeUserConfig(encryption,
                                                    current_user.port,
                                                    current_user.password,
                                                    nameserver,
                                                    redirect);

        if(DEBUG_MODE) {
            printf("USER: %s, PASS: %s \n", current_user.port.c_str(), current_user.password.c_str());
            printf("%s",current_user_buffer.c_str());
            printf("\n");
        }

        writer.open("PROTECTED_USER.conf");
        writer<<current_user_buffer;
        writer.close();

        system((std::string("ss-server -c PROTECTED_USER.conf -f ss-pid/") + current_user.port + ".pid").c_str());
    }

    //Clean up
    Utils::RemoveFile("PROTECTED_USER.conf");
}