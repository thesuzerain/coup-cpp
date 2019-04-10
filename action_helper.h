//
// Created by Wyatt Verchere on 2019-04-10.
//

#ifndef COUP_CPP_ACTION_HANDLE_H
#define COUP_CPP_ACTION_HANDLE_H

bool DoChallenge(Game * game, int challengerId, int challengedId, Card challengeCard);
bool CheckChallenge(Game * game, int playerId, ActionType actionType, int targetId, Card card, std::string text);
Card CheckBlock(Game * game, int target, ActionType actionType, std::string text, std::vector<Card> blockCard);

void loseInfluence(Game * game, int playerId);

class action_handle {

};


#endif //COUP_CPP_ACTION_HANDLE_H
