//
// Created by Baiyi Liao on 4/16/22.
//

#include "utils.h"

#include <utility>

void Utils::Print(const std::string& tail, const std::vector<char *>& c_str_arr) {

    for(auto & i : c_str_arr)  {
        printf("%s", i);
    }

    printf("%s",tail.c_str());
}

void Utils::Print(const std::vector<char *>& c_str_arr) {
    Print("\n",c_str_arr);
}

