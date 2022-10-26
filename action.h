//
// Created by Wyatt Verchere on 2019-04-08.
//

// Provides handling for possible in-game possible actions the players can take.


#ifndef COUP_CPP_ACTION_H
#define COUP_CPP_ACTION_H

#include "actiontype.h"
#include "game.h"

// Checks if 'player' can legally declare an action of type 'actionType' based on the amount of moeny they have.
// (Not to be confused with whether that action would be successful, or whether their declaration would be a bluff.)
bool GetCanDoAction(const Player & player, ActionType actionType);

// Handles potential challenging of a card of type 'card' being played.
// Displays text 'text' beforehand.
// Modifies 'game' based on chalenge success.
bool TryChallenge(Game& game, int playerId, Card card, std::string text);

// Handles potential blocking of a card being played, by card of type blockCard.
// Displays text 'text' beforehand.
// Modifies 'game' based on block success.
Card TryBlock(Game& game, int target, ActionType actionType, std::string text, const std::vector<Card>& blockCard);

// Handles the challenging of challengerId challenging challengedId over their use of challengeCard.
bool DoChallenge(Game& game, int challengerId, int challengedId, Card challengeCard);

// Handles the declaration of playerId to be taking an action of actionType.
// It handles blocking and challenging of this action, and the delegation of it to other helper functions for each possible action type.
// playerId is the player doing the action, and targetId is the targeted player if applicable.
// Modifies 'game'.
void DoAction(Game& game, int playerId, ActionType actionType, int targetId);

// Helper for Assassinate action (removes influence from 'targetId' if unblocked by Contessa or not successefully challenged)
void HandleAssassinate(Game & game, int playerId, int targetId);

// Helper for Coup action (immediately remove influence from 'targetId')
void HandleCoup(Game & game, int playerId, int targetId);

// Helper for Income action (immediately give playerId +1 money)
void HandleIncome(Game & game, int playerId, int targetId);

// Helper for Foreign Aid (gives playerId +2 money unless blocked by Duke)
void HandleForeignAid(Game & game, int playerId, int targetId);

// Helper for Tax (gives playerId +3 money unless challenged)
void HandleTax(Game & game, int playerId, int targetId);

// Helper for Exchange (playerId doubles the number of cards in their hands then discards that many, unless challenged)
void HandleExchange(Game & game, int playerId, int targetId);

// Helper for Steal (gives playerId +2 money, and targetId loses 2, unless challenged or blocked by Captain or Ambassador)
void HandleSteal(Game & game, int playerId, int targetId);

// Handles losing a card after a failed challenge, assasination, coup, etc.
// Modifies 'game'.
void LoseInfluence(Game & game, int playerId);


#endif // COUP_CPP_ACTION_H