//
// Created by LBYPatrick on 3/16/2022.
//

#include "parser.h"
#include "io.h"

using std::unordered_map;
using std::deque;
using std::string;
using rapidjson::Document;

int Parser::ConfigToString(std::unordered_map<std::string, std::string> config, std::string &buffer) {
    return 0;
}

int Parser::ParseDom(Document * dom, Config **ret_root) {

    if(!dom || !dom->IsObject()) return ERR_PARSER_DOM_INVALID;

    *ret_root = new Config();

    return ParseDom(dom->GetObject(),*ret_root);
}

int Parser::ParseDom(rapidjson::Document::Object dom_parent, Config * config_parent) {

    auto node = new Config(config_parent);

    string tag = "";
    node->Get("tag",tag);

    for(auto & i : dom_parent) {

        //Deal with children
        if(strcmp(i.name.GetString(),"children") == 0 && i.value.IsArray()) {
            for (auto & j : i.value.GetArray()) {

                if(!j.IsObject() || !ParseDom(j.GetObject(),node)) {

                    io::PrintLog(string("Failed to parse children under parent with tag ")
                                 + tag
                                 + string("."), LOG_ERROR);

                    return ERR_PARSER_DOM_INVALID;
                }
            }
        }
        //Deal with port-password pairs
        else if(strcmp(i.name.GetString(),"combinations") == 0 && i.value.IsObject()) {
            for (auto & j : i.value.GetObject()) {
                auto temp = new Config(node);
                temp->Set("remote_port",j.name.GetString());
                temp->Set("password",j.value.GetString());
            }
        }
        //Deal with other key-value pairs
        else if(!node->Set(i.name.GetString(),i.value.GetString())) {

            io::PrintLog(string("Invalid key-value pair under parent with tag \""
            + tag
            + string("\" (")
            + string(i.name.GetString())
            + ","
            + string(i.value.GetString())),LOG_ERROR);

            return ERR_PARSER_DOM_INVALID;
        }
    }

    return 0;
}

