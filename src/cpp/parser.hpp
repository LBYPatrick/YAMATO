//
// Created by LBYPatrick on 8/8/2018.
//
#pragma once

#include "includes.hpp"

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
		dns_ = "1.1.1.1",
		tcp_fastopen_ = "true",
		redirect_,
		timeout_ = "1440",
		server_ = "0.0.0.0",
		verbose_ = "true",
        group_name_ = "Default";

	TunnelMode mode = TCP;
};