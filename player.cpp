//
// Created by Wyatt Verchere on 2019-04-08.
//

#include "player.h"
#include "ui.h"
#include "action.h"
#include "ai.h"

#include<iomanip>
#include <random>
#include <sstream>



// Initializes a player to $2, 2 cards
Player::Player(Card card1, Card card2, std::string name){
    srand(time(NULL));
    cardsLeft = 2;
    cards = {card1,card2};
    money = 2;
    playerName = name;

}

// We print out all the possible actions the player can take
// Each action is printed black if it doesn't require a card, green if it does and they have it
// and red if it requires a card they don't have (they can still perform the action, but will need to bluff)
std::string Player::GetPlayerActionsString() {
    std::stringstream ss;

    if (money >= 10) {
        // If the player has more than $10, they MUST do a coup
        ss << "(6) Coup (-$7)";
    }
    else {
        ss << "(0) Income (+$1)\t\t\t(1) Foreign Aid (+$2)\n";

        ss << ColorizeText("(2) ", (HasCard(Card::duke) ? Color::GREEN : Color::RED)) << "Tax (+$3)\t\t\t\t";
        ss << ColorizeText("(3) ", (HasCard(Card::ambassador) ? Color::GREEN : Color::RED)) << "Exchange\n";

        ss << ColorizeText("(4) ", (HasCard(Card::captain) ? Color::GREEN : Color::RED)) << "Steal (+$2)\t\t\t\t";

        // The player cannot safely assassinate or coup (at all) unless they have the needed $
        if (money >= 3) {
            ss << ColorizeText("(5) ", (HasCard(Card::assassin) ? Color::GREEN : Color::RED)) << "Assassinate (-$3)\n";

            if (money >= 7) {
                ss << "(6) Coup (-$7)\n";
            }
        }
        else {
            ss << "\n";
        }
    }
    return ss.str();
}

// We create a display string that prints out info about this player.
std::string Player::GetPlayerInfoString( bool attacking) {

    std::stringstream ss;
    int textAlignmentSize = 20;

    // If player is out, display 'defeated' instead
    if (cardsLeft <= 0) {
        ss << std::left << std::setfill(' ') << std::setw(textAlignmentSize);
        ss << playerName << std::endl;
        return ss.str();
    }


    // Each player's name and their current monetary worth
    ss << playerName << ":\t\t$" << money;
    ss << std::right << std::setfill(' ') << std::setw(textAlignmentSize); // filler for text alignment


    // We reveal their first card only if the player is out of the game, or it is owned by the player
    if (myId == 0 || cardsLeft < 1) {
        ss << CardToString(cards[0]);
    }
    else {
        ss << "?????????";
    }
    ss << std::right << std::setfill(' ') << std::setw(textAlignmentSize); // filler for text alignment

    // We reveal the second card if that player has revealed it, or it is owned by the player.
    // We add the "revealed" keyword to show that it is seen by all players
    if (myId == 0 && cardsLeft < 2) {
        ss << CardToString(cards[1]) << "(revealed)";
    }
    else {
        if (myId == 0 || cardsLeft < 2) {
            ss << CardToString(cards[1]);
        }
        else {
            ss << "?????????";
        }
    }
    ss << std::right << std::setfill(' ') << std::setw(textAlignmentSize); // filler for text alignment

    // To make it easier and understandable, we also write down how many cards they have left
    ss << "Cards:" << cardsLeft;
    ss << "\n";
    ss << std::right << std::setfill(' ') << std::setw(textAlignmentSize); // filler for text alignment

    return ss.str();

}

// Quickly checks if the player has a certain card (used for bluffing, etc)
bool Player::HasCard(Card card) const {
    return (cards[0] == card || (cards[1] == card && cardsLeft >= 2)) ;

}

