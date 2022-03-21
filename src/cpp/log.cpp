//
// Created by LBYPatrick on 3/18/2022.
//

#include "log.h"

using std::string;
using std::vector;
using std::ostringstream;
using std::to_string;

const std::unordered_map<string,int> kColumnMapping = {

        {"time",0},
        {"group_name",1},
        {"pid",2},
        {"user_port",3},
        {"site",4},
        {"site_port",5}

};

string Log::ToString(string &buffer) {

    string ret;

    const vector<string> columns = ToVector();

    const size_t len = columns.size();

    for(size_t i = 0; i < len; ++i) {

        ret += columns[i];
        if(i != len - 1) ret += ", ";
    }

    return ret;
}



vector<string> Log::ToVector() {

    vector<string> ret;

    ret.push_back(time == (time_t)(-1) ? "NULL" : to_string(time));
    ret.push_back(group_name.empty() ? "NULL" : group_name);
    ret.push_back(pid == -1 ? "NULL" : std::to_string(pid));
    ret.push_back(user_port == -1 ? "NULL" : std::to_string(pid));
    ret.push_back(site.empty() ? "NULL" : group_name);
    ret.push_back(site_port == -1 ? "NULL" : std::to_string(pid));

    return vector<string>();
}

std::unordered_map<std::string, int> Log::GetColumnMapping() {
    return kColumnMapping;
}
