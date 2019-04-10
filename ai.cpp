//
// Created by Wyatt Verchere on 2019-04-09.
//

#include "ai.h"

#include <random>


int GetRandomChoice(int rand, int max){

    return rand % max;
}

Card GetBlockChoice(int rand, Player * player, std::vector<Card> cards){

    //If they have the card, they will always block with it
    for(int i = 0; i < cards.size(); i++){
        if(player->HasCard(cards[i])){
           return cards[i];
        }

    }

    return rand % 3 ? cards[0] : Card::numberOfCards;
}

int GetChallengeChoice(int rand){
    return rand % 8;

}
