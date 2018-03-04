#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

struct Param {
    string full;
    string abbr;
};

struct Json {
    string element;
    string key;
};

struct User {
    string port;
    string password;
};