cmake_minimum_required(VERSION 3.13)
project(coup_cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(coup_cpp
        action.cpp
        action.h
        ai.cpp
        ai.h
        card.h
        deck.cpp
        deck.h
        game.cpp
        game.h
        main.cpp
        player.cpp
        player.h
        ui.cpp
        ui.h  "actiontype.h")
