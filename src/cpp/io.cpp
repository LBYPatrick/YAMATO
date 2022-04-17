//
// Created by LBYPatrick on 3/16/2022.
//

#include "rapidjson/document.h"

#include "io.h"

int min_log_level = 0;

const char * kTags [] = {
        "VERBOSE",
        "DEBUG",
        "INFO",
        "WARNING",
        "ERROR"
};

int io::ReadFile(std::string_view filename, char **buf) {

    //Open file
    FILE * fp = fopen(filename.data(),"r");
    if(fp) return -1;

    //Get file size
    fseek(fp,0L,SEEK_END);
    const int sz = ftell(fp);
    rewind(fp);

    //Initialize buffer
    *buf = new char[sz];
    fgets(*buf,sz,fp);

    return 0;
}

void io::PrintLog(std::string str, int level) {

    if(level < min_log_level || level >= N_LOG_LEVELS) return;

    printf("[%s] %s\n", kTags[level], str.c_str());
}

void io::SetMinLogPrintLevel(int level) {
    if(level <= 5 && level >= 0)
        min_log_level = level;
}
