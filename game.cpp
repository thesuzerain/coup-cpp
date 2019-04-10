//
// Created by Wyatt Verchere on 2019-04-08.
//

#include "game.h"

#include <iostream>

Game::Game(int playerCount){

    this->playerCount = playerCount;
    deck = new Deck();

    std::string names[] = {"Player", "Avery","Sam","Robin"};

    // Creates the opponents.
    // The player is ALWAYS player 0.
    for(int i = 0; i < playerCount; i++){
        players.push_back( new Player(deck->Draw(),deck->Draw(),names[i]));
        players[i]->myId = i;
    }


    //Main game loop.
    bool endGame = false;
    while(!endGame){
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
        players[id]->RunPlayerMain(this,false);

    } else {
        players[id]->RunOpponentMain(this);
    }



}