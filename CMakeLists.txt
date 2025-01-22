cmake_minimum_required(VERSION 3.30)
project(DetectiveGame_MB_Project)

set(CMAKE_CXX_STANDARD 20)

# Voeg SFML toe aan het project
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Voeg je executabele toe
add_executable(Game
        main.cpp
        LRParser/src/cfg.cpp
        LRParser/include/cfg.h
        LRParser/src/parsing_table.cpp
        LRParser/include/parsing_table.h
        LRParser/src/lrparser.cpp
        LRParser/include/lrparser.h
        Game/Game.h
        PDA/PDA.cpp
)

# Link SFML-bibliotheken
target_link_libraries(Game sfml-graphics sfml-window sfml-system)
