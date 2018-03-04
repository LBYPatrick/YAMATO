cmake_minimum_required(VERSION 3.9)
project(ss_manager)

set(CMAKE_CXX_STANDARD 11)

add_executable(ss_manager
        src/cpp/constants.h
        src/cpp/main.cpp
        src/cpp/SSManager.cpp
        src/cpp/SSManager.h src/cpp/Utils.cpp src/cpp/Utils.h)
