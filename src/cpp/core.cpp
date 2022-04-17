//
// Created by LBYPatrick on 3/16/2022.
//

#include "core.h"
#include "io.h"
#include "config.h"
#include "parser.h"

using namespace rapidjson;
using std::string;

int core::StartSS(std::string_view config_name) {

    char * buf = NULL;

    if(!io::ReadFile(config_name,&buf)) {
        io::PrintLog(string("No file permission to read the specified config ") + string(config_name),LOG_ERROR);
        return ERR_CORE_CONFIG_PERM;
    }

    Document dom;
    dom.Parse(buf);
    free(buf);

    if(dom.HasParseError()) {

        io::PrintLog(string("The JSON object from ") + string(config_name) + " contains syntax errors.",LOG_ERROR);
        return ERR_CORE_CONFIG_INVALID;
    }

    Config * root = NULL;

    if(!Parser::ParseDom(&dom,&root)) {

    }



    return 0;
}

int core::StopSS(std::string_view config_name) {
    return 0;
}

int core::GetParsedLog(std::string_view config_name, std::vector<std::string> &buffer) {
    return 0;
}

int core::StartRPC(std::string_view config_name) {
    return 0;
}

int core::StopRPC(std::string_view config_name) {
    return 0;
}
