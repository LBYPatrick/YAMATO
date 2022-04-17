//
// Created by LBYPatrick on 3/16/2022.
//

#ifndef YAMATO_PARSER_H
#define YAMATO_PARSER_H

#include <string>
#include <unordered_map>
#include <queue>
#include <format>
#include "config.h"
#include "rapidjson/document.h"

#define ERR_PARSER_DOM_INVALID 1

namespace Parser {

    int ParseDom(rapidjson::Document::Object dom_parent,Config * config_parent);
    int ParseDom(rapidjson::Document * dom,Config ** ret_root);
    int ConfigToString(std::unordered_map<std::string, std::string> config, std::string & buffer);

}

#endif //YAMATO_PARSER_H
