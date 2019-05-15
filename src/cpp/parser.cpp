//
// Created by LBYPatrick on 8/8/2018.
//

#include "parser.hpp"
#include "util.hpp"

vector<string> Parser::GetConfig() {

    vector <string> return_buffer;

    return_buffer.push_back("{");

    return_buffer.push_back(R"("server" : ")" + server_ + "\",");
    return_buffer.push_back(R"("server_port" : ")" + remote_port_ + "\",");
    return_buffer.push_back(R"("local_port" : ")" + local_port_ + "\",");
    return_buffer.push_back(R"("password" : ")" + user_pass_ + "\",");
    return_buffer.push_back(R"("method" : ")" + method_ + "\",");
    return_buffer.push_back("\"fast_open\" : " + tcp_fastopen_ + ",");
    return_buffer.push_back("\"timeout\" : " + timeout_ + ",");
    return_buffer.push_back(R"("nameserver" : ")" + dns_ + "\",");

    if (!redirect_.empty()) {
		return_buffer.push_back(R"("redirect" : ")" + redirect_ + "\",");
    }

    return_buffer.push_back(R"("mode" : ")" + GetAttribute(UDP_OR_TCP) + "\"");


    return_buffer.push_back("}");

#if DEBUG_CONFIG
    printf("From GetConfig(): \n\n");
    for(string & line : return_buffer) {
        printf("%s\n", line.c_str());
    }
    printf("\nEND\n");
#endif

    return return_buffer;
}

bool Parser::WriteConfig(string filename) {

	ofstream o(filename);

	if (!o.is_open()) {
		return false;
	}

	for (string & line : GetConfig()) {
		o << line << "\n";
	}

	//I don't really need to close the ofstream because it would close itself after the destructor is called
	return true;
}

void Parser::SetUser(string port, string pass) {
    remote_port_ = std::move(port);
    user_pass_ = pass;
}

void Parser::SetAttribute(SSInfo type, string value) {
    switch (type) {
        case REMOTE_PORT :
            remote_port_ = value;
            break;
        case LOCAL_PORT :
            local_port_ = value;
            break;
        case KEY :
            user_pass_ = value;
            break;
        case METHOD:
            method_ = value;
            break;
        case TCP_FASTOPEN:
            tcp_fastopen_ = value;
            break;
        case DNS:
            dns_ = value;
            break;
        case REDIRECT:
            redirect_ = value;
            break;
        case TIMEOUT:
            timeout_ = value;
            break;
        case SERVER:
            server_ = value;
            break;
        case UDP_OR_TCP:
            if (util::IsTheSame(value, "tcp", true, false)) {
                mode = TCP;
            } else if (util::IsTheSame(value, "udp", true, false)) {
                mode = UDP;
            } else {
                mode = BOTH;
            }
            break;
        case VERBOSE:
            verbose_ = (value == "true" ? "true" : "false");
            break;
        case GROUP_NAME:
            group_name_ = value;
            break;
    }
}

string Parser::GetAttribute(SSInfo type) {
    switch (type) {
        case REMOTE_PORT:
            return remote_port_;
            break;
        case LOCAL_PORT:
            return local_port_;
            break;
        case KEY:
            return user_pass_;
            break;
        case METHOD:
            return method_;
            break;
        case TCP_FASTOPEN:
            return tcp_fastopen_;
            break;
        case DNS:
            return dns_;
            break;
        case REDIRECT:
            return redirect_;
            break;
        case TIMEOUT:
            return timeout_;
            break;
        case SERVER:
            return server_;
            break;
        case UDP_OR_TCP:
            switch (mode) {
                case UDP:
                    return "udp_only";
                    break;
                case TCP:
                    return "tcp_only";
                    break;
                case BOTH:
                    return "tcp_and_udp";
                    break;
                default:
                    break;
            }
            break;
        case VERBOSE:
            return verbose_;
            break;
        case GROUP_NAME:
            return group_name_;
            break;
    }
    return string();
}


vector<size_t> util::QuickSort::Sort(vector<long long>& arr, size_t low, size_t high, bool is_increasing_order) {

	//Get the indexes of the sorted array in increasing order
	vector<size_t> raw = Sort(arr, low, high);

	if (is_increasing_order) {
		return raw;
	}
	else {
		const size_t raw_size = raw.size();
		const size_t halfway = (raw_size - raw_size % 2) / 2 - 1;

		for (size_t i = 0; i <= halfway; ++i) {
			std::swap(raw[i], raw[raw_size - 1 - i]);
		}
		return raw;
	}

	return vector<size_t>();
}

