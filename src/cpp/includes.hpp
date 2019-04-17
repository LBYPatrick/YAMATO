

#pragma once

#define SOFTWARE_VERSION "1.4.1"
#define _CRT_SECURE_NO_WARNINGS

#define DEBUG_CONFIG false
#define DEBUG_IO false
#define DEBUG_ANALYZER false
#define DEBUG_CMDOUT false
#define READ_BUFFER_SIZE 16 * 1024 //16K

using namespace std;

#include <vector>
#include <string>
#include <array>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <chrono>

#ifdef _WIN32

#define popen _popen
#define pclose _pclose

#endif

struct InquiryData {
    string key;
    size_t value;
};

enum StatType {
	PORT_FREQUENCY,
	WEBSITE_FREQUENCY
};

struct TableElement {
    string l_element;
    string r_element;
};

struct YAML {
    size_t level = 0;
    string left = "";
    string right = "";
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
    VERBOSE,
    GROUP_NAME
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
	PORT,
	SERVER_ADDR,
	PROFILE_NAME,
	NEED_DUMP_BEFORE_LOAD
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

struct FileFilter {
	string key;
	bool is_include = false;

	FileFilter(string key) {
		this->key = key;
		is_include = true;
	}
	FileFilter() {}
};

struct YFile {
	string filename;
	FileFilter filter;

	YFile(string filename) {
		this->filename = filename;
	}
	YFile() {}
	YFile(YFile& another) {
		this->filename = another.filename;
		this->filter = another.filter;
	}
};