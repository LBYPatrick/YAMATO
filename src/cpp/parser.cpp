//
// Created by LBYPatrick on 8/8/2018.
//

#include "parser.hpp"

array<string, 9> WEBSITES = {
	"bing.com",
	"qq.com",
	"tmall.com",
	"amazon.cn",
	"sina.com.cn",
	"music.163.com",
	"bilibili.com",
	"bilibilijj.com",
	"baidu.com"
};

vector<string> Parser::GetConfig() {

    vector<string> return_buffer;

    return_buffer.push_back("{");

    return_buffer.push_back("server : \"" + server_ + "\",");
    return_buffer.push_back("server_port : \"" + remote_port_ + "\",");
    return_buffer.push_back("local_port : \"" + local_port_ + "\",");
    return_buffer.push_back("password : \"" + user_pass_ + "\",");
    return_buffer.push_back("method : \"" + method_ + "\",");
    return_buffer.push_back("fast_open : \"" + tcp_fastopen_ + "\",");
    return_buffer.push_back("timeout : \"" + timeout_ + "\",");
    return_buffer.push_back("nameserver : \"" + dns_ + "\",");

    if(redirect_.size() == 0) {

		int index = rand() % WEBSITES.size();
#if DEBUG
		printf("Max Index: %d\tGenerated Index: %d\n",WEBSITES.size() - 1, index);
#endif
        return_buffer.push_back("redirect : \"" + WEBSITES[index] + "\"");
    }
    else {
        return_buffer.push_back("redirect : \"" + redirect_ + "\"");
    }
    return_buffer.push_back("}");

#if DEBUG
    printf("From GetConfig(): \n\n");
    for(string & line : return_buffer) {
        printf("%s\n", line.c_str());
    }
    printf("\nEND\n");
#endif

    return vector<string>();
}

void Parser::SetUser(string port, string pass) {
    remote_port_ = port;
    user_pass_   = pass;
}

void Parser::SetAttribute(SSInfo type, string value) {
    switch(type) {
        case REMOTE_PORT :
            remote_port_ = value;
            break;
        case LOCAL_PORT :
            local_port_  = value;
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
    }
}

string Parser::GetAttribute(SSInfo type)
{
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
	}
	return string();
}
