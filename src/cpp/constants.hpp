

#ifndef SS_MANAGER_CONSTANTS_H
#define SS_MANAGER_CONSTANTS_H

#include <vector>
#include <string>

using std::vector;
using std::string;

struct Json {
    string element;
    string key;
};

struct User {
    string port;
    string password;
};

struct Help {
    string option;
    string description;
};

#endif //SS_MANAGER_CONSTANTS_H