
#include "ymt.hpp"
#include "util.hpp"

vector <string> log_buffer_;
string extra_param_;
string config_;
string input_log_;
vector <PIDInfo> pid_table_;
vector <Parser> users_;
string port_;
string server_addr_;
string profile_name_;

void ymt::RunConfig() {

    auto * pids = new vector<PIDInfo>();

    //Unload sessions from the config session if loaded already

    if (util::IsFileExist(config_ + ".pidmap")) StopConfig();

    //Read User Information
    if (users_.size() == 0) UpdateUsers();

    //Start Running processes for users
    for (Parser &p : users_) {
        pids->push_back({p.GetAttribute(REMOTE_PORT), RunUser(p)});
    }

    //Write the pid list to a file
    auto * file = new vector<string>();
    for (auto &p : *pids) {
        file->push_back(p.port + ": " + p.pid);
    }
    util::WriteFile(config_ + ".pidmap",*file);
    //Clean up
    util::RemoveFile("SS.conf");
    for (Parser &p : users_) {
        util::RemoveFile(p.GetAttribute(REMOTE_PORT) + ".pid");
    }
    delete file;
    delete pids;
}

string ymt::RunUser(Parser &p) {

    string pid_buffer;

    vector <string> config = p.GetConfig();
    //Write user config
    util::WriteFile("SS.conf", config);

    util::SysExecute(
            ("ss-server -c SS.conf " + string(p.GetAttribute(VERBOSE) == "true" ? "-v" : "") + " " + extra_param_ +
             " -f " + p.GetAttribute(REMOTE_PORT) + ".pid"), false);

    //Return pid buffer
    return util::ReadFile(p.GetAttribute(REMOTE_PORT) + ".pid")[0];
}

void ymt::StopConfig() {
    vector <string> config;
    YAML yaml;
    int fails = 0;
    int goods = 0;

    //Clean syslog first
    CleanSyslog();

    config = util::ReadFile(config_ + ".pidmap");


    if (config.empty()) {
        util::ReportError(
                "Cannot read the pidmap for the file specified, please load the config before unload and DO NOT delete pidmap.");
        return;
    }

    for (auto &line : config) {
        yaml = util::DecodeYamlLine(line);

        if (goods > 3 || util::IsProcessAlive(yaml.right)) {
            util::SysExecute("kill -15 " + yaml.right, false);
            goods += 1;
        } else {
            fails++;
            if (fails >= 5) break;
        };
    }

    util::RemoveFile(config_ + string(".pidmap"));
}

vector <string> ymt::GetPortLog() {

    auto * target_pid = new string();
    auto * target_port = new string();
    vector <string> r;

    //Obtain Raw Log
    if (log_buffer_.empty()) UpdateLog();

    //Return Raw Log if port/pid is not specified
    if (port_.empty()) return log_buffer_;

    //Obtain PID Table
    if (pid_table_.empty()) UpdatePIDTable();

    //If user input is port
    for (auto &i : pid_table_) {
        if (port_ == i.port) {
            *target_pid = i.pid;
            *target_port = i.port;
            break;
        }
    }

    //If user input is pid
    if (target_port->empty()) {
        for (auto &i : pid_table_) {
            if (port_ == i.pid) {
                *target_pid = i.pid;
                *target_port = i.port;
            }
        }
    }

    if (target_port->empty()) return r;

    for (auto &i : log_buffer_) {
        if (i.find("ss-server[" + *target_pid + "]") != -1) {
            r.push_back(i);
        }
    }

    delete target_pid;
    delete target_port;

    return r;
}

void
ymt::SetExtraParam(string extra_param) {
    extra_param_ = extra_param;
}

void ymt::SetAttribute(YMTAttributes attribute, string value) {

    //Set Attributes
    switch (attribute) {
        case CONFIG_FILENAME:
            config_ = value;
            break;
        case LOG_INPUT_FILENAME:
            input_log_ = value;
            break;
        case EXTRA_PARAM:
            extra_param_ = value;
            break;
        case PORT:
            port_ = value;
            break;
        case SERVER_ADDR:
            server_addr_ = value;
            break;
        case PROFILE_NAME:
            profile_name_ = value;
            break;
    }
}

/**
 * Obtain PID Table containing pids matching to the ports defined in user config
 * @return PID Table with an PIDInfo vector array
 */
vector <PIDInfo> ymt::GetPIDTable() {

    vector <PIDInfo> r;

    vector <string> file_buffer = util::ReadFile(config_ + ".pidmap");
    YAML yaml_buffer;

    for (string &line : file_buffer) {
        yaml_buffer = util::DecodeYamlLine(line);

        r.push_back({yaml_buffer.left, yaml_buffer.right});
    }

    return r;
}

/**
 * An internal method for updating PIDTable. U Don't really need to change anything.
 */

void ymt::UpdatePIDTable() {
    pid_table_ = GetPIDTable();
}


/**
 * Obtain Formatted Data (With Time, Port, PID and Destination Information
 * @return
 */
vector <SSLog> ymt::GetFormattedData() {

    vector <SSLog> log_buffer;
    string last_pid, last_port;
    bool is_pid_matched = false;
    const bool is_port_specified = (port_.size() != 0);

    //Get PID table (if and only if it is the first time across the program)
    if (pid_table_.empty()) { UpdatePIDTable(); }

    //Get Log (if and only if it is the first time across the program)
    if (log_buffer_.empty()) { UpdateLog(); }

    printf("Formatting Data...\n");

    for (int i = 0; i < log_buffer_.size(); ++i) {

        if (i + 1 == log_buffer_.size() || i % 100 == 0) {
            util::PercentageBar(i + 1, log_buffer_.size());
        }

        string temp_pid = util::SubString(log_buffer_[i], log_buffer_[i].find_first_of("ss-server[") + 10,
                                          log_buffer_[i].find_first_of(']'));

        if (last_pid.empty() || temp_pid != last_pid) {
            for (PIDInfo &n : pid_table_) {
                if (n.pid == temp_pid) {

                    is_pid_matched = true;

                    last_port = n.port;
                    last_pid = n.pid;
                    break;
                }
            }
        } else { is_pid_matched = true; }

        if (!is_pid_matched) continue;

        size_t context1_location = log_buffer_[i].find("connect to ");


        vector <size_t> s_temp = util::SearchString(log_buffer_[i], ' ', 0, log_buffer_[i].find("ss-server"));
        size_t context2_location = s_temp[s_temp.size() - 2];

        if (context1_location != -1) {
            size_t addr_location = context1_location + 11;

            string time = util::SubString(log_buffer_[i], 0, context2_location);
            string destination = util::SubString(log_buffer_[i], addr_location, log_buffer_.size());

            //Push to log buffer

            //Exclude Lines that are not for the port specified (well only if the port is specified)
            if (is_port_specified && last_port != port_) {
                is_pid_matched = false; //Reset Stat
                continue;
            }

            log_buffer.push_back({time, last_port, last_pid, destination, CONNECT});
        }
        is_pid_matched = false;
    }

    return log_buffer;
}

vector <string> ymt::GetFormattedStringData() {
    vector <string> r;

    r.push_back("Time\tPort\tPID\tDestination");

    for (SSLog &i : GetFormattedData()) {
        r.push_back(i.time + "\t" + i.port + "\t" + i.pid + "\t" + i.destination);
    }
    return r;
}

vector <string> ymt::GetStatistics() {

    vector <SSLog> log = GetFormattedData();
    vector <InquiryData> site_list;
    vector <InquiryData> port_list;
    vector<long long> temp;
    vector <string> r;
    vector <size_t> data_seq;

    size_t LOG_SIZE = log.size();

    //Get website frequency data

    if (LOG_SIZE == 0) {
        printf("No enough data for this port.\n");
        return vector<string>();
    }

    printf("Analyzing website data...\n");

    for (int i = 0; i < LOG_SIZE; ++i) {

        if (i + 1 == LOG_SIZE || i % 100 == 0) {
            util::PercentageBar(i + 1, LOG_SIZE);
        }

        bool is_website_matched = false;
        for (auto &n : site_list) {
            if (log[i].destination == n.key) {
                n.value += 1;
                is_website_matched = true;
            }
        }
        if (is_website_matched) continue;
        else {
            site_list.push_back({log[i].destination, 1});
        }
    }

    for (auto &i : site_list) {
        temp.push_back((long long) i.value);
    }

    data_seq = util::QuickSort::Sort(temp);

    //Push web frequency data
    r.push_back("Website\tInquiry Count");

    for (int i = data_seq.size() - 1; i >= 0; i--) {
        r.push_back(site_list[data_seq[i]].key + string("\t") + to_string((int) (site_list[data_seq[i]].value)));
    }

    r.push_back("");

    //Get port frequency data
    printf("Analyzing Port data...\n");

    for (int i = 0; i < LOG_SIZE; ++i) {

        if (i + 1 == LOG_SIZE || i % 100 == 0) {
            util::PercentageBar(i + 1, LOG_SIZE);
        }

        bool is_port_matched = false;
        for (auto &n : port_list) {
            if (log[i].port == n.key) {
                n.value += 1;
                is_port_matched = true;
            }
        }
        if (is_port_matched) continue;
        else {
            port_list.push_back({log[i].port, 1});
        }
    }

    //Sort Port Frequency Data

    temp.clear();
    data_seq.clear();

    for (InquiryData &i : port_list) {
        temp.push_back(i.value);
    }

    data_seq = util::QuickSort::Sort(temp);

    //Push port frequency data
    r.push_back("User Port\tInquiry Count");
    r.push_back("");

    for (int i = data_seq.size() - 1; i >= 0; --i) {
        r.push_back(port_list[data_seq[i]].key + string("\t") + to_string((int) (port_list[data_seq[i]].value)));
    }

    return r;
}

void ymt::SetFileName(string filename) {
    config_ = filename;
}

void ymt::UpdateLog() {

    vector <string> temp = util::ReadFile((input_log_.empty() ? "/var/log/syslog" : input_log_));

    for (string &i : temp) {
        if (i.find("ss-server[") != -1) {
            log_buffer_.push_back(i);
        }
    }

}

void ymt::CleanSyslog() {
    util::SysExecute("rm /var/log/syslog", false);
    util::SysExecute("touch /var/log/syslog", false);
    util::SysExecute("chmod 755 /var/log/syslog", false);
    util::SysExecute("service rsyslog restart", false);
}

vector <string> ymt::GetUserInfo() {

    if (pid_table_.empty()) UpdatePIDTable();
    if (users_.empty()) UpdateUsers();

    Parser target_user;
    string target_pid;
    string target_port;
    bool is_user_located = false;
    vector <string> r;

    for (auto &i : users_) {
        if (port_ == i.GetAttribute(REMOTE_PORT)) {
            target_user = i;
            target_port = i.GetAttribute(REMOTE_PORT);
            is_user_located = true;
            break;
        }
    }

    //If user input is port
    for (auto &i : pid_table_) {
        if (port_ == i.port) {
            target_pid = i.pid;
            target_port = i.port;
            break;
        }
    }

    //If user input is pid
    if (target_port.empty()) {
        for (auto &i : pid_table_) {
            if (port_ == i.pid) {
                target_pid = i.pid;

                if (target_port != i.port) {
                    is_user_located = false;
                }

                target_port = i.port;
            }
        }
    }

    if (!is_user_located) {
        for (auto &i : users_) {
            if (target_port == i.GetAttribute(REMOTE_PORT)) {
                target_user = i;
                is_user_located = true;
                break;
            }
        }
    }

    //No found? Give up
    if (!is_user_located) {
        return r;
    }

    //Now Push User Information In

    r = util::Make2DTable({
                                  {"Remote Port",        target_user.GetAttribute(REMOTE_PORT)},
                                  {"Local  Port",        target_user.GetAttribute(LOCAL_PORT)},
                                  {"PID",                (target_pid.empty() ? "(NOT RUNNING)" : target_pid)},
                                  {"Method(Encryption)", target_user.GetAttribute(METHOD)},
                                  {"Password",           target_user.GetAttribute(KEY)},
                                  {"TCP Fast Open",      target_user.GetAttribute(TCP_FASTOPEN)},
                                  {"Tunnel Mode",        target_user.GetAttribute(UDP_OR_TCP)},
                                  {"Timeout",            target_user.GetAttribute(TIMEOUT)},
                                  {"Verbose",            target_user.GetAttribute(VERBOSE)},
                                  {"SS Link",            GetSSShareLink(target_user)}
                          });

    return r;
}

void ymt::UpdateUsers() {

    //Make template configuration (Can be modified by config)
    Parser default_config;

    //Read raw config
    vector <string> yaml_content = util::ReadFile(config_);

    //Parse YAML
    for (string &line : yaml_content) {

        //Skip blanklines & comments
        if (line.empty()) continue;
        else {
            int slash_pos = line.find("//");
            if (slash_pos != -1) {
                line = util::SubString(line, 0, slash_pos);
            }
            if (line.empty()) continue;
        }


        YAML l = util::DecodeYamlLine(line);

        //If it is global level -- no user
        if (l.level == 0) {
            switch (util::MatchWithWords(l.left,
                                         {"group", "nameserver", "method", "fastopen", "redirect", "timeout", "server",
                                          "tunnel_mode", "verbose"})) {
                case 0:
                    default_config.SetAttribute(GROUP_NAME, l.right);
                    break;
                case 1:
                    if (util::IsTheSame("localhost", l.right, false, false) ||
                        util::IsTheSame("bind9-local", l.right, false, false)) {
                        default_config.SetAttribute(DNS, util::GetMachineIP());
                    } else {
                        default_config.SetAttribute(DNS, l.right);
                    }
                    break;
                case 2:
                    default_config.SetAttribute(METHOD, l.right);
                    break;
                case 3:
                    default_config.SetAttribute(TCP_FASTOPEN, l.right);
                    break;
                case 4:
                    default_config.SetAttribute(REDIRECT, l.right);
                    break;
                case 5:
                    default_config.SetAttribute(TIMEOUT, l.right);
                    break;
                case 6:
                    default_config.SetAttribute(SERVER, l.right);
                    break;
                case 7:
                    default_config.SetAttribute(UDP_OR_TCP, l.right);
                    break;
                case 8:
                    default_config.SetAttribute(VERBOSE, l.right);
                    break;

                default:
                    break;
            }
        } else {
            Parser user = default_config;
            user.SetUser(l.left, l.right);
            users_.push_back(user);
        }
    }
}

string ymt::GetSSShareLink(Parser &user) {

    if (server_addr_.empty()) {
        server_addr_ = util::GetMachineIP();
    }

    if (profile_name_.empty()) {
        profile_name_ = user.GetAttribute(GROUP_NAME) + '-' + user.GetAttribute(REMOTE_PORT);
    }

    return "ss://" + util::GetEncodedBase64(user.GetAttribute(METHOD)
                                            + ':'
                                            + user.GetAttribute(KEY)
                                            + '@'
                                            + server_addr_
                                            + ':'
                                            + user.GetAttribute(REMOTE_PORT)) + "#" + profile_name_;
}
