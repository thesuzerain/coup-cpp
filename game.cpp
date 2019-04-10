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
    }

}

void Game::RunTurn(int id){

    // If the player out of cards, they are out of the game and we skip them
    if(players[id]->cardsLeft == 0) return;

    if(id == 0)
        players[id]->RunPlayerMain(this,false);



}