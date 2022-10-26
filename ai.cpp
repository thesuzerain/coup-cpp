//
// Created by Wyatt Verchere on 2019-04-09.
//

#include "ai.h"
#include <random>

// Selects a random target out of available players.
int GetTarget(int rand, const Game & game, int thisId){

    // We loop until we randomly select a legal target.
    // There must be at least one or game would already be over.
    int selectTarget = 0;
    do {
        selectTarget = rand % game.playerCount;
        rand += 1;
    } while( game.players[selectTarget]->cardsLeft <= 0 || selectTarget == thisId); // Target must be a live and not themself

    return selectTarget;
}


Card GetBlockChoice(int rand, const Player & player, const std::vector<Card> & cards){

    if (player.cardsLeft <= 0) return Card::nullCard; // defeated player

    //If they have the card, they will always block with it
    for(int i = 0; i < cards.size(); i++){
        if(player.HasCard(cards[i])){
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

ActionType PickAction(int rand, Game & game, const Player & player){

    // We want the AI to favor actions it is legally able to do
    // Some actions are more heavily weighted than others

    double actionWeights[7] = {
        15, // Income Weight        - low, if we can do better, do better
        25, // Foreign Aid weight   - higher than Income, but not the optimal way to get money.
        50, // Tax Weight           - very often, if we can get away with it, tax is the best move
        40, // Exchange Weight      - Decent if necessary if other options are bad)
        18, // Steal Weight         - low, as it is very often blocked easily
        40, // Assassinate Weight   - high if possible
        80 // Coup Weight           - high. We always want to do this if we can.
    };

    //If we can't afford to coup or assassinate, drop those weights to 0
    if (player.money < 7)  actionWeights[ActionType::coup] = 0;
    if (player.money < 3)  actionWeights[ActionType::assassinate] = 0;

    // If we do NOT have the corresponding card for a given action, weight goes down as its a high-risk play (if we are challenged, we could lose influence)
    if(!player.HasCard(Card::duke)) actionWeights[ActionType::tax] *= 0.25;
    if(!player.HasCard(Card::ambassador)) actionWeights[ActionType::exchange] *= 0.25;
    if(!player.HasCard(Card::captain)) actionWeights[ActionType::steal] *= 0.25;
    if(!player.HasCard(Card::assassin)) actionWeights[ActionType::assassinate] *= 0.25;

    //If we do not have a Duke, or Ambassador (meaning it is more likely others will), increase weight for Income (guaranteed $)
    actionWeights[ActionType::income] *= 1.5; // Income Weight

    //If we have less than $3, we can't afford anything, so any action that gives us money will be prioritized (over Exchange, say)
    if (player.money < 3) {
        actionWeights[ActionType::income] *= 1.5;
        actionWeights[ActionType::foreign_aid] *= 1.5;
        actionWeights[ActionType::tax] *= 1.5;
        actionWeights[ActionType::steal] *= 1.5;
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
            return static_cast<ActionType>(j);
        } else {
            // We move to the next slot
            randVal -= actionWeights[j];
        }
    }

    // Shouldn't get here.
    // Defaults to income
    return ActionType::income;
}


int GetReplaceCard(int rand, const std::vector<Card>& cards){
    return rand % cards.size();
}
