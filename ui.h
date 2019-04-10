//
// Created by Wyatt Verchere on 2019-04-08.
//

#ifndef COUP_CPP_UI_H
#define COUP_CPP_UI_H

#include <iostream>
#include <vector>
#include "main.h"
#include "player.h"
#include "card.h"

enum Color {
    RESET       = 0,
    RED      = 31,
    GREEN    = 32,
    YELLOW    = 33,
    BLUE     = 34,
    MAGENTA     = 35,
    CYAN     = 36,
    WHITE     = 37
};

std::string CardToString(Card card);

void PrintNewFrame(std::string text);
std::string Colorize(std::string text, Color color);

ActionType GetPlayerActionChoice();
int GetPlayerTargetChoice(Game * game, bool lastCommandInvalid);
bool GetPlayerYesNoChoice(std::string text, bool lastCommandInvalid);
int GetPlayerVariableChoice(std::string text, std::vector<std::string> options, bool lastCommandInvalid);
bool GetPlayerConfirmation(std::string text);

class ui {


};

#endif //COUP_CPP_UI_H
