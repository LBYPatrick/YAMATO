

#pragma once

#define SOFTWARE_VERSION "1.2.1"
#define DEBUG_CONFIG false
#define DEBUG_ANALYZER true
#define DEBUG_CMDOUT false

using namespace std;

#include <vector>
#include <string>
#include <array>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <iostream>

struct InquiryData {
	string key;
	size_t value;
};

struct Help {
	string option;
	string description;
};

struct YAML {
	int level;
	string left;
	string right;
};

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
	UDP_OR_TCP,
	VERBOSE
};

enum TunnelMode {
	TCP,
	UDP,
	BOTH
};

struct PIDInfo {
	string port;
	string pid;
};

enum YMTAttributes {
	CONFIG_FILENAME,
	LOG_INPUT_FILENAME,
	EXTRA_PARAM,
	PORT
};

enum LogBehavior {
	CONNECT,
	HANDSHAKE_FAIL
};

struct SSLog {
	string time;
	string port;
	string pid;
	string destination;
	LogBehavior behavior;
};

struct SortItem {
    size_t old_index;
    long long key;
};

struct SpeedData {
    string location;
    long up;
    long down;
    long latency;
};