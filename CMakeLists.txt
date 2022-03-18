cmake_minimum_required(VERSION 3.12)
project(YAMATO)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/cpp)

add_executable(YAMATO
        src/cpp/main.cpp src/cpp/core.h src/cpp/io.h src/cpp/rpc.h src/cpp/core.cpp src/cpp/io.cpp src/cpp/rpc.cpp src/cpp/parser.h src/cpp/parser.cpp src/cpp/config.cpp src/cpp/config.h)
