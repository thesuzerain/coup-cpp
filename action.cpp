//
// Created by Wyatt Verchere on 2019-04-08.
//

#include "action.h"
#include "ui.h"
#include "deck.h"
#include "ai.h"
#include "player.h"

#include <random>
#include <iostream>
#include <sstream>
#include <algorithm>    // std::transform



bool GetCanDoAction(const Player & player, ActionType actionType){

    switch(actionType){
        case ActionType::assassinate: return (player.money >= 3);
        case ActionType::coup : return (player.money >= 7);
        case ActionType::income: return (player.money < 10);
        case ActionType::foreign_aid: return (player.money < 10);
        case ActionType::tax: return (player.money < 10);
        case ActionType::steal: return (player.money < 10);
        case ActionType::exchange: return (player.money < 10);
        default: return false;
    }
}

void DoAction(Game & game, int playerId, ActionType actionType, int targetId) {
    // We delegate the handling of each possible action type to a helper function using a switch statement
    switch (actionType) {

        // Assassination
        case ActionType::assassinate:
            HandleAssassinate(game,playerId,targetId);
            break;

        // Coup
        case ActionType::coup :
            HandleCoup(game,playerId,targetId);
            break;

            // Income
        case ActionType::income:
            HandleIncome(game,playerId,targetId);
            break;

            // Foreign Aid
        case ActionType::foreign_aid:
            HandleForeignAid(game,playerId,targetId);
            break;

            //Tax
        case ActionType::tax:
            HandleTax(game,playerId,targetId);
            break;

            //Steal
        case ActionType::steal:
            HandleSteal(game,playerId,targetId);
            break;

            // Exchange
        case ActionType::exchange:
            HandleExchange(game,playerId,targetId);
            break;
        default:
            break;
    }
}

// Helper function for the "Assassinate" action
// It can be challenged, blocked by Contessa, and that blocking can be contested
void HandleAssassinate(Game & game, int playerId, int targetId) {
    ActionType actionType = ActionType::assassinate;
    std::stringstream ss;

    // Regardless of challenging and whether it goes through, the player has to pay to perform the action
    game.players[playerId]->money -= 3;

    ss << game.players[playerId]->playerName << " has chosen to assassinate " << game.players[targetId]->playerName << ".\n";
    ss << "\n";
    ss << "Would you like to challenge " << game.players[playerId]->playerName << "'s Assassin card?\n";

    // We check if anyone wants to challenge.
    // If nobody challenges or none are successful, flow continues.
    if(!TryChallenge(game, playerId, Card::assassin, ss.str())){

        ss.str(std::string());
        ss << game.players[playerId]->playerName << " has chosen to assassinate you.\n";
        ss << "\n";
        ss << "Would you like to block with a Contessa?\n";

        //We check if target wants to block with a Contessa.
        // If they do, we go into the challenge for Contessa
        // If not, we finish assassination
        if(TryBlock( game, targetId, actionType, ss.str(), {Card::contessa, Card::nullCard}) == Card::contessa){

            ss.str(std::string());
            ss << game.players[targetId]->playerName << " is blocking with a Contessa.\n";
            ss << "\n";
            ss << "Would you like to challenge " << game.players[targetId]->playerName << "'s Contessa card?\n";

            // We check if anyone would like to challenge the defenders Contessa card.
            // If there is no succesful challenge, the block is succesful and the assassination fails, and we exit the action flow.
            if(!TryChallenge(game, targetId, Card::contessa, ss.str())){
                ss.str(std::string());
                ss << "The assassination was blocked by a Contessa.\n";
                ss << "\n";
                GetPlayerConfirmation(ss.str());
                return;
            }
        }

        // Assassination successful.
        ss.str(std::string());
        ss << "The assassination was successful and "<<game.players[targetId]->playerName<<" lost an influence.\n";
        ss << "\n";
        GetPlayerConfirmation(ss.str());
        LoseInfluence( game,  targetId);
    } else {
        ss.str(std::string());
        ss << "The assassination failed due to a player's challenge.\n";
        ss << "\n";
        GetPlayerConfirmation(ss.str());
    }
}

// Helper function for the "Coup" action
void HandleCoup(Game & game, int playerId, int targetId) {
    ActionType actionType = ActionType::coup;

    std::stringstream ss;
    // A coup costs 7 and is ALWAYS successful in removing a player's influence (no challenge, etc).
    game.players[playerId]->money -= 7;

    ss << game.players[playerId]->playerName << " has chosen to coup " << game.players[targetId]->playerName << ".\n";
    ss << "The Coup was successful and "<<game.players[targetId]->playerName<<" lost an influence.\n";
    ss << "\n";
    GetPlayerConfirmation(ss.str());
    LoseInfluence( game,  targetId);
}

void HandleExchange(Game & game, int playerId, int targetId) {
    ActionType actionType = ActionType::exchange;

    std::stringstream ss;

    ss << game.players[playerId]->playerName << " has chosen to exchange.\n";
    ss << "\n";
    ss << "Would you like to challenge " << game.players[playerId]->playerName << "'s Ambassador card?\n";

    // We check if anyone wants to challenge.
    // If nobody challenges or none are successful, flow continues.
    if(!TryChallenge(game, playerId, Card::ambassador, ss.str())) {

        int cardsLeft = game.players[playerId]->cardsLeft;

        // We draw as many cards as we have
        for(int i = 0; i < cardsLeft; i++) game.players[playerId]->cards.push_back(game.deck->Draw());
        while(game.players[playerId]->cards.size() > cardsLeft){

            // Vector of all card names for cards the player names
            std::vector<std::string> cardNames;
            std::transform(game.players[playerId]->cards.begin(),game.players[playerId]->cards.end(),
                           std::back_inserter(cardNames),CardToString);

            ss.str(std::string());
            ss << "You have too many cards due to the effect of Ambassador.\n\n";
            ss << "Please select ("<< game.players[playerId]->cards.size() - cardsLeft <<") cards to be replaced.\n";
            int replaceIndex;

            // Player selects the card they want to replace
            if(playerId == 0){
                replaceIndex = GetPlayerVariableChoice(ss.str(),cardNames,false);
            } else {
                replaceIndex = GetReplaceCard(rand(),game.players[playerId]->cards);
            }

            // We add the card back into the deck
            game.deck->Replace(game.players[playerId]->cards[replaceIndex]);
            game.players[playerId]->cards.erase(game.players[playerId]->cards.begin() + replaceIndex);
        }
    }
}

//Helper function for the "Steal" action
void HandleSteal(Game & game, int playerId, int targetId) {
    ActionType actionType = ActionType::steal;

    std::stringstream ss;
    ss << game.players[playerId]->playerName << " has chosen to steal from " << game.players[targetId]->playerName << ".\n";
    ss << "\n";
    ss << "Would you like to challenge " << game.players[playerId]->playerName << "'s Captain card?\n";

    // We check if anyone wants to challenge.
    // If nobody challenges or none are successful, flow continues.
    if(!TryChallenge(game, playerId, Card::captain, ss.str())){

        ss.str(std::string());
        ss << game.players[playerId]->playerName << " has chosen to steal from you.\n";
        ss << "\n";
        ss << "Would you like to block with another card?\n";

        //We check if target wants to block with a Captain/Ambassador.
        // If they do, we go into the challenge for Captain/Ambassador
        // If not, we finish assassination
        Card blockedCard = TryBlock( game, targetId, actionType, ss.str(), {Card::ambassador,Card::captain, Card::nullCard});
        if(blockedCard == Card::ambassador || blockedCard == Card::captain){

            ss.str(std::string());
            ss << game.players[targetId]->playerName << " is blocking with a "<< CardToString(blockedCard)<<".\n";
            ss << "\n";
            ss << "Would you like to challenge " << game.players[targetId]->playerName << "'s "<< CardToString(blockedCard)<<" card?\n";

            // We check if anyone would like to challenge the defenders Captain/Ambassador card.
            // If there is no succesful challenge, the block is succesful and the stealing fails, and we exit the action flow.
            if(!TryChallenge(game, targetId, blockedCard, ss.str())){
                ss.str(std::string());
                ss << "The stealing was blocked by a "<< CardToString(blockedCard)<<".\n";
                ss << "\n";
                GetPlayerConfirmation(ss.str());
                return;
            }
        }

        // Stealing successful.
        ss.str(std::string());
        ss << "The stealing was successful and "<<game.players[targetId]->playerName<<" lost $2 to "<<game.players[playerId]->playerName <<".\n";
        ss << "\n";
        GetPlayerConfirmation(ss.str());

        // As much money is stolen as possible, up to 2
        if(game.players[targetId]->money >= 2){
            game.players[targetId]->money -= 2;
            if(game.players[targetId]->money < 0) game.players[targetId]->money = 0;
            game.players[playerId]->money += 2;

        } else if (game.players[targetId]->money >= 1) {
            game.players[targetId]->money -= 1;
            if(game.players[targetId]->money < 0) game.players[targetId]->money = 0;
            game.players[playerId]->money += 1;
        }

    } else {
        ss.str(std::string());
        ss << "The stealing failed due to a player's challenge.\n";
        ss << "\n";
        GetPlayerConfirmation(ss.str());

    }


}

//Helper function for the "Income" action
void HandleIncome(Game & game, int playerId, int targetId) {
    ActionType actionType = ActionType::income;

    std::stringstream ss;
    ss << game.players[playerId]->playerName << " has chosen Income, giving them an additional $1.";
    ss << "\n";

    game.players[playerId]->money += 1;

    GetPlayerConfirmation(ss.str());


}

//Helper function for the "Foreign Aid" action
void HandleForeignAid(Game & game, int playerId, int targetId) {
    ActionType actionType = ActionType::foreign_aid;

    std::stringstream ss;
    ss << game.players[playerId]->playerName << " is calling for Foreign Aid.\n";
    ss << "\n";
    ss << "Would you like to block " << game.players[playerId]->playerName << "'s Foreign Aid with a Duke?\n";

    for(int i = 0; i < game.playerCount; i++){
        //A player cannot block themselves
        if(i == playerId) continue;

        if(TryBlock( game, i, actionType, ss.str(), {Card::duke, Card::nullCard}) == Card::duke){

            ss.str(std::string());
            ss << game.players[i]->playerName << " is blocking with a Duke.\n";
            ss << "\n";
            ss << "Would you like to challenge " << game.players[i]->playerName << "'s Duke card?\n";

            // We check if anyone would like to challenge the defenders Duke card.
            // If there is no succesful challenge, the block is succesful and the foreign aid fails, and we exit the action flow.
            if(!TryChallenge(game, i, Card::duke, ss.str())){
                ss.str(std::string());
                ss << "The Foreign Aid was blocked by a Duke.\n";
                ss << "\n";
                GetPlayerConfirmation(ss.str());
                return;
            }
        }
    }
    ss.str(std::string());
    ss << game.players[playerId]->playerName << "'s Foreign Aid was successful, giving them an additional $2.";
    ss << "\n";

    game.players[playerId]->money += 2;

    GetPlayerConfirmation(ss.str());

}


//Helper function for the "Tax" action
void HandleTax(Game & game, int playerId, int targetId) {
    ActionType actionType = ActionType::tax;

    std::stringstream ss;
    ss << game.players[playerId]->playerName << " is using Tax, with a Duke.\n";
    ss << "\n";
    ss << "Would you like to challenge " << game.players[playerId]->playerName << "'s Duke card?\n";
    // We check if anyone would like to challenge the defenders Duke card.
    // If there is no succesful challenge, the block is succesful and the foreign aid fails, and we exit the action flow.
    if(TryChallenge(game, playerId, Card::duke, ss.str())){
        ss.str(std::string());
        ss << "The Tax was challenged and stopped.\n";
        ss << "\n";
        GetPlayerConfirmation(ss.str());
        return;
    }
    ss.str(std::string());

    ss << game.players[playerId]->playerName << "'s Duke was successful, giving them an additional $3.";
    ss << "\n";

    game.players[playerId]->money += 3;

    GetPlayerConfirmation(ss.str());
}


// We perform a challenge, where one player calls another players bluff
// Whoever loses the challenge loses a card
// boolean returned is on challenge success (challenger wins)
bool DoChallenge(Game& game, int challengerId, int defenderId, Card challengeCard) {

    // Defender wins the challenge, offense loses a card, defense reshuffles in revealed card.
    if (game.players[defenderId]->HasCard(challengeCard)) {

        std::stringstream ss;
        ss << "The challenging player, " << game.players[challengerId]->playerName << ", lost the challenge.\n";
        ss << game.players[defenderId]->playerName + " must shuffle back in the challenged card and redraw.\n\n";
        ss << game.players[challengerId]->playerName << " must select an influence to lose.";

        GetPlayerConfirmation(ss.str());

        // Reshuffle in challenged card (as other players know about it now)
        if (game.players[defenderId]->cards[0] == challengeCard) {
            game.deck->Replace(challengeCard);
            game.players[defenderId]->cards[0] = game.deck->Draw();
        }
        else {
            game.deck->Replace(challengeCard);
            game.players[defenderId]->cards[1] = game.deck->Draw();
        }

        LoseInfluence(game, challengerId);

        return false;
    }
    else {

        // Challenger wins the challenge, defense loses a card

        std::stringstream ss;
        ss << "The challenging player, " << game.players[challengerId]->playerName << ", won the challenge.\n\n";
        ss << game.players[defenderId]->playerName << " must select an influence to lose.";
        GetPlayerConfirmation(ss.str());
        LoseInfluence(game, defenderId);

        return true;
    }

}

// For one reason or another, a player must sacrifice an influence.
// If they still have two remaining, they can choose which one to lose
void LoseInfluence(Game& game, int playerId) {

    // If the cards are the same, or if there is only one card, it does not matter which card is lost
    if (game.players[playerId]->cardsLeft < 2 ||
        (game.players[playerId]->cards[0] == game.players[playerId]->cards[1])) {
        game.players[playerId]->cardsLeft -= 1;
        if (game.players[playerId]->cardsLeft < 0) game.players[playerId]->cardsLeft = 0;
        return;
    }

    int cardToLose = 0;

    if (playerId == 0) {
        std::string str = "Which of your two cards would you rather lose?\n";
        std::vector<std::string> cards = { CardToString(game.players[playerId]->cards[0]),CardToString(game.players[playerId]->cards[1]) };

        // Player selects a card to lose manually
        cardToLose = GetPlayerVariableChoice(str, cards, false);

    }
    else {

        // Ai randomly selects a card to lose
        cardToLose = GetReplaceCard(rand(), game.players[playerId]->cards);
    }

    // We swap the card values. The last ones are removed first.
    // (We do this because they keep the cards, they are merely revealed now)
    if (cardToLose == 0) {
        std::swap(game.players[playerId]->cards[0], game.players[playerId]->cards[1]);
    }
    game.players[playerId]->cardsLeft -= 1;


    if (game.players[playerId]->cardsLeft < 0) game.players[playerId]->cardsLeft = 0;



}

// Called when the player or an opponent is allowed to perform a challenge
// Text describes the scenario to be presented in the UI
// Returns true if a challenge occurs, and is subsequently successful
bool TryChallenge(Game & game, int playerId, Card card, std::string text) {

    // The player is only allowed to challenge if it is not their own card
    if (playerId != 0) {

        if (GetPlayerYesNoChoice(text, false)) {
            return DoChallenge(game, 0, playerId, card);
        }
    }

    // If the attacker is the player, an AI is the only one who can challenge it
    // (They are also allowed to challenge another opponent)
    for (int i = 1; i < game.playerCount; i++) {
        if (game.players[i]->cardsLeft <= 0) continue; // An opponent can't challenge if they are out of the game
        if (playerId == i) continue; // A opponent can't challenge thesmselves

        // We use the AI to select an opponents choice
        if (GetChallengeChoice(rand()) == 0) {

            std::stringstream ss;
            ss << "\n" << game.players[i]->playerName << " is choosing to challenge.\n";
            GetPlayerConfirmation(ss.str());
            return (DoChallenge(game, i, playerId, card));
        }

    }
    return false;
}

// Called when the player or an opponent is allowed to perform a block
// Returns the card they choose to block with, or Card::nullCard if they do not block (ie: nullCard is null)
Card TryBlock(Game& game, int target, ActionType actionType, std::string text, const std::vector<Card>& blockCards) {

    std::stringstream ss;
    ss << text << "\n";

    for (int i = 0; i < game.playerCount; i++)
    {
        game.players[i]->GetPlayerInfoString(false);
    }
    text = ss.str();

    if (target == 0) {

        std::vector<std::string> blockCardNames;
        std::transform(blockCards.begin(), blockCards.end(),
            std::back_inserter(blockCardNames), CardToString);

        return blockCards[GetPlayerVariableChoice(text, blockCardNames, false)];
    }
    else {
        // We use the AI to select a players choice
        return GetBlockChoice(rand(), *game.players[target], blockCards);
    }
}
