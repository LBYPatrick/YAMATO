//
// Created by LBYPatrick on 2/6/2018.
//

#ifndef SS_MANAGER_UTILS_H
#define SS_MANAGER_UTILS_H
#include <string>
#include "constants.h"

using std::string;

namespace Utils {
    void RemoveLetter(string & original_string, char letter);
    Json GetJson(string raw_json_line);
    void reportError(string message);
}

#endif //SS_MANAGER_UTILS_H
