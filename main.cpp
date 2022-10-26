//
// Created by Wyatt Verchere on 2019-04-08.
//

#include "player.h"
#include "ui.h"
#include "game.h"

#include <vector>
#include <iostream>



int main(){

    std::cout << "Welcome to Coup!" << std::endl;

    std::vector<std::string> names = { "Player", "Avery","Sam","Robin" };
    Game game = Game(names); // We can change this number to change the number of players
    
}

