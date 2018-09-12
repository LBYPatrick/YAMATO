
#include "ssm.hpp"

string extra_param_;

void
ssm::RunConfig(string filename) {

	vector<string> yaml_content;
	vector<Parser> users;
	vector<PIDInfo> pids;

	Parser default_config;

	//Unload sessions from the config session if loaded already

	if (util::IsFileExist(filename + ".pidmap")) StopConfig(filename);

	yaml_content = util::ReadFile(filename);

	//Parse YAML
	for (string & line : yaml_content) {

		//Skip blanklines & comments
		if (line.size() == 0) continue;
		else {
			int slash_pos = line.find("//");
			if (slash_pos != -1) {
				line = util::SubString(line,0,slash_pos);
			}
			if (line.size() == 0) continue;
		}


		YAML l = util::GetYaml(line);

		//If it is global level -- no user
		if (l.level == 0) {
			switch (util::Search(l.left, { "group", "nameserver", "method","fastopen","redirect","timeout","server","tunnel_mode"})) {
			case  0: break; //Don't really know how can I use group name...
			case  1:
				default_config.SetAttribute(DNS, l.right);
				break;
			case  2:
				default_config.SetAttribute(METHOD, l.right);
				break;
			case  3:
				default_config.SetAttribute(TCP_FASTOPEN, l.right);
				break;
			case  4:
				default_config.SetAttribute(REDIRECT, l.right);
				break;
			case  5:
				default_config.SetAttribute(TIMEOUT, l.right);
				break;
			case  6:
				default_config.SetAttribute(SERVER, l.right);
				break;
			case  7:
				default_config.SetAttribute(UDP_TCP, l.right);
				break;
			default: break;
			}
		}
		else {
			Parser user = default_config;
			user.SetUser(l.left, l.right);
			users.push_back(user);
		}
	}


	//Start Running processes for users

	for (Parser & p : users) {
		pids.push_back({ RunUser(p),p.GetAttribute(REMOTE_PORT) });
	}

	//Write the pid list to a file

	ofstream writer;
	writer.open(filename + ".pidmap");
	for (PIDInfo & p : pids) {
		writer << p.port + ": " + p.pid + "\n";
	}
	writer.close();


	//Clean up
	util::RemoveFile("SS.conf");
	for (Parser & p : users) {
		util::RemoveFile(p.GetAttribute(REMOTE_PORT) + ".pid");
	}
}

string ssm::RunUser(Parser p) {

	string pid_buffer;

	vector<string> config = p.GetConfig();
	string file_buffer;

	for (string line : config) {
		file_buffer += line + "\n";
	}

	//Write user config
	util::WriteFile("SS.conf", { file_buffer });

	system(string("ss-server -c SS.conf " + extra_param_ + " -f " + p.GetAttribute(REMOTE_PORT) + ".pid").c_str());
	
	//Return pid buffer
	return util::ReadFile(p.GetAttribute(REMOTE_PORT) + ".pid",false)[0];
}

void
ssm::StopConfig(string filename) {
	vector<string> config;
	YAML yaml;
	int fails = 0;
	int goods;

	config = util::ReadFile(filename + ".pidmap");


	if (config.size() == 0) {
		util::ReportError(
			"Cannot read the pidmap for the file specified, please load the config before unload and DO NOT delete pidmap.");
		return;
	}

	for (string line : config) {
		yaml = util::GetYaml(line);

		if (goods > 3 || util::IsProcessAlive(yaml.right)) {
			util::SysExecute("kill -15 " + yaml.right);
			goods += 1;
		}
		else {
			fails++;
			if (fails >= 5) break;
		};
	}

	util::RemoveFile(filename + string(".pidmap"));
}

void
ssm::CheckPort(string filename, string port) {

	vector<string> pidmap;
	YAML yaml_line;
	string target_pid = "-1";

	pidmap = util::ReadFile(filename + ".pidmap");

	//Quit if cannot find the pidmap file
	if (pidmap.size() == 0) {
		util::ReportError(
			"Cannot read the pidmap for the file specified, please load the config before unload and DO NOT delete pidmap.");
		return;
	}

	//Assume the "port" parameter is a port
	for (string & line : pidmap) {

		yaml_line = util::GetYaml(line);

		if (yaml_line.left == port) {
			target_pid = yaml_line.right;
			break;
		}
	}

	if (target_pid == "-1") {

		//Now assume the "port parameter" is actually a PID
		for (string & line : pidmap) {

			yaml_line = util::GetYaml(line);

			if (yaml_line.right == port) {
				target_pid = yaml_line.right;
				port = yaml_line.left;
				break;
			}

		}

		//Still no found...
		if (target_pid == "-1") { return; }
	}

	vector<string> file_buffer = util::SysExecute(R"(journalctl | grep "ss-server\[)" + target_pid + R"(\]")");

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
	for (string & line : file_buffer) {
		printf("%s\n", line.c_str());
	}
	printf("\n"
		"=========================\n");
}

void
ssm::SetExtraParam(string extra_param) {
	extra_param_ = extra_param;
}
