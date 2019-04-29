//
// Created by Wyatt Verchere on 2019-04-08.
//

#include "deck.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */




Deck::Deck(){

    // Deck is initialized to have 3 of every card
    for (int i = 0; i < Card::numberOfCards; i++){
        cardsLeft[i] = 3;
    }
}


// Draws a random card from the deck by selecting one randomly
// Subtracts it from the cards stored in the deck and returns it
Card Deck::Draw(){

    srand(time(NULL));
    int randVal = rand() % CardCount();

    // Iterate through types of cards until we arrive at randomly selected one, draw it.
    for(int i = 0; i < Card::numberOfCards; i++){
        if(randVal < cardsLeft[i]){
            cardsLeft[i] --;
            return static_cast<Card>(i);
        } else {
            randVal -= cardsLeft[i];
        }
    }

    // Should not be able to get here.
    // Just in case, return some card.
    return static_cast<Card>(0);
}

void Deck::Replace(Card card){
    cardsLeft[card] ++;
}


int Deck::CardCount(){
   int ret = 0;
    for (int i = 0; i < Card::numberOfCards; i++){
        ret += cardsLeft[i];
    }
    return ret;
}