//
// Created by Wyatt Verchere on 2019-04-08.
//

#ifndef COUP_CPP_GAME_H
#define COUP_CPP_GAME_H

#include <vector>
#include "player.h"
#include "deck.h"

class Player;

class Game {

public:

    Deck * deck;

    int playerCount;
    std::vector<Player *> players; // The player is always player 0

    Game(int playerCount);
    void RunTurn(int playerId);
};


#endif //COUP_CPP_GAME_H
