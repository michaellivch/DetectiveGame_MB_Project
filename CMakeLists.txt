cmake_minimum_required(VERSION 3.30)
project(DetectiveGame_MB_Project)

set(CMAKE_CXX_STANDARD 20)

add_executable(Game main.cpp
        CFG/cfg.cpp
        CFG/cfg.h)
