//
// Created by Wyatt Verchere on 2019-04-08.
//

#include "player.h"
#include "ui.h"
#include "action.h"

#include <sstream>


Player::Player(Card card1, Card card2, std::string name){

    cardsLeft = 2;
    cards = {card1,card2};

    money = 9;

    playerName = name;

}




// We print out information about each player.

std::string GetPlayersInfoString(Game * game, bool attacking){
    std::stringstream ss;
    for(int i = 0; i < game->playerCount; i++){

        // If we are choosing to attack a player, we give them reference numbers
        if(attacking) ss << "("<<i<<") ";

        // Each player's name and their current monetary worth
        ss << game->players[i]->playerName << ":\t\t$" << game->players[i]->money;



        //TODO: remove debug
        // We only reveal the cards owned by a player if it is owned by the true player (us) or
        // owned by an opponent who has revealed it/lost a card.


        /*
        // We reveal their first card only if the player is out of the game, or it is owned by the player
        if(i == 0 || game->players[i]->cardsLeft < 1){
            ss << "\t\t"<<CardToString(game->players[i]->cards[0]);
            if (game->players[i]->cardsLeft < 1) ss << "(revealed)";
        } else {
            ss << "\t\t"<<"??????";
        }

        // We reveal the second card if that player has revealed it, or it is owned by the player.
        // We add the "revealed" keyword to show that it is seen by all players
        if(i == 0 && game->players[i]->cardsLeft < 2){
            ss << "\t\t"<<CardToString(game->players[i]->cards[1]) << "(revealed)";

        } else {
            if(i == 0 || game->players[i]->cardsLeft < 2){
                ss << "\t\t"<<CardToString(game->players[i]->cards[1]);
                if (game->players[i]->cardsLeft < 1) ss << "(revealed)";

            } else {
                ss << "\t\t"<<"??????";
            }

        }

        ss << "\n";
*/
        ss << "\t\t"<<CardToString(game->players[i]->cards[0]);
        ss << "\t\t"<<CardToString(game->players[i]->cards[1])<<"\n";


    }
    return ss.str();
}

// Quickly checks if the player has a certain card (used for bluffing, etc)
bool Player::HasCard(Card card){
    return (cards[0] == card || (cards[1] == card && cardsLeft >= 2)) ;

}

// The main part of the player's turn.
// ie: what action they are going to choose.
void Player::RunPlayerMain(Game * game, bool lastCommandInvalid) {


    // We maintain stream ss throughout the function, and print it into the frame when it is ready
    std::stringstream ss;

    ss << playerName << "'s turn!" << "\n";
    ss << "\n";


    // We print out all the possible actions for each player
    // Each action is printed black if it doesn't require a card, green if it does and they have it
    // and red if it requires a card they don't have (they can still perform the action, but will need to bluff)

    if(money >= 10){
        // If the player has more than $10, they MUST do a coup
        ss << "(0) Coup (-$7)";

    } else {
        ss << "(0) Income (+$1)\t\t\t(1) Foreign Aid (+$2)\n";

        ss << Colorize("(2) ",(HasCard(Card::duke) ? Color::GREEN : Color::RED)) << "Tax (+$3)\t\t\t\t";
        ss << Colorize("(3) ",(HasCard(Card::ambassador) ? Color::GREEN : Color::RED)) << "Exchange\n";

        ss << Colorize("(4) ",(HasCard(Card::captain) ? Color::GREEN : Color::RED)) << "Steal (+$2)\t\t\t\t";

        // The player cannot assassinate or coup unless they have the needed $
        if(money >= 3){
            ss << Colorize("(5) ",(HasCard(Card::assassin) ? Color::GREEN : Color::RED)) << "Assassinate (-$3)\n";

            if(money >= 7){
                ss << "(6) Coup (-$7)\n";

            }
        } else {
            ss << "\n";
        }
    }

    ss << "\n";

    // We print out information about each player,
    ss << GetPlayersInfoString(game,false);


    if(lastCommandInvalid){
        ss << "\nInvalid action.";
    } else {
        ss << "\nPlease enter an action.";

    }
    PrintNewFrame(ss.str());

    ActionType actionChoice = GetPlayerActionChoice();
    if(actionChoice == ActionType::bad_action || !CanDoAction(this,actionChoice)){
        RunPlayerMain(game,true);
    } else {

        // If the action we are doing involves another player (ie: an attack)
        // we have to first select that player. Otherwise, we perform the action right away.
        if(actionChoice == ActionType::assassinate ||
        actionChoice == ActionType::steal ||
        actionChoice == ActionType::coup){
            int playerChoice = GetPlayerTargetChoice(game,false);
            DoAction(game,0,actionChoice,playerChoice);
        } else {
            DoAction(game,0,actionChoice,0); // we do 0 for the target as it doesn't matter
        }


    }


}


// The main part of each's opponent's turn.
// ie: what action they are going to choose.

void Player::RunOpponentMain() {}

