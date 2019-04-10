//
// Created by Wyatt Verchere on 2019-04-08.
//

#ifndef COUP_CPP_ACTION_H
#define COUP_CPP_ACTION_H

#include "player.h"
#include "ui.h"
#include "action_helper.h"


bool CanDoAction(Player * player, ActionType actionType);

void DoAction(Game * game, int playerId, ActionType actionType, int targetId);

void HandleAssassinate(Game * game, int playerId, ActionType actionType, int targetId);
void HandleCoup(Game * game, int playerId, ActionType actionType, int targetId);
void HandleIncome(Game * game, int playerId, ActionType actionType, int targetId);
void HandleForeignAid(Game * game, int playerId, ActionType actionType, int targetId);
void HandleTax(Game * game, int playerId, ActionType actionType, int targetId);
void HandleExchange(Game * game, int playerId, ActionType actionType, int targetId);
void HandleSteal(Game * game, int playerId, ActionType actionType, int targetId);


#endif //COUP_CPP_ACTION_H
