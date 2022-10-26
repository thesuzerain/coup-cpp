//
// Created by Wyatt Verchere on 2019-04-08.
//

#include "game.h"
#include "ui.h"
#include "action.h"
#include "ai.h"

#include <iostream>
#include <sstream>

Game::Game(std::vector<std::string> playerNames){

    this->playerCount = playerNames.size();
    deck = std::make_unique<Deck>();

    // Creates the opponents.
    // The player is ALWAYS player 0.
    for(int i = 0; i < playerCount; i++){
        players.push_back( std::make_shared<Player>(deck->Draw(),deck->Draw(), playerNames[i]));
        players[i]->myId = i;
    }

    //Main game loop.
    bool endGame = false;
    while(!endGame){
        std::cout << playerCount << std::endl;
        for(int i = 0; i < playerCount; i++){

            // Skip this players turn if they are out
            if(players[i]->cardsLeft <= 0) continue;

            RunTurn(i);

            endGame = true;
            // If any other opponent has cards left, game is won yet!
            for(int j = 1; j < playerCount; j++){
                if(players[j]->cardsLeft > 0) endGame = false;
            }

            // Player has no cards left, game is over.
            if(players[0]->cardsLeft <= 0){
                endGame = true;
            }

            if(endGame) break;
        }
    }

    //Game is over!
    if(players[0]->cardsLeft > 0){
        std::cout << "Congratulations! You are the only surviving player and have won!" << std::endl;
    } else {
        std::cout << "Unfortunately, you have been knocked out. Please try again!"  << std::endl;
    }

}

void Game::RunTurn(int id){

    // If the player out of cards, they are out of the game and we skip them
    if(players[id]->cardsLeft == 0) return;

    if(id == 0){
        RunPlayerMain(false);

    } else {
        RunOpponentMain(id);
    }
}

void Game::RunPlayerMain(bool lastCommandInvalid) {

    // We maintain stream ss throughout the function, and print it into the frame when it is ready
    std::stringstream ss;

    // Display player's name and spaces it out
    ss << players[0]->playerName << "'s turn!" << std::endl << std::endl;;

    // Print out the legal actions we can take
    ss << players[0]->GetPlayerActionsString() << std::endl;

    // We print out information about each player,
    for (int i = 0; i < playerCount; i++) {
        ss << players[i]->GetPlayerInfoString(false);
    }

    if (lastCommandInvalid) {
        ss << std::endl <<"Invalid action.";
    }
    else {
        ss << std::endl << "Please enter an action.";
    }

    PrintNewFrame(ss.str());

    ActionType actionChoice = GetPlayerActionChoice();
    if (actionChoice == ActionType::bad_action || !GetCanDoAction(*players[0], actionChoice)) {
        RunPlayerMain(true);
    }
    else {

        // If the action we are doing involves another player (ie: an attack)
        // we have to first select that player. Otherwise, we perform the action right away.
        if (actionChoice == ActionType::assassinate ||
            actionChoice == ActionType::steal ||
            actionChoice == ActionType::coup) {
            int playerChoice = GetPlayerTargetChoice(*this, false);
            DoAction(*this, 0, actionChoice, playerChoice);
        }
        else {
            DoAction(*this, 0, actionChoice, 0); // we do 0 for the target as it doesn't matter
        }
    }
}


// The main part of each's opponent's turn.
// ie: what action they are going to choose.
void Game::RunOpponentMain(int id) {
    Player player = *players[id];

    // We select an attack target, whether or not we intend to use it
    int target = GetTarget(rand(), *this, id);

    // If we have at least $10, we must coup
    if (player.money >= 10) {
        DoAction(*this, id, ActionType::coup, target);
    }
    else {
        ActionType action = PickAction(rand(), *this, player);
        DoAction(*this, id, action, target);
    }
}

