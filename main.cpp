//
// Created by Wyatt Verchere on 2019-04-08.
//

#include "main.h"
#include "player.h"
#include "ui.h"
#include "game.h"

#include <iostream>



int main(){


    std::cout << "Welcome to Coup!" << std::endl;


    PrintNewFrame("hi");

    std::cout << Colorize("blue text",Color::BLUE) << std::endl;

    Game * thisGame = new Game(4);

    while(true){
        thisGame->RunTurn(0);
    }

}

