//
// Created by Wyatt Verchere on 2019-04-10.
//

#include "action_helper.h"

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
        cardToLose = GetPlayerVariableChoice(str,cards,false);

    } else {
        cardToLose = GetReplaceCard(rand(), game->players[playerId]->cards);
    }

    // We swap the card values. The last ones are removed first.
    // (We do this because they keep the cards, they are merely revealed now)
    if(cardToLose == 0){
        std::swap(game->players[playerId]->cards[0],game->players[playerId]->cards[1]);
    }
    game->players[playerId]->cardsLeft -= 1;



}

// Called when the player or an opponent is allowed to perform a challenge
// Text describes the scenario to be presented in the UI
// Returns true if the challenge is succesful and the action is subsequently nullified

bool CheckChallenge(Game * game, int playerId, ActionType actionType, int targetId, Card card, std::string text){
    // The player is allowed to challenge the cardds bluff
    if(playerId != 0){

        // If the attacker is an opponent, the player is allowed to challenge it
        if(GetPlayerYesNoChoice(text,false)){
            return DoChallenge(game,0,playerId,card);
        }
    }

    // If the attacker is the player, an AI is the only one who can challenge it
    // (They are also allowed to challenge another opponent)
    for(int i = 1; i < game->playerCount; i++){
        if(game->players[i]->cardsLeft <= 0) continue; // A player can't challenge if they are out of the game!
        if(playerId == i) continue; // A player can't challenge thesmselves!

        // We use the AI to select a players choice
        if(GetChallengeChoice(rand()) == 0){

            std::stringstream ss;
            ss << "\n" <<game->players[i]->playerName << " is choosing to challenge.\n";
            GetPlayerConfirmation(ss.str());
            return (DoChallenge(game,i,playerId,card));
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