cmake_minimum_required(VERSION 3.22)
project(DetectiveGame_MB_Project)

set(CMAKE_CXX_STANDARD 20)

# Voeg SFML toe aan het project
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Voeg je executabele toe
add_executable(Game
        main.cpp
        LRParser/src/cfg.cpp
        LRParser/include/cfg.h
        LRParser/src/ParsingTable.cpp
        LRParser/include/ParsingTable.h
        LRParser/src/LALR_Parser.cpp
        LRParser/include/LALR_Parser.h
        PDA/PDA.cpp
        Game/StateManager.h
        Game/StateManager.cpp
        Game/State.cpp
        Game/State.h
)

# Link SFML-bibliotheken
target_link_libraries(Game sfml-graphics sfml-window sfml-system)

install(TARGETS Game DESTINATION bin)