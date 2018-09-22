//
// Created by LBYPatrick on 8/8/2018.
//

#ifndef SS_MANAGER_PARSER_HPP
#define SS_MANAGER_PARSER_HPP

#include "constants.hpp"
#include "util.hpp"

const int NUM_SS_OPTIONS = 9;

enum SSInfo {
    REMOTE_PORT,
    LOCAL_PORT,
    KEY,
    METHOD,
    TCP_FASTOPEN,
    DNS,
    REDIRECT,
    TIMEOUT,
    SERVER,
	UDP_OR_TCP
};

enum TunnelMode {
	TCP,
	UDP,
	BOTH
};

class Parser {
public:
    vector<string> GetConfig();
    void SetUser(string port, string pass);
    void SetAttribute(SSInfo type, string value);
	string GetAttribute(SSInfo type);
private:
//Some Default Values
	string remote_port_ = "8388",
		local_port_ = "1080",
		user_pass_ = "NO_PASS",
		method_ = "chacha20-ietf",
		dns_ = "8.8.8.8",
		tcp_fastopen_ = "true",
		redirect_,
		timeout_ = "1440",
		server_ = "0.0.0.0";

	TunnelMode mode = TCP;
};


#endif //SS_MANAGER_PARSER_HPP
