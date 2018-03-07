
#include "SSManager.h"
#define DEBUG_MODE 0


inline bool SSManager::ReadFile(string filename, vector<string> & file_buffer) {
    
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

    std::vector<string>         file_buffer;
    Json                        json_read_buffer;
    std::vector<User>           temp_user_list;
    std::string                 temp_encryption = "chacha20-ietf";
    std::string                 temp_nameserver = "8.8.8.8";
    std::string                 temp_redirect   = "bing.com";
    std::string                 temp_groupname  = "DEFAULT GROUP";
    std::vector<std::string>    temp_pid_list;
    std::vector<std::string>    port_list;


    bool isInUserList = 0;

    //Unload sessions from the config session if loaded already
    if(Utils::IsFileExist(filename + ".pidmap")) StopConfig(filename);

    //Read file, quit if failed
    if(!ReadFile(filename,file_buffer)) {return;}

    for(string current_file_line : file_buffer) {

        //Skip blank lines and commented lines
        if(current_file_line[0] == '#' || current_file_line.size() < 1) continue;

        if(current_file_line.find('}') != string::npos) {
            isInUserList = false;
            if(temp_user_list.size() != 0) {RunUsers(temp_user_list,temp_encryption,temp_nameserver,temp_redirect,temp_pid_list);}

            for(User current_user : temp_user_list) { port_list.push_back(current_user.port); }

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

    //Write the pid list to a file
    WritePidMap(filename + ".pidmap",temp_pid_list,port_list);

}

inline void SSManager::WritePidMap(string filename, std::vector<string> & pid_list,std::vector<string> & port_list) {
    ofstream writer;
    writer.open(filename.c_str());
    for(int i = 0; i < pid_list.size(); ++i) {
        writer<<port_list[i]<<" : "<<pid_list[i]<<"\n";
    }
    writer.close();
}

inline void SSManager::RunUsers(std::vector<User> & user_list, string & encryption, string & nameserver, string & redirect, std::vector<std::string> & pid_list_buffer) {

    ofstream writer;
    ifstream reader;
    string pid_buffer;
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

        system(std::string("ss-server -c PROTECTED_USER.conf -f " + current_user.port + ".pid").c_str());
        reader.open(current_user.port + ".pid");
        reader>>pid_buffer;
        reader.close();

        pid_list_buffer.push_back(pid_buffer);
    }

    //Clean up
    for(User temp_user : user_list) {
        Utils::RemoveFile(temp_user.port + ".pid");
    }
    Utils::RemoveFile("PROTECTED_USER.conf");
}

void SSManager::StopConfig(string filename) {
    std::vector<std::string> file_buffer;
    Json                     json_read_buffer;

    if(!ReadFile(filename + ".pidmap", file_buffer)) {
        Utils::ReportError("Cannot read the pidmap for the file specified, please load the config before unload and DO NOT delete pidmap.");
        return;
    }
    else {
        for(std::string current_line : file_buffer) {
            json_read_buffer = Utils::GetJson(current_line);
            
            if(DEBUG_MODE) printf("PORT: %s  PID: %s\n", json_read_buffer.element.c_str(), json_read_buffer.key.c_str());
            
            system((std::string("kill -15 ") + json_read_buffer.key).c_str());
        }
    }

    Utils::RemoveFile(filename + std::string(".pidmap"));
}

void SSManager::CheckPort(string filename, string port) {

    std::vector<std::string> file_buffer;
    Json                     json_read_buffer;
    std::string              target_pid = "-1";

    if(!ReadFile(filename + ".pidmap", file_buffer)) {
        Utils::ReportError("Cannot read the pidmap for the file specified, please load the config before unload and DO NOT delete pidmap.");
        return;
    }
    else {
        for(std::string current_line : file_buffer) {
            json_read_buffer = Utils::GetJson(current_line);
            if(json_read_buffer.element == port) {
                target_pid = json_read_buffer.key;
            }
        }

        if(target_pid == "-1") Utils::ReportError("Port " + port + " no found in pidmap for " + filename);
        else {
            file_buffer = Utils::SysExecute(R"(journalctl | grep "ss-server\[)" + target_pid + R"(\]")");

            for(std::string current_line : file_buffer) {
                printf("%s\n",current_line.c_str());
            }
        }
    }
}