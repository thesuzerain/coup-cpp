//
// Created by Wyatt Verchere on 2019-04-08.
//

#include "action.h"
#include "ui.h"
#include "deck.h"
#include "ai.h"

#include <random>
#include <iostream>
#include <sstream>
#include <algorithm>    // std::transform



// Checks if a player is allowed to perform an action for their amount of money
// Assassinate and Coup have monetary requirements, you have to have less than 10 to NOT do a coup
bool CanDoAction(Player * player, ActionType actionType){

    switch(actionType){
        case ActionType::assassinate: return (player->money >= 3);
        case ActionType::coup : return (player->money >= 7);
        case ActionType::income: return (player->money < 10);
        case ActionType::foreign_aid: return (player->money < 10);
        case ActionType::tax: return (player->money < 10);
        case ActionType::steal: return (player->money < 10);
        case ActionType::exchange: return (player->money < 10);
        default: return false;
    }

}

// Performs an action of the players choice
// A switch statement handles different behaviours for different actions
void DoAction(Game * game, int playerId, ActionType actionType, int targetId) {

    srand(time((NULL)));

    // We use this stream for the text being sent to frames
    std::stringstream ss;

    // We delegate the handling of each possible action type to a helper function using a switch statement
    switch (actionType) {

        // Assassination
        case ActionType::assassinate:
            HandleAssassinate(game,playerId,actionType,targetId);
            break;

        // Coup
        case ActionType::coup :
            HandleCoup(game,playerId,actionType,targetId);
            break;

            // Income
        case ActionType::income:
            HandleIncome(game,playerId,actionType,targetId);
            break;

            // Foreign Aid
        case ActionType::foreign_aid:
            HandleForeignAid(game,playerId,actionType,targetId);
            break;

            //Tax
        case ActionType::tax:
            HandleTax(game,playerId,actionType,targetId);
            break;

            //Steal
        case ActionType::steal:
            HandleSteal(game,playerId,actionType,targetId);
            break;

            // Exchange
        case ActionType::exchange:
            HandleExchange(game,playerId,actionType,targetId);
            break;
        default:
            break;
    }
}

// Helper function for the "Assassinate" action
// It can be challenged, blocked by Contessa, and that blocking can be contested
void HandleAssassinate(Game * game, int playerId, ActionType actionType, int targetId) {
    std::stringstream ss;

    // Regardless of challenging and whether it goes through, the player has to pay to perform the action
    game->players[playerId]->money -= 3;

    ss << game->players[playerId]->playerName << " has chosen to assassinate " << game->players[targetId]->playerName << ".\n";
    ss << "\n";
    ss << "Would you like to challenge " << game->players[playerId]->playerName << "'s Assassin card?\n";

    // We check if anyone wants to challenge.
    // If nobody challenges or none are successful, flow continues.
    if(!CheckChallenge(game, playerId, Card::assassin, ss.str())){

        ss.str(std::string());
        ss << game->players[playerId]->playerName << " has chosen to assassinate you.\n";
        ss << "\n";
        ss << "Would you like to block with a Contessa?\n";

        //We check if target wants to block with a Contessa.
        // If they do, we go into the challenge for Contessa
        // If not, we finish assassination
        if(CheckBlock( game, targetId, actionType, ss.str(), {Card::contessa}) == Card::contessa){

            ss.str(std::string());
            ss << game->players[targetId]->playerName << " is blocking with a Contessa.\n";
            ss << "\n";
            ss << "Would you like to challenge " << game->players[targetId]->playerName << "'s Contessa card?\n";

            // We check if anyone would like to challenge the defenders Contessa card.
            // If there is no succesful challenge, the block is succesful and the assassination fails, and we exit the action flow.
            if(!CheckChallenge(game, targetId, Card::contessa, ss.str())){
                ss.str(std::string());
                ss << "The assassination was blocked by a Contessa.\n";
                ss << "\n";
                GetPlayerConfirmation(ss.str());
                return;
            }
        }

        // Assassination successful.
        ss.str(std::string());
        ss << "The assassination was successful and "<<game->players[targetId]->playerName<<" lost an influence.\n";
        ss << "\n";
        GetPlayerConfirmation(ss.str());
        loseInfluence( game,  targetId);
    } else {
        ss.str(std::string());
        ss << "The assassination failed due to a player's challenge.\n";
        ss << "\n";
        GetPlayerConfirmation(ss.str());

    }
}

// Helper function for the "Coup" action
// A coup costs 7 and is ALWAYS successful in removing a player's influence (no challenge, etc).
void HandleCoup(Game * game, int playerId, ActionType actionType, int targetId) {
    std::stringstream ss;
    // The player has to pay to perform the action
    game->players[playerId]->money -= 7;

    ss << game->players[playerId]->playerName << " has chosen to coup " << game->players[targetId]->playerName << ".\n";

    ss << "The Coup was successful and "<<game->players[targetId]->playerName<<" lost an influence.\n";
    ss << "\n";
    GetPlayerConfirmation(ss.str());
    loseInfluence( game,  targetId);

}

//Helper function for the "Exchange" action
// It can be challenged.
// If allowed to continue, we double the amount of cards we have, and then
// replaced cards until we are back to the number we were before.
void HandleExchange(Game * game, int playerId, ActionType actionType, int targetId) {
    std::stringstream ss;

    ss << game->players[playerId]->playerName << " has chosen to exchange.\n";
    ss << "\n";
    ss << "Would you like to challenge " << game->players[playerId]->playerName << "'s Ambassador card?\n";

    // We check if anyone wants to challenge.
    // If nobody challenges or none are successful, flow continues.
    if(!CheckChallenge(game, playerId, Card::ambassador, ss.str())) {

        int cardsLeft = game->players[playerId]->cardsLeft;

        // We draw as many cards as we have
        for(int i = 0; i < cardsLeft; i++) game->players[playerId]->cards.push_back(game->deck->Draw());
        while(game->players[playerId]->cards.size() > cardsLeft){

            // Vector of all card names for cards the player names
            std::vector<std::string> cardNames;
            std::transform(game->players[playerId]->cards.begin(),game->players[playerId]->cards.end(),
                           std::back_inserter(cardNames),CardToString);


            ss.str(std::string());
            ss << "You have too many cards due to the effect of Ambassador.\n\n";
            ss << "Please select ("<< game->players[playerId]->cards.size() - cardsLeft <<") cards to be replaced.\n";
            int replaceIndex;
            // Player selects the card they want to replace
            if(playerId == 0){
                replaceIndex = GetPlayerVariableChoice(ss.str(),cardNames,false);
            } else {
                replaceIndex = GetReplaceCard(rand(),game->players[playerId]->cards);
            }

            // We add the card back into the deck
            game->deck->Replace(game->players[playerId]->cards[replaceIndex]);

            // We remove the card from the player's cards
            game->players[playerId]->cards.erase(game->players[playerId]->cards.begin() + replaceIndex);

        }


    }


}

//Helper function for the "Steal" action
// It can be challenged, or blocked by Ambassador or Captain (which can in turn be challenged)
// Steals $2 from another targeted player
void HandleSteal(Game * game, int playerId, ActionType actionType, int targetId) {
    std::stringstream ss;
    ss << game->players[playerId]->playerName << " has chosen to steal from " << game->players[targetId]->playerName << ".\n";
    ss << "\n";
    ss << "Would you like to challenge " << game->players[playerId]->playerName << "'s Captain card?\n";

    // We check if anyone wants to challenge.
    // If nobody challenges or none are successful, flow continues.
    if(!CheckChallenge(game, playerId, Card::captain, ss.str())){

        ss.str(std::string());
        ss << game->players[playerId]->playerName << " has chosen to steal from you.\n";
        ss << "\n";
        ss << "Would you like to block with another card?\n";

        //We check if target wants to block with a Captain/Ambassador.
        // If they do, we go into the challenge for Captain/Ambassador
        // If not, we finish assassination
        Card blockedCard = CheckBlock( game, targetId, actionType, ss.str(), {Card::ambassador,Card::captain});
        if(blockedCard == Card::ambassador || blockedCard == Card::captain){

            ss.str(std::string());
            ss << game->players[targetId]->playerName << " is blocking with a "<< CardToString(blockedCard)<<".\n";
            ss << "\n";
            ss << "Would you like to challenge " << game->players[targetId]->playerName << "'s "<< CardToString(blockedCard)<<" card?\n";

            // We check if anyone would like to challenge the defenders Captain/Ambassador card.
            // If there is no succesful challenge, the block is succesful and the stealing fails, and we exit the action flow.
            if(!CheckChallenge(game, targetId, blockedCard, ss.str())){
                ss.str(std::string());
                ss << "The stealing was blocked by a "<< CardToString(blockedCard)<<".\n";
                ss << "\n";
                GetPlayerConfirmation(ss.str());
                return;
            }
        }

        // Stealing successful.
        ss.str(std::string());
        ss << "The stealing was successful and "<<game->players[targetId]->playerName<<" lost $2 to "<<game->players[playerId]->playerName <<".\n";
        ss << "\n";
        GetPlayerConfirmation(ss.str());

        // As much money is stolen as possible, up to 2
        if(game->players[targetId]->money >= 2){
            game->players[targetId]->money -= 2;
            if(game->players[targetId]->money < 0) game->players[targetId]->money = 0;
            game->players[playerId]->money += 2;

        } else if (game->players[targetId]->money >= 1) {
            game->players[targetId]->money -= 1;
            if(game->players[targetId]->money < 0) game->players[targetId]->money = 0;
            game->players[playerId]->money += 1;
        }

    } else {
        ss.str(std::string());
        ss << "The stealing failed due to a player's challenge.\n";
        ss << "\n";
        GetPlayerConfirmation(ss.str());

    }


}

//Helper function for the "Income" action
// It cannot be challenged or blocked.
// Gain $1
void HandleIncome(Game * game, int playerId, ActionType actionType, int targetId) {
    std::stringstream ss;
    ss << game->players[playerId]->playerName << " has chosen Income, giving them an additional $1.";
    ss << "\n";

    game->players[playerId]->money += 1;

    GetPlayerConfirmation(ss.str());


}

//Helper function for the "Foreign Aid" action
// It can be blocked by an Ambassador.
// Gain $2
void HandleForeignAid(Game * game, int playerId, ActionType actionType, int targetId) {
    std::stringstream ss;
    ss << game->players[playerId]->playerName << " is calling for Foreign Aid.\n";
    ss << "\n";
    ss << "Would you like to block " << game->players[playerId]->playerName << "'s Foreign Aid with a Duke?\n";

    for(int i = 0; i < game->playerCount; i++){
        //A player cannot block themselves
        if(i == playerId) continue;

        if(CheckBlock( game, i, actionType, ss.str(), {Card::duke}) == Card::duke){

            ss.str(std::string());
            ss << game->players[i]->playerName << " is blocking with a Duke.\n";
            ss << "\n";
            ss << "Would you like to challenge " << game->players[i]->playerName << "'s Duke card?\n";

            // We check if anyone would like to challenge the defenders Duke card.
            // If there is no succesful challenge, the block is succesful and the foreign aid fails, and we exit the action flow.
            if(!CheckChallenge(game, i, Card::duke, ss.str())){
                ss.str(std::string());
                ss << "The Foreign Aid was blocked by a Duke.\n";
                ss << "\n";
                GetPlayerConfirmation(ss.str());
                return;
            }
        }
    }
    ss.str(std::string());
    ss << game->players[playerId]->playerName << "'s Foreign Aid was successful, giving them an additional $2.";
    ss << "\n";

    game->players[playerId]->money += 2;

    GetPlayerConfirmation(ss.str());

}


//Helper function for the "Tax" action
// It can be challenged.
// Gain $3
void HandleTax(Game * game, int playerId, ActionType actionType, int targetId) {
    std::stringstream ss;
    ss << game->players[playerId]->playerName << " is using Tax, with a Duke.\n";
    ss << "\n";
    ss << "Would you like to challenge " << game->players[playerId]->playerName << "'s Duke card?\n";
    // We check if anyone would like to challenge the defenders Duke card.
    // If there is no succesful challenge, the block is succesful and the foreign aid fails, and we exit the action flow.
    if(CheckChallenge(game, playerId, Card::duke, ss.str())){
        ss.str(std::string());
        ss << "The Tax was challenged and stopped.\n";
        ss << "\n";
        GetPlayerConfirmation(ss.str());
        return;
    }
    ss.str(std::string());

    ss << game->players[playerId]->playerName << "'s Duke was successful, giving them an additional $3.";
    ss << "\n";

    game->players[playerId]->money += 3;

    GetPlayerConfirmation(ss.str());
}

