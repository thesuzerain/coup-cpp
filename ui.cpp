//
// Created by Wyatt Verchere on 2019-04-08.
//

#include "ui.h"

# include <sstream>


bool WINDOWS = false;
#ifdef _WIN32
WINDOWS = true;
#endif


// Prints out the card name in its designated color
std::string CardToString(Card card){
    switch (card){
        case duke: return Colorize("Duke",Color::MAGENTA);
        case captain: return Colorize("Captain",Color::BLUE);
        case ambassador: return Colorize("Ambassador",Color::GREEN);
        case assassin: return Colorize("Assassin",Color::RESET);
        case contessa: return Colorize("Contessa",Color::RED);
        default: return Colorize("??????",Color::RESET);

    }
}

void PrintNewFrame(std::string text){

    // Creates a new frame in the UI.

    // Clears the previous text if the user is on a linux-based system
    if (!WINDOWS) std::cout << "\033[2J\033[1;1H" << std::endl;

    // Creates an outline around the text
    std::cout << "=========================" << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << text << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << "=========================" << std::endl;

}

std::string Colorize(std::string text, Color color) {

    // If the user is on a linux-based terminal that accepts these escape characters (I don't think win32 does),
    // colorizes the text with the given color.

    if (!WINDOWS) {
        std::stringstream colorText;
        colorText << "\x1B[" << color << "m" << text << "\x1B[" << RESET << "m";

        return colorText.str();
    }

    return text;
}

//TODO
ActionType GetPlayerActionChoice(){
    std::string input;
    getline(std::cin,input);
     // If it is bigger than just the 1 number for the command, then it needs to be a help command.
    if(input.length() > 1){
        if(input.length() > 4 && input.substr(0,4) == "help "){
            int help = std::stoi(input.substr(5,5));

            //TODO: add help functionality
            return ActionType ::bad_action;

        } else {
            return ActionType::bad_action;
        }
    } else {
        try{
            int action = std::stoi(input);

            return static_cast<ActionType>(action );

        } catch (std::exception &e){
            return ActionType ::bad_action;

        }
    }


}

// This function is used for actions such as Steal, Coup, and Assassinate
// A target is selected by the player
// It cannot be themselves and it must be a player in the game
int GetPlayerTargetChoice(Game * game, bool lastCommandInvalid){

    // We print out the available options in a frame for the player to see
    std::stringstream ss;
    ss << GetPlayersInfoString(game,true);
    ss << "\n";
    if(lastCommandInvalid){
        ss << "Invalid attacking target.\n";
    } else {
        ss << "Who would you like to attack?\n";

    }
    PrintNewFrame(ss.str());


    //We get an input.
    std::string input;
    getline(std::cin,input);


    // We check for sanity (should only be one digit).
    if(input.length() <= 1){
        try{

            //Should only be an integer that is NOT 0 (the player using the attack) and is a legal player
            int action = std::stoi(input);
            if(action == 0 || action >= game->playerCount){
                return GetPlayerTargetChoice(game, true);
            } else {
                return action;
            }

        } catch(std::exception &e) {
            return GetPlayerTargetChoice(game, true);
        }
    } else {
        return GetPlayerTargetChoice(game, true);
    }

}

bool GetPlayerYesNoChoice(std::string text, bool lastCommandInvalid){
    std::vector<std::string> opts = {"Yes","No"};
    return GetPlayerVariableChoice( text, opts,  lastCommandInvalid) == 0;
}


// Returns the index of the string selected from the "opts" list
int GetPlayerVariableChoice(std::string text, std::vector<std::string> opts, bool lastCommandInvalid){

    // We print out the available options in a frame for the player to see
    std::stringstream ss;
    ss << text << "\n";

    // We create text lines for each available option
    for(int i = 0; i < opts.size(); i++){
        ss << "(" << i << ") " << opts[i];

        // We want two options to a line, so we alternate adding spaces between options and newline characters
        if(i % 2 == 0){
            ss << "\t\t\t";
        } else {
            ss << "\n";
        }
    }

    if(lastCommandInvalid){
        ss << "Invalid choice.\n";
    } else {
        ss << "\n";
    }

    PrintNewFrame(ss.str());

    //We get an input.
    std::string input;
    getline(std::cin,input);

    // We check for sanity (should only be one digit).
    if(input.length() <= 1){
        try{
            //Should only an integer that is from 0 to one less than the size of the "opts" vector.

            int action = std::stoi(input);
            if(action >= 0 || action < opts.size()){
                return action;
            } else {
                return GetPlayerVariableChoice(text, opts, true);

            }

        } catch(std::exception &e) {
            return GetPlayerVariableChoice(text, opts, true);
        }
    } else {
        return GetPlayerVariableChoice(text, opts, true);
    }

}


// Displays text in its own screen to the user (used for an alert message, essentially).
// No interaction is required so we do not care what the user input is.

bool GetPlayerConfirmation(std::string text){
    // We print out desired text in a frame.
    std::stringstream ss;
    ss << text << "\n";

    PrintNewFrame(ss.str());

    //We get an input.
    std::string input;
    getline(std::cin,input);

    // We do not need to sanitize input

    return true;
};