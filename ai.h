//
// Created by Wyatt Verchere on 2019-04-09.
//

#ifndef COUP_CPP_AI_H
#define COUP_CPP_AI_H

#include <vector>

#include "player.h"
#include "card.h"

int GetTarget(int rand, Game * game, int thisId);

Card GetBlockChoice(int rand, Player * player, std::vector<Card> cards);

int GetChallengeChoice(int rand);

int PickAction(int rand, Game * game,Player * player);

int GetReplaceCard(int rand, std::vector<Card>);


#endif //COUP_CPP_AI_H
