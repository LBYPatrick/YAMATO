
#include "ymt.hpp"
#include "util.hpp"

string extra_param_;
string config_;
string input_log_;
vector <PIDInfo> pid_table_;
vector <Parser> users_;
YFile log_file;
string port_;
string server_addr_;
string profile_name_;
bool is_dump_needed_ = false;
ofstream format_data_writer;

void ymt::RunConfig() {

	vector<PIDInfo> pids;

	//Unload sessions from the config session if loaded already

	if (util::IsFileExist(config_ + ".pidmap")) StopConfig();

	//Read User Information
	if (users_.size() == 0) UpdateUsers();

	pids.reserve(users_.size());

	//Start Running processes for users
	for (Parser& p : users_) {
		pids.push_back({ p.GetAttribute(REMOTE_PORT), RunUser(p) });
	}

	//Write the pid list to a file
	vector<string> file;

	file.reserve(pids.size());

	for (auto& p : pids) {
		file.push_back(p.port + ": " + p.pid);
	}

	util::WriteFile(config_ + ".pidmap", file);
	//Clean up
	util::FlushDeleteQueue();
}

/**
	A function that starts a ss-server with a user configuration.
	@param A Parser object (Containing user information)
	@return the PID of the user process

*/

string ymt::RunUser(Parser & p) {

	string pid_buffer;

	string obfs_filename;
	string conf_path;
	string pid_path;

	util::GetRandomString(obfs_filename, 5);

	vector <string> config = p.GetConfig();

	conf_path = "/dev/shm/" + obfs_filename + ".conf";
	pid_path = "/dev/shm/" + obfs_filename + ".pid";

	//Write user config
	util::WriteFile(conf_path, config);

	util::SysExecute(
		("ss-server -c " + conf_path + " " + string(p.GetAttribute(VERBOSE) == "true" ? "-v" : "") + " " + extra_param_ +
		 " -f " + pid_path), false);

	util::PushToDeleteQueue(conf_path);
	util::PushToDeleteQueue(pid_path);

	//Return pid buffer
	return util::ReadFile(pid_path)[0];
}

void ymt::StopConfig() {
	vector<string> config;
	YAML yaml;
	int fails = 0;
	int goods = 0;

	//dump log when needed
	if (is_dump_needed_) {

		string machine_name;
		string dump_filename;
		string str_temp;
		//Still, C++ sucks compared to higher-level languages
		auto temp_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
		auto date = localtime(&temp_time);

		if (server_addr_.empty()) {
			machine_name = util::SysExecute("hostname")[0];
		}
		else {
			machine_name = server_addr_;
		}

		//Parse dump filename
		dump_filename = machine_name + "_";

		//Month
		str_temp = to_string(date->tm_mon + 1);
		dump_filename += (str_temp.length() == 1 ? "0" : "") + str_temp;

		//Day
		str_temp = to_string(date->tm_mday);
		dump_filename += (str_temp.length() == 1 ? "0" : "") + str_temp;

		//Year
		str_temp = to_string(date->tm_year + 1900);
		dump_filename += (str_temp.length() == 1 ? "0" : "") + str_temp;

		//Suffix
		dump_filename += ".log";

		//At this point the filename should be MACHINE_MMDDYYYY.log

		PrintFormattedData(false, dump_filename);
	}

	//Stopping instances
	//Clean syslog first
	CleanSyslog();

	//Kill processes
	config = util::ReadFile(config_ + ".pidmap");

	if (config.empty()) {
		util::ReportError(
			"Cannot read the pidmap for the file specified, please load the config before unload and DO NOT delete pidmap.");
		return;
	}

	for (auto& line : config) {
		yaml = util::DecodeYamlLine(line);

		if (goods > 3 || util::IsProcessAlive(yaml.right)) {
			util::SysExecute("kill -15 " + yaml.right, false);
			goods += 1;
		}
		else {
			fails++;
			if (fails >= 5) break;
		};
	}

	util::RemoveFile(config_ + string(".pidmap"));
}

bool ymt::GetPortLog(YFile & buffer) {

	string target_pid;
	string target_port;

	if (!UpdateLog()) return false;

	//Make a copy of the object
	buffer = YFile(log_file);

	//Return Raw Log if port/pid is not specified
	if (port_.empty()) return true;

	//Obtain PID Table
	if (pid_table_.empty()) UpdatePIDTable();

	//If user input is port
	for (auto& i : pid_table_) {
		if (port_ == i.port) {
			target_pid = i.pid;
			target_port = i.port;
			break;
		}
	}

	//If user input is pid
	if (target_port.empty()) {
		for (auto& i : pid_table_) {
			if (port_ == i.pid) {
				target_pid = i.pid;
				target_port = i.port;
			}
		}
	}

	if (!target_port.empty()) {
		buffer.filter = FileFilter("ss-server[" + target_pid + "]");
	};

	return true;
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
	case NEED_DUMP_BEFORE_LOAD:
		is_dump_needed_ = (value == "true");
		break;
	}
}

/**
 * Obtain PID Table containing pids matching to the ports defined in user config
 * @return PID Table with an PIDInfo vector array
 */
vector<PIDInfo> ymt::GetPIDTable() {

	vector <PIDInfo> r;

	vector <string> file_buffer = util::ReadFile(config_ + ".pidmap");
	YAML yaml_buffer;

	for (string& line : file_buffer) {
		yaml_buffer = util::DecodeYamlLine(line);

		r.push_back({ yaml_buffer.left, yaml_buffer.right });
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
vector<SSLog> ymt::GetFormattedData() {

	vector <SSLog> r;
	string last_pid, last_port;
	string read_buffer;
	size_t log_count = 0;
	bool is_pid_matched = false;
	const bool is_port_specified = (port_.size() != 0);
	ifstream reader;

	//Get PID table (if and only if it is the first time across the program)
	if (pid_table_.empty()) { UpdatePIDTable(); }

	if (!UpdateLog()) { return r; }

	util::ReportEvent("Start parsing log.", false);

	//Force output
	cout.flush();

	reader = ifstream(log_file.filename);

	while (util::GetNextValidLine(reader, read_buffer, log_file.filter)) {

		++log_count;

		string temp_pid = util::SubString(read_buffer, read_buffer.find("ss-server[") + 10,
										  read_buffer.find(']'));

		if (last_pid.empty() || temp_pid != last_pid) {
			for (PIDInfo& n : pid_table_) {
				if (n.pid == temp_pid) {

					is_pid_matched = true;

					last_port = n.port;
					last_pid = n.pid;
					break;
				}
			}
		}
		else { is_pid_matched = true; }

		if (!is_pid_matched) continue;

		size_t context1_location = read_buffer.find("connect to ");

		vector <size_t> s_temp = util::SearchString(read_buffer, ' ', 0, read_buffer.find("ss-server"));
		size_t context2_location = s_temp[s_temp.size() - 2];

		if (context1_location != -1) {
			size_t addr_location = context1_location + 11;

			string time = util::SubString(read_buffer, 0, context2_location);
			string destination = util::SubString(read_buffer, addr_location, read_buffer.size());

			//Exclude Lines that are not for the port specified (well only if the port is specified)
			if (is_port_specified && last_port != port_) {
				is_pid_matched = false; //Reset Stat
				continue;
			}

			//Push to log buffer
			r.push_back({ time, last_port, last_pid, destination, CONNECT});
		}
		is_pid_matched = false;
	}

	util::ReportEvent("Finish parsing log, " + to_string(log_count) + "records processed.", false);

	return r;
}

bool ymt::PrintFormattedData(bool is_readable, string output_filename) {
	format_data_writer = ofstream(output_filename);

	if (!format_data_writer.is_open()) return false;

	return PrintFormattedData(is_readable, true);
}


bool ymt::PrintFormattedData(bool is_readable) {
	return PrintFormattedData(is_readable, false);
}

bool ymt::PrintFormattedData(bool is_readable, bool is_write) {

	vector<SSLog> data = GetFormattedData();

	string machine_name;

	//Obtain server name;
#ifdef _WIN32
	if (server_addr_.empty()) {
		util::ReportError("Please specify server name/address with -s");
		return false;
	}
	machine_name = server_addr_;
#else
	if (!server_addr_.empty()) {
		machine_name = server_addr_;
	}
	else {
		machine_name = util::SysExecute("hostname")[0];

		if (machine_name.empty()) {
			util::ReportError("Failed to obtain server name with \"hostname\" command, please check your system configuration");
			return false;
		}
	}
#endif

	if (data.empty()) return false;

	if (is_readable) {

		const size_t time_column_length = data[0].time.length() + 2; //"Time"
		const size_t port_column_length = 7; //"Port"
		const size_t pid_column_length = 6; //"PID"
		size_t machine_column_length;

		if (machine_name.length() > 7) {
			machine_column_length = machine_name.length() + 2;
		}
		else {
			machine_column_length = 9;
		}

		string head_line = "Time";
		string space = " ";

		//Time
		util::AppendDuplicateString(head_line, space, time_column_length - 4);

		//Machine
		head_line += "Machine";
		util::AppendDuplicateString(head_line, space, machine_column_length - 7);

		//Port
		head_line += "Port";

		util::AppendDuplicateString(head_line, space, port_column_length - 4);

		//PID
		head_line += "PID";
		util::AppendDuplicateString(head_line, space, pid_column_length - 3);

		//Destination (last element, no spaces needed)
		head_line += "Destination";
		head_line += '\n';

		if (is_write) {
			format_data_writer << head_line;
		}
		else {
			util::Print(head_line, true);
		}
		for (SSLog& i : data) {

			//Time
			string line = i.time;
			util::AppendDuplicateString(line, space, 2);

			//Machine
			line += machine_name;
			util::AppendDuplicateString(line, space, machine_column_length - machine_name.length());

			//Port
			line += i.port;
			util::AppendDuplicateString(line, space, port_column_length - i.port.length());

			//PID
			line += i.pid;
			util::AppendDuplicateString(line, space, pid_column_length - i.pid.length());

			//Destination (last element)
			line += i.destination;

			line += '\n';

			if (is_write) {
				format_data_writer << line;
			}
			else {
				util::Print(line, true);
			}
		}
	}
	else {
		for (SSLog& i : data) {
			string line = i.time + "," + machine_name + "," + i.port + "," + i.pid + "," + i.destination + '\n';
			if (is_write) {
				format_data_writer << line;
			}
			else {
				util::Print(line, true);
			}
		}
	}

	format_data_writer.close();

	return true;
}

void ymt::PrintStatProgress(const int& current,const int & total,const int & segment_size) {
	//Show progress

	string percent;

	if (current != total && current % segment_size == 0) {
		percent = util::TruncateDouble(((double)current / (double)total) * 100, 2);
		util::ReportEvent("Progress: " 
						    + to_string(current) 
						    + "/" 
						    + to_string(total) 
						    + ", " 
						    + percent 
						    + "%",
						  false,
						  false);
	}
	else if (current == total) {
		percent = util::TruncateDouble(100, 2);
		util::ReportEvent("Progress: " 
						    + to_string(current) 
						    + "/" 
						    + to_string(total) 
						    + ", " 
						    + percent 
						    + "%",
						  false,
						  true);
	}
}

vector<string> ymt::GetStringStats() {

	vector <SSLog> log = GetFormattedData();
	vector <InquiryData> temp_content;
	ofstream output;
	vector<string> r;

	long long int max_left_length = 0;

	size_t LOG_SIZE = log.size();

	if (LOG_SIZE == 0) {
		util::ReportError("No enough data for this port.\n");
		return r;
	}

	//Get website frequency data

	GetStats(WEBSITE_FREQUENCY, log, temp_content);

	//Push website frequency data

	for (auto& element : temp_content) {
		if (element.key.length() > max_left_length) {
			max_left_length = element.key.length();
		}
	}

	string header = "Website";
	util::AppendDuplicateString(header, " ", max_left_length - 7 + 2);
	header += "Count";

	r.push_back(header);

	for (auto& element : temp_content) {
		string line = element.key;
		util::AppendDuplicateString(line, " ", max_left_length - element.key.length() + 2);
		line += to_string(element.value);
		r.push_back(line);
	}

	temp_content.clear();

	r.push_back("");

	//Get port frequency data
	GetStats(PORT_FREQUENCY, log, temp_content);

	//Push port frequency data

	const int PORT_LENGTH = 13;

	r.push_back("User Port    Inquiry Count");
	
	for (auto& element : temp_content) {
		string line = element.key;
		util::AppendDuplicateString(line, " ", PORT_LENGTH - element.key.length());
		line += to_string(element.value);
		r.push_back(line);
	}

	return r;
}

void ymt::GetStats(StatType type, vector<SSLog> & log, vector<InquiryData> & buffer) {

	vector<InquiryData> temp_content;
	vector<long long> temp_index;

	const int LOG_SIZE = log.size();
	const int SEGMENT_SIZE = LOG_SIZE / 10;

	int counter = 0;


	util::ReportEvent(string("Analyzing ") + (type == WEBSITE_FREQUENCY ? "website frequency data..." : "port frequency data..."),false);

	//Parse Data
	switch (type) {
	case WEBSITE_FREQUENCY:
		for (auto& line : log) {
			counter++;

			bool is_website_matched = false;

			for (auto& n : temp_content) {
				
				if (line.destination == n.key) {
					n.value += 1;
					is_website_matched = true;
					break;
				}

			}

			if(!is_website_matched) {
				temp_content.push_back({ line.destination, 1 });
			}

			PrintStatProgress(counter, LOG_SIZE, SEGMENT_SIZE);
		}
		break;
	case PORT_FREQUENCY:
		for (auto& line : log) {

			counter++;

			bool is_port_matched = false;
			for (auto& n : temp_content) {
				if (line.port == n.key) {
					n.value += 1;
					is_port_matched = true;
				}
			}

			if(!is_port_matched) {
				temp_content.push_back({ line.port, 1 });
			}

			PrintStatProgress(counter, LOG_SIZE, SEGMENT_SIZE);
		}
		break;
	}

	//Do Quick Sort
	temp_index.reserve(temp_content.size());

	for (auto& i : temp_content) {
		temp_index.push_back(i.value);
	}

	auto final_index = util::QuickSort::Sort(temp_index);

	//Push it in
	for(int i = temp_index.size()-1; i >= 0; i--) {
		buffer.push_back(temp_content[final_index[i]]);
	}

}

void ymt::SetFileName(string filename) {
	config_ = filename;
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

	for (auto& i : users_) {
		if (port_ == i.GetAttribute(REMOTE_PORT)) {
			target_user = i;
			target_port = i.GetAttribute(REMOTE_PORT);
			is_user_located = true;
			break;
		}
	}

	//If user input is port
	for (auto& i : pid_table_) {
		if (port_ == i.port) {
			target_pid = i.pid;
			target_port = i.port;
			break;
		}
	}

	//If user input is pid
	if (target_port.empty()) {
		for (auto& i : pid_table_) {
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
		for (auto& i : users_) {
			if (target_port == i.GetAttribute(REMOTE_PORT)) {
				target_user = i;
				is_user_located = true;
				break;
			}
		}
	}

	//No found? Give up
	if (!is_user_located) {
		util::ReportError("Failed to find pid for the process dedicated to the specifiedd port, thus failed to obtain user log.");
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
								  {"Verbose",            target_user.GetAttribute(VERBOSE)}
						  });

	r.push_back("");
	r.push_back("SS Link:");
	r.push_back(GetSSShareLink(target_user));

	return r;
}

void ymt::UpdateUsers() {

	//Make template configuration (Can be modified by config)
	Parser default_config;

	//Read raw config
	vector <string> yaml_content = util::ReadFile(config_);

	//Parse YAML
	for (string& line : yaml_content) {

		//Skip blanklines & comments
		if (line.empty()) continue;
		else {
			size_t slash_pos = line.find("//");
			if (slash_pos != -1) {
				line = util::SubString(line, 0, slash_pos);
			}
			if (line.empty()) continue;
		}


		YAML l = util::DecodeYamlLine(line);

		//If it is global level -- no user
		if (l.level == 0) {
			switch (util::MatchWithWords(l.left,
										 { "group",
										  "nameserver",
										  "method",
										  "fastopen",
										  "redirect",
										  "timeout",
										  "server",
										  "tunnel_mode",
										  "verbose" })) {
			case 0:
				default_config.SetAttribute(GROUP_NAME, l.right);
				break;
			case 1:
				if (util::IsTheSame("localhost", l.right, false, false) ||
					util::IsTheSame("bind9-local", l.right, false, false)) {
					default_config.SetAttribute(DNS, util::GetMachineIP());
				}
				else {
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
		}
		else {
			Parser user = default_config;
			user.SetUser(l.left, l.right);
			users_.push_back(user);
		}
	}
}

bool ymt::UpdateLog() {

	string filename = input_log_.empty() ? "/var/log/syslog" : input_log_;

	if (!util::IsFileExist(filename)) return false;

	log_file = YFile(input_log_.empty() ? "/var/log/syslog" : input_log_);

	log_file.filter = FileFilter("ss-server[");

	return true;
}

string ymt::GetSSShareLink(Parser & user) {

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
