
#include "ssm.hpp"

#define DEBUG_MODE 0


inline bool ssm::ReadFile(string filename, vector<string> &file_buffer) {

    ifstream reader;
    string read_buffer;

    reader.open(filename);

    if (!reader.is_open()) {
        util::ReportError("Cannot open file. Please check filename/file access");
        return false;
    }

    //Read file
    while (getline(reader, read_buffer)) { file_buffer.push_back(read_buffer); }

    reader.close(); //Close file session
    return true;
}

string ssm::MakeUserConfig(string method, string port, string password, string nameserver, string redirect) {

    return "{\n"
           "\t\"server\" : \"0.0.0.0\",\n"
           "\t\"server_port\" : " + port + ",\n"
           "\t\"local_port\": 1080,\n"
           "\t\"password\" : \"" + password + "\",\n"
           "\t\"timeout\" : 3600,\n"
           "\t\"method\" : \"" + method + "\",\n"
           "\t\"fast_open\" : true,\n"
           "\t\"nameserver\" : \"" +
           nameserver + "\",\n"
           "\t\"redirect\" : \"" + redirect + "\"\n}";




    /*
    string return_buffer;
    return_buffer += "{\n"
                     "\t\"server\" : \"0.0.0.0\",\n"
                     "\t\"server_port\" : ";
    return_buffer += port;

    return_buffer += ",\n"

    return_buffer += password;

    return_buffer += "\",\n"
                     "\t\"timeout\" : 3600,\n"
                     "\t\"method\" : \"";
    return_buffer += method;

    return_buffer += "\",\n"
                     "\t\"fast_open\" : true,\n"
                     "\t\"nameserver\" : \"";
    return_buffer += nameserver;

    return_buffer += "\",\n"
                     "\t\"redirect\" : \"";
    return_buffer += redirect + "\"\n}";

    return return_buffer;
    */
}

void ssm::RunConfig(string filename) {

    vector<string> file_buffer;
    Json json_read_buffer;
    vector<User> temp_user_list;
    string temp_encryption = "chacha20-ietf";
    string temp_nameserver = "8.8.8.8";
    string temp_redirect = "bing.com";
    string temp_groupname = "DEFAULT GROUP";
    vector<string> temp_pid_list;
    vector<string> port_list;


    bool isInUserList = 0;

    //Unload sessions from the config session if loaded already
    if (util::IsFileExist(filename + ".pidmap")) StopConfig(filename);

    //Read file, quit if failed
    if (!ReadFile(filename, file_buffer)) { return; }

    for (string current_file_line : file_buffer) {

        util::RemoveLetter(current_file_line, ' ');
        //Skip blank lines and commented lines
        if (current_file_line.find("//") == 0 || current_file_line.size() < 1) continue;

        if (current_file_line.find('}') != string::npos) {
            isInUserList = false;
            if (temp_user_list.size() != 0) {
                RunUsers(temp_user_list, temp_encryption, temp_nameserver, temp_redirect, temp_pid_list);
            }

            for (User current_user : temp_user_list) { port_list.push_back(current_user.port); }

            temp_user_list.clear();
            continue;
        }

        json_read_buffer = util::GetJson(current_file_line);

        if (isInUserList) {
            if (json_read_buffer.element == "nameserver") { temp_nameserver = json_read_buffer.key; }
            else if (json_read_buffer.element == "redirect") { temp_redirect = json_read_buffer.key; }
            else { temp_user_list.push_back({json_read_buffer.element, json_read_buffer.key}); }
            continue;
        } else {
            temp_groupname = json_read_buffer.element;

            util::RemoveLetter(json_read_buffer.key, '{');
            util::RemoveLetter(json_read_buffer.key, ':');

            temp_encryption = json_read_buffer.key;
            isInUserList = 1;
            continue;
        }
    }

    //Write the pid list to a file
    WritePidMap(filename + ".pidmap", temp_pid_list, port_list);

}

inline void ssm::WritePidMap(string filename, vector<string> &pid_list, vector<string> &port_list) {
    ofstream writer;
    writer.open(filename.c_str());
    for (int i = 0; i < pid_list.size(); ++i) {
        writer << port_list[i] << " : " << pid_list[i] << "\n";
    }
    writer.close();
}

inline void ssm::RunUsers(vector<User> &user_list, string &encryption, string &nameserver, string &redirect,
                          vector<string> &pid_list_buffer) {

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

        if (DEBUG_MODE) {
            printf("USER: %s, PASS: %s \n", current_user.port.c_str(), current_user.password.c_str());
            printf("%s", current_user_buffer.c_str());
            printf("\n");
        }

        writer.open("PROTECTED_USER.conf");
        writer << current_user_buffer;
        writer.close();

        system(string("ss-server -c PROTECTED_USER.conf -u -f " + current_user.port + ".pid").c_str());
        reader.open(current_user.port + ".pid");
        reader >> pid_buffer;
        reader.close();

        pid_list_buffer.push_back(pid_buffer);
    }

    //Clean up
    for (User temp_user : user_list) {
        util::RemoveFile(temp_user.port + ".pid");
    }
    util::RemoveFile("PROTECTED_USER.conf");
}

void ssm::StopConfig(string filename) {
    vector<string> file_buffer;
    Json json_read_buffer;
    int fail_count = 0;

    if (!ReadFile(filename + ".pidmap", file_buffer)) {
        util::ReportError(
                "Cannot read the pidmap for the file specified, please load the config before unload and DO NOT delete pidmap.");
        return;
    } else {
        for (string current_line : file_buffer) {
            json_read_buffer = util::GetJson(current_line);

            if (DEBUG_MODE)
                printf("PORT: %s  PID: %s\n", json_read_buffer.element.c_str(), json_read_buffer.key.c_str());

            if (util::IsProcessAlive(stoi(json_read_buffer.key))) {
                system((string("kill -15 ") + json_read_buffer.key).c_str());
            } else {
                fail_count++;

                if (fail_count >= 5) break;
            };
        }
    }

    util::RemoveFile(filename + string(".pidmap"));
}

void ssm::CheckPort(string filename, string port) {

    vector<string> file_buffer;
    Json json_read_buffer;
    string target_pid = "-1";



    //Quit if cannot find the pidmap file
    if (!ReadFile(filename + ".pidmap", file_buffer)) {
        util::ReportError(
                "Cannot read the pidmap for the file specified, please load the config before unload and DO NOT delete pidmap.");
        return;
    }

    //Assume the "port" parameter is a port
    for (string current_line : file_buffer) {

        json_read_buffer = util::GetJson(current_line);

        if (json_read_buffer.element == port) {
            target_pid = json_read_buffer.key;
            break;
        }
    }

    if (target_pid == "-1") {

        //Now assume the "port parameter" is actually a PID
        for (string current_line : file_buffer) {

            json_read_buffer = util::GetJson(current_line);

            if (json_read_buffer.key == port) {
                target_pid = json_read_buffer.key;
                port = json_read_buffer.element;
                break;
            }

        }

        //Still no found...
        if (target_pid == "-1") { return; }
    }

    file_buffer = util::SysExecute(R"(journalctl | grep "ss-server\[)" + target_pid + R"(\]")");

    //Some basic info
    printf("ss-manager information\n"
           "=========================\n"
           "Configuration: %s\n"
           "Port:          %s\n"
           "PID:           %s\n\n"
           "Log from system about this user:\n\n",
           filename.c_str(),
           port.c_str(),
           target_pid.c_str());

    //Print actual log
    for (string current_line : file_buffer) {
        printf("%s\n", current_line.c_str());
    }
    printf("\n"
           "=========================\n");
}