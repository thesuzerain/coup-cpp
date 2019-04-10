//
// Created by Wyatt Verchere on 2019-04-08.
//

#ifndef COUP_CPP_PLAYER_H
#define COUP_CPP_PLAYER_H

#include <string>
#include "game.h"
#include "card.h"

enum ActionType {income, foreign_aid, tax, exchange, steal, assassinate, coup, bad_action};

class Game;


std::string GetPlayersInfoString(Game * game, bool attacking);

class Player {


public:
    int cardsLeft;

    // We define it as a vector, as the total number of cards changes with Ambassador
    // (even though we never get rid of cards when losing an influence, we just reveal them)
    std::vector<Card> cards;

    int money;

    std::string playerName;

    Player(Card card1, Card card2, std::string name);

    void RunPlayerMain(Game * game, bool lastCommandInvalid);
    void RunOpponentMain();
    bool HasCard(Card card);


    };


#endif //COUP_CPP_PLAYER_H
