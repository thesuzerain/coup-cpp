cmake_minimum_required(VERSION 3.13)
project(coup_cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(coup_cpp
        main.cpp
        main.h
        player.cpp
        player.h
        ui.cpp
        ui.h
        game.cpp
        game.h
        deck.cpp
        deck.h
        action.cpp
        action.h
        card.cpp
        card.h
        ai.cpp
        ai.h)
