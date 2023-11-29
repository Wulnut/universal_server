cmake_minimum_required(VERSION 3.10.2)
project(universal_server)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_C_COMPILER "/usr/bin/clang")
set (CMAKE_CXX_COMPILER "/usr/bin/clang++")

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CLANG_TIDY_FIX ON)
set (CLANG-TIDY-EXE ".clang-tidy")

if (EXISTS "${CLANG-TIDY-EXE}")
    message(STATUS "use clang-tidy to check")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG-TIDY-EXE};-checks=-*,modernize-use-nullptr;-p=${CMAKE_BINARY_DIR};-export-fixes=clang-tidy-fixes.yml")
    if(CLANG_TIDY_FIX)
        message(STATUS "apply suggested fix" )
        set(CMAKE_CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY};-fix")
    endif()
endif()

include_directories(.)

add_executable(universal_server
        main.cc
        src/us_service.cc
        src/us_server.cc
        src/inactive_conn_mgr.cc
        include/us_server.h
        include/json.hpp
        include/us_service.h
        include/common.h
        include/inactive_conn_mgr.h
)

TARGET_LINK_LIBRARIES(
        universal_server
        muduo_net
        muduo_base
        pthread
)