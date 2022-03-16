cmake_minimum_required(VERSION 3.12)
project(YAMATO)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/cpp)

add_executable(YAMATO
        src/cpp/main.cpp src/cpp/core.hpp src/cpp/config.hpp src/cpp/io.hpp src/cpp/rpc.hpp src/cpp/core.cpp src/cpp/io.cpp src/cpp/rpc.cpp)
