//
// Created by Wyatt Verchere on 2019-04-09.
//

#include "ai.h"

#include <random>

// Selects a random target out of available players.
int GetTarget(int rand, Game * game, int thisId){

    // We loop until we randomly select a legal target.
    // There must be at least one or game would already be over.

    int selectTarget = 0;
    do {
        selectTarget = rand % game->playerCount;
        rand += 1;
    } while( game->players[selectTarget]->cardsLeft <= 0 || selectTarget == thisId); // Target must be a live and not themself

    return selectTarget;
}


Card GetBlockChoice(int rand, Player * player, std::vector<Card> cards){

    //If they have the card, they will always block with it
    for(int i = 0; i < cards.size(); i++){
        if(player->HasCard(cards[i])){
           return cards[i];
        }
    }

    // We bluff a block 1/3 times
    return rand % 3 ? cards[0] : Card::nullCard;
}

int GetChallengeChoice(int rand){
    // We challenge cards 1/8 times.
    return rand % 8;
}

int PickAction(int rand, Game * game,Player * player){

    // We want the AI to favor actions it is legally able to do
    // Some actions are more heavily weighted than others

    enum ActionType {income, foreign_aid, tax, exchange, steal, assassinate, coup, bad_action};

    double actionWeights[7];
    actionWeights[0] = 15; // Income Weight
    actionWeights[1] = 25; // Foreign aid Weight
    actionWeights[2] = 50; // Tax Weight
    actionWeights[3] = 40; // Exchange Weight- dependent on what cards we have
    actionWeights[4] = 18; // Steal Weight - Low because it is very rarely unchallenged
    actionWeights[5] = 40; // Assassinate Weight - higher when one other player
    actionWeights[6] = 80; // Coup  Weight

    // If we do NOT have the corresponding card, weight goes down as its a high-risk play (if we are challenged)
    if(!player->HasCard(Card::duke)) actionWeights[2] *= 0.25;
    if(!player->HasCard(Card::ambassador)) actionWeights[3] *= 0.25;
    if(!player->HasCard(Card::captain)) actionWeights[4] *= 0.25;
    if(!player->HasCard(Card::assassin)) actionWeights[5] *= 0.25;


    //If we can't afford to coup or assassinate, drop those weights to 0
    if(player->money < 7)  actionWeights[6] = 0;
    if(player->money < 3)  actionWeights[5] = 0;

    //If we do not have a Duke, or Ambassador (meaning more likely others will), increase weight for Income (guaranteed $)
    actionWeights[0] *= 1.5; // Income Weight

    //If we have less than $3, we can't afford anything, and weights related to money collection will go up
    if(player->money < 3){
        actionWeights[0] *= 1.5;
        actionWeights[1] *= 1.5;
        actionWeights[2] *= 1.5;
        actionWeights[4] *= 1.5;
    }

    //TODO: Add more ways to change weights, or (later) a more sophisticated state-based AI (small enough game to include ML maybe?).

    // We randomly select a variable based off weights.
    // (Random number from 0 to sum, slot-based, so first 0-X will be Income, X-Y will be foreign aid, etc,
    // where Y-X is the foreign aid weight, X - 0 is the income weight, etc)

    double sum = 0;
    for(int i = 0; i < 7; i++) sum += actionWeights[i];

    double randVal = rand % (int)(sum+0.5); // add 0.5 so it rounds correctly
    for(int j = 0; j < 7; j++) {
        if(randVal < actionWeights[j]){
            // We have selected this action
            return j;
        } else {
            // We move to the next slot
            randVal -= actionWeights[j];
        }
    }

    return 0;

}


int GetReplaceCard(int rand, std::vector<Card> cards){
    return rand % cards.size();
}
