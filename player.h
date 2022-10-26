//
// Created by Wyatt Verchere on 2019-04-08.
//

// Class for a given player (who has a set of cards in their hand)

#ifndef COUP_CPP_PLAYER_H
#define COUP_CPP_PLAYER_H

#include <string>
#include <vector>

#include "card.h"

class Player {

public:

    Player(Card card1, Card card2, std::string name);

    int cardsLeft;

    // We define a player's hand as a vector, as the total number of cards changes with Ambassador
    // (even though we never get rid of cards when losing an influence, we just reveal them)
    std::vector<Card> cards;

    int money;
    int myId; // locally stored index of player

    std::string playerName;

    // Checks cards in hand for whether or not 'card' is owned 
    bool HasCard(Card card) const;

    // Creates string that describes visible information about this player (revealed cards, etc)
    std::string GetPlayerInfoString(bool attacking);

    // Creates string that describes all possible actions player can take with current cards (For display in terminal)
    std::string Player::GetPlayerActionsString();

};

#endif //COUP_CPP_PLAYER_H
