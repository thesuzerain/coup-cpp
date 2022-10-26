//
// Created by Wyatt Verchere on 2019-04-08.
//

// A class that represents the game state itself (including its players, cards, etc)

#ifndef COUP_CPP_GAME_H
#define COUP_CPP_GAME_H

#include <vector>
#include <string>
#include <memory>

#include "player.h"
#include "deck.h"

class Game {

public:

    // Generates a new Game with as many players as there are listed in playerNames.
    // The first name listed will correspond to the name of the live player.
    Game(std::vector<std::string> playerNames);

    std::unique_ptr<Deck> deck;

    int playerCount;
    std::vector<std::shared_ptr<Player>> players; // The real player is always player 0

    // Runs the turn for playerId
    // Delegates to either RunPlayerMain or RunOpponentMain as appropriate.
    void RunTurn(int playerId);

    // Runs the entirety turn for the active live player, based on their terminal input.
    void RunPlayerMain(bool lastCommandInvalid);
    
    // Runs the entirety of the turn for a given AI NPC, using a weighted RNG to decide their action.
    void RunOpponentMain(int id);

};


#endif //COUP_CPP_GAME_H
