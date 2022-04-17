//
// Created by LBYPatrick on 3/16/2022.
//

#ifndef YAMATO_IO_H
#define YAMATO_IO_H
#define LOG_VERBOSE 0
#define LOG_DEBUG 1
#define LOG_INFO 2
#define LOG_WARNING 3
#define LOG_ERROR 4
#define N_LOG_LEVELS 5


#include <string>

namespace io {

    int ReadFile(std::string_view filename, char ** buf);
    void SetMinLogPrintLevel(int level);
    void Print(char * str, int level);
    void PrintLog(std::string str, int level);

}

#endif //YAMATO_IO_H