//
// Created by Wyatt Verchere on 2019-04-08.
//

#include "action.h"
#include "ui.h"
#include "deck.h"
#include "ai.h"

#include <random>
#include <iostream>
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

// Called when the player or an opponent is allowed to perform a challenge
// Text describes the scenario to be presented in the UI
// Returns true if the challenge is succesful and the action is subsequently nullified

bool CheckChallenge(Game * game, int playerId, ActionType actionType, int targetId, Card card, std::string text){
    // The player is allowed to challenge the assassinators bluff
    if(playerId != 0){

        // If the attacker is an opponent, the player is allowed to challenge it
        // TODO: other opponents should still be able to challenge other opponents if the player doesn't
        // TODO: should be an easy restructural change of this if statement
        if(GetPlayerYesNoChoice(text,false)){
            if(DoChallenge(game,0,playerId,card)) return true;
        }
    } else {

        // If the attacker is the player, an AI is the only one who can challenge it it.
        for(int i = 1; i < game->playerCount; i++){
            if(game->players[i]->cardsLeft <= 0) return true; // A player can't challenge if they are out of the game!

            // We use the AI to select a players choice
            if(GetChallengeChoice(rand()) == 0){

                std::stringstream ss;
                ss << "\n" <<game->players[i]->playerName << " is choosing to challenge.\n";
                GetPlayerConfirmation(ss.str());
                return (DoChallenge(game,i,playerId,card));
            }

        }
    }
    return false;
}

// Called when the player or an opponent is allowed to perform a block
// Returns the card they choose to block with, or Card::numberOfCards if they do not block (ie: numberOfCards is null)
Card CheckBlock(Game * game, int target, ActionType actionType, std::string text, std::vector<Card> blockCards){
    if(target == 0){

        std::vector<std::string> blockCardNames;
        std::transform(blockCards.begin(),blockCards.end(),
                       std::back_inserter(blockCardNames),CardToString);

        blockCards.push_back(Card::numberOfCards);
        blockCardNames.push_back("Do not block.");

        // The player is allowed to attempt to block with this card
        return blockCards[GetPlayerVariableChoice(text,blockCardNames,false)];
    } else {
        // We use the AI to select a players choice
        return GetBlockChoice(rand(),game->players[target],blockCards);
    }
}


// Performs an action of the players choice
// A switch statement handles different behaviours for different actions
void DoAction(Game * game, int playerId, ActionType actionType, int targetId) {

    srand(time((NULL)));

    // We use this stream for the text being sent to frames
    std::stringstream ss;

    switch (actionType) {

        // If the player assassinates, it can be challenged, blocked by Contessa, and that blocking can be contested
        case ActionType::assassinate:

            // Regardless of challenging and whether it goes through, the player has to pay to perform the action
            game->players[playerId]->money -= 3;

            ss << game->players[playerId]->playerName << " has chosen to assassinate " << game->players[targetId]->playerName << ".\n";
            ss << "\n";
            ss << "Would you like to challenge " << game->players[playerId]->playerName << "'s Assassin card?\n";

            // We check if anyone wants to challenge.
            // If nobody challenges or none are successful, flow continues.
            if(!CheckChallenge(game, playerId, actionType, targetId, Card::assassin, ss.str())){

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
                    if(!CheckChallenge(game, targetId, actionType, playerId, Card::contessa, ss.str())){
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

            break;

            // A coup costs 7 and is ALWAYS successful in removing a player's influence.
        case ActionType::coup :
            // The player has to pay to perform the action
            game->players[playerId]->money -= 7;

            ss << game->players[playerId]->playerName << " has chosen to coup " << game->players[targetId]->playerName << ".\n";

            ss << "The Coup was successful and "<<game->players[targetId]->playerName<<" lost an influence.\n";
            ss << "\n";
            GetPlayerConfirmation(ss.str());
            loseInfluence( game,  targetId);
            break;

        // Income has no inter-player interaction, so $1 is added and flow completes.
        case ActionType::income:

            ss << game->players[playerId]->playerName << " has chosen Income, giving them an additional $1.";
            ss << "\n";

            game->players[playerId]->money += 1;

            GetPlayerConfirmation(ss.str());
            break;

         // Any player is allowed to try and block foreign aid with a Duke
         // If not, gain $2
        case ActionType::foreign_aid:

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
                    if(!CheckChallenge(game, i, actionType, 32323232, Card::duke, ss.str())){
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
            break;


        case ActionType::tax:
            ss << game->players[playerId]->playerName << " is using Tax, with a Duke.\n";
            ss << "\n";
            ss << "Would you like to challenge " << game->players[playerId]->playerName << "'s Duke card?\n";
                    // We check if anyone would like to challenge the defenders Duke card.
                    // If there is no succesful challenge, the block is succesful and the foreign aid fails, and we exit the action flow.
            if(CheckChallenge(game, playerId, actionType, 32323232, Card::duke, ss.str())){
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
            break;

        case ActionType::steal:


            ss << game->players[playerId]->playerName << " has chosen to steal from " << game->players[targetId]->playerName << ".\n";
            ss << "\n";
            ss << "Would you like to challenge " << game->players[playerId]->playerName << "'s Captain card?\n";

            // We check if anyone wants to challenge.
            // If nobody challenges or none are successful, flow continues.
            if(!CheckChallenge(game, playerId, actionType, targetId, Card::captain, ss.str())){

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
                    if(!CheckChallenge(game, targetId, actionType, playerId, blockedCard, ss.str())){
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

                game->players[targetId]->money -= 2;
                game->players[playerId]->money += 2;

            } else {
                ss.str(std::string());
                ss << "The assassination failed due to a player's challenge.\n";
                ss << "\n";
                GetPlayerConfirmation(ss.str());

            }

            break;

        // First, anyone is allowed to challenge the Ambassador.
        // If allowed to continue, we double the amount of cards we have, and then
        // replaced cards until we are back to the number we were before.
        case ActionType::exchange:

            ss << game->players[playerId]->playerName << " has chosen to exchange.\n";
            ss << "\n";
            ss << "Would you like to challenge " << game->players[playerId]->playerName << "'s Ambassador card?\n";

            // We check if anyone wants to challenge.
            // If nobody challenges or none are successful, flow continues.
            if(!CheckChallenge(game, playerId, actionType, targetId, Card::ambassador, ss.str())) {

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

                    // Player selects the card they want to replace
                    int replaceIndex = GetPlayerVariableChoice(ss.str(),cardNames,false);

                    // We add the card back into the deck
                    game->deck->Replace(game->players[playerId]->cards[replaceIndex]);

                    // We remove the card from the player's cards
                    game->players[playerId]->cards.erase(game->players[playerId]->cards.begin() + replaceIndex);

                }


            }

            break;
        default:
            break;
    }
}

// We perform a challenge, where one player calls another players bluff
// Whoever loses the challenge loses a card
// boolean returned is whether it was successful (and the challenger wins)
bool DoChallenge(Game * game, int challengerId, int defenderId, Card challengeCard){

    // Defender wins the challenge, offense loses a card, defense reshuffles in revealed card.
    if(game->players[defenderId]->HasCard(challengeCard)){


        std::stringstream ss;
        ss << "The challenging player, " << game->players[challengerId]->playerName << ", lost the challenge.\n";
        ss << game->players[defenderId]->playerName+" must shuffle back in the challenged card and redraw.\n\n";
        ss << game->players[challengerId]->playerName << " must select an influence to lose.";

        GetPlayerConfirmation(ss.str());

        // Reshuffle in challenged card (as other players know about it now)
        if(game->players[defenderId]->cards[0] == challengeCard){
            game->deck->Replace(challengeCard);
            game->players[defenderId]->cards[0] = game->deck->Draw();
        } else {
            game->deck->Replace(challengeCard);
            game->players[defenderId]->cards[1] = game->deck->Draw();
        }

        loseInfluence( game,  challengerId);

        return false;
    } else {

        // Challenger wins the challenge, defense loses a card

        std::stringstream ss;
        ss << "The challenging player, " << game->players[challengerId]->playerName << ", won the challenge.\n\n";
        ss << game->players[defenderId]->playerName << " must select an influence to lose.";
        GetPlayerConfirmation(ss.str());
        loseInfluence( game,  defenderId);

        return true;
    }

}

// For one reason or another, a player must sacrifice an influence.
// If they still have two remaining, they can choose which one to lose
void loseInfluence(Game * game, int playerId){

    // If the cards are the same, or if there is only one card, it does not matter which card is lost
    if( game->players[playerId]->cardsLeft < 2 ||
            (game->players[playerId]->cards[0] == game->players[playerId]->cards[1])){
        game->players[playerId]->cardsLeft -= 1;
        return;
    }

    int cardToLose = 0;

    if(playerId == 0){
        std::string str = "Which of your two cards would you rather lose?\n";

        std::vector<std::string> cards = {CardToString(game->players[playerId]->cards[0]),CardToString(game->players[playerId]->cards[1])};

        // We ask the player!
        if(GetPlayerVariableChoice(str,cards,false)) cardToLose = 1;

    } else {
        // TODO: AI
        cardToLose = GetRandomChoice(rand(), 2);
    }

    // We swap the card values. The last ones are removed first.
    // (We do this because they keep the cards, they are merely revealed now)
    if(cardToLose == 1){
        std::swap(game->players[playerId]->cards[0],game->players[playerId]->cards[1]);
    }
    game->players[playerId]->cardsLeft -= 1;



}