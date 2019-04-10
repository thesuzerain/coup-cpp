//
// Created by Wyatt Verchere on 2019-04-09.
//

#ifndef COUP_CPP_AI_H
#define COUP_CPP_AI_H

#include <vector>

#include "player.h"
#include "card.h"

int GetRandomChoice(int rand, int max);

Card GetBlockChoice(int rand, Player * player, std::vector<Card> cards);
int GetChallengeChoice(int rand);


class AI {
};


#endif //COUP_CPP_AI_H
