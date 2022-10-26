//
// Created by Wyatt Verchere on 2019-04-09.
//

#ifndef COUP_CPP_AI_H
#define COUP_CPP_AI_H

#include <vector>

#include "player.h"
#include "game.h"
#include "card.h"
#include "actiontype.h"

// Selects target for 'thisId' to use a card on, randomly out of available players
// Returns target as index.
int GetTarget(int rand, const Game & game, int thisId);

// Decides whether or not an AI should claim a block, and with which card
// std::vector<Card> cards is a list of cards for which a block can be claimed
// Returns nullCard if no block, otherwise returns Card
Card GetBlockChoice(int rand, const Player & player, const std::vector<Card> & cards);

// Decides whether or not this AI should issue a challenge when a player claims a card.
// TODO:  make this AI more robust- currently it just randomly challenges 1/8 times.
int GetChallengeChoice(int rand);

// Uses a weighted random AI system to pick what Action an AI ('player') will attempt to perform on its turn.
// Returns an ActionType that is chosen and legal. 
ActionType PickAction(int rand, Game & game, const Player & player);

// Decides which cards to put back in the deck (ie: after an Exchange).
// TODO: make this AI more robust- currently it just randomly chooses.
int GetReplaceCard(int rand, const std::vector<Card> &);

#endif //COUP_CPP_AI_H
