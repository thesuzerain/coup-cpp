//
// Created by Wyatt Verchere on 2019-04-10.
//

#ifndef COUP_CPP_ACTION_HELPER_H
#define COUP_CPP_ACTION_HELPER_H

#include "game.h"
#include "ui.h"
#include "ai.h"


bool DoChallenge(Game * game, int challengerId, int challengedId, Card challengeCard);
bool CheckChallenge(Game * game, int playerId, Card card, std::string text);
Card CheckBlock(Game * game, int target, ActionType actionType, std::string text, const std::vector<Card> & blockCard);

void loseInfluence(Game * game, int playerId);


#endif //COUP_CPP_ACTION_HELPER_H
