//
// Created by Wyatt Verchere on 2019-04-08.
//

#ifndef COUP_CPP_UI_H
#define COUP_CPP_UI_H

#include <iostream>
#include <vector>
#include "card.h"
#include "game.h"
#include "actiontype.h"

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

void PrintNewFrame(const std::string & text);

// If allowable on this terminal, colorizes the text in 'text' with Color 'color'.
// Does not work on most Windows machines.
std::string ColorizeText(const std::string & text, Color color);

// Allows terminal input to determine what action the live (id=0) player wants to take (from all legal actions).
// Returns corresponding actiontype;
ActionType GetPlayerActionChoice();

// Allows terminal input to determine what target the live (id=0) player wants to select.
// Returns index of target.
int GetPlayerTargetChoice(const Game & game, bool lastCommandInvalid);

// Allows terminal input to handle confirmation.
// Returns true if yes, false if no.
bool GetPlayerYesNoChoice(const std::string & text, bool lastCommandInvalid);

// Allows terminal input to choose from a list of possible options ('options').
// Returns index of chosen selection from the options.
int GetPlayerVariableChoice(const std::string & text, const std::vector<std::string> & options, bool lastCommandInvalid);

// Allows blocking display text without parsing response.
// Returns true if success.
bool GetPlayerConfirmation(const std::string & text);

#endif //COUP_CPP_UI_H
