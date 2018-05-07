cmake_minimum_required(VERSION 3.9)
project(ss_manager)

set(CMAKE_CXX_STANDARD 11)

add_executable(ss_manager
        src/cpp/constants.hpp
        src/cpp/main.cpp
        src/cpp/ssm.cpp
        src/cpp/ssm.hpp src/cpp/util.cpp src/cpp/util.hpp)
