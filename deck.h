//
// Created by Wyatt Verchere on 2019-04-08.
//

// Game object representing a Deck of cards (shared by all players).
// Essentially a wrapper for an std::vector

#ifndef COUP_CPP_DECK_H
#define COUP_CPP_DECK_H

#include <vector>
#include "card.h"

class Deck {

private:
    int CardCount();

    std::vector<Card> cards;

public:
    // Initializes a new deck with 3 of each possible card type.
    Deck();

    // Draws a random card from the deck by selecting one randomly
    // Subtracts it from the cards stored in the deck and returns it
    // Returns nullCard if deck empty
    Card Draw();

    // Adds a copy of 'card' to the deck.
    void Replace(Card card);

};
#endif //COUP_CPP_DECK_H