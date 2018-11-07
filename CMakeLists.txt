cmake_minimum_required(VERSION 3.12)
project(YAMATO)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/cpp)

add_executable(YAMATO
        src/cpp/includes.hpp
        src/cpp/main.cpp
        src/cpp/parser.cpp
        src/cpp/parser.hpp
        src/cpp/util.cpp
        src/cpp/util.hpp
        src/cpp/ymt.cpp
        src/cpp/ymt.hpp)
