cmake_minimum_required(VERSION 3.25)
project(universal_server)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(universal_server
        main.cpp)
