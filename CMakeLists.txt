cmake_minimum_required(VERSION 3.30)
project(DetectiveGame_MB_Project)

set(CMAKE_CXX_STANDARD 20)

add_executable(Game main.cpp
        LRParser/src/cfg.cpp
        LRParser/include/cfg.h
        LRParser/src/parsing_table.cpp
        LRParser/include/parsing_table.h
        LRParser/src/lrparser.cpp
        LRParser/include/lrparser.h)
