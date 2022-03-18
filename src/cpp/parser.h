//
// Created by LBYPatrick on 3/16/2022.
//

#ifndef YAMATO_PARSER_H
#define YAMATO_PARSER_H

#include <string>
#include <unordered_map>


namespace Parser {

    int ParseFile(std::string filename,std::vector<std::unordered_map<std::>>);
    int ConfigToString(std::unordered_map<std::string, std::string> config, std::string & buffer);

}

#endif //YAMATO_PARSER_H
