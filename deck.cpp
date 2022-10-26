//
// Created by Wyatt Verchere on 2019-04-08.
//

#include "deck.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

Deck::Deck(){
    // Deck is initialized to have 3 of every card
    for (int i = 0; i < Card::numberOfCards; i++){
        cards.push_back(static_cast<Card>(i));
    }
}

Card Deck::Draw(){
    // Return null if empty deck
    if (CardCount() <= 0) {
        return Card::nullCard;
    }

    srand(time(NULL));
    int randVal = rand() % CardCount();

    Card card = cards[randVal];
    cards.erase(cards.begin()+randVal);
    return card;
}

void Deck::Replace(Card card){
    cards.push_back(card);
}

int Deck::CardCount(){
    int i = 0;
    // Counts only non-null cards
    for (Card c : cards)
        if (c != Card::nullCard && c != Card::numberOfCards)
            i++;
    return i;
}
