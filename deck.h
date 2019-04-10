//
// Created by Wyatt Verchere on 2019-04-08.
//

#ifndef COUP_CPP_DECK_H
#define COUP_CPP_DECK_H

#include "card.h"

class Deck {

private:
    int * cardsLeft;

    int CardCount();
public:
    Deck();
    Card Draw();
    void Replace(Card card);

};


#endif //COUP_CPP_DECK_H
