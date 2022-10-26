//
// Created by Wyatt Verchere on 2019-04-08.
//

#include "ui.h"

# include <sstream>

#ifdef _WIN32
bool WINDOWS = true;
#else
bool WINDOWS = false;
#endif


// Prints out the card name in its designated color
std::string CardToString(Card card){
    switch (card){
        case duke: return ColorizeText("Duke",Color::MAGENTA);
        case captain: return ColorizeText("Captain",Color::BLUE);
        case ambassador: return ColorizeText("Ambassador",Color::GREEN);
        case assassin: return ColorizeText("Assassin",Color::RESET);
        case contessa: return ColorizeText("Contessa",Color::RED);
        case nullCard: return ColorizeText("(None)", Color::RESET); // The pseudo-card "nullCard" is used for block handling to symbolize the player choosing to not block.
        default: return ColorizeText("??????",Color::RESET);

    }
}

void PrintNewFrame(const std::string & text){

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

std::string ColorizeText(const std::string & text, Color color) {

    // If the user is on a linux-based terminal that accepts these escape characters (I don't think win32 does),
    // colorizes the text with the given color.

    if (!WINDOWS) {
        std::stringstream colorText;
        colorText << "\x1B[" << color << "m" << text << "\x1B[" << RESET << "m";

        return colorText.str();
    }

    return text;
}

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
    return ActionType::bad_action;

}

// This function is used for actions such as Steal, Coup, and Assassinate
// A target is selected by the player
// It cannot be themselves and it must be a player in the game
int GetPlayerTargetChoice(const Game & game, bool lastCommandInvalid){

    // We print out the available options in a frame for the player to see
    std::stringstream ss;
    for (int i = 1; i < game.playerCount; i++) {
        ss << "(" << i << ") " << game.players[i]->GetPlayerInfoString(true);
    }
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
            if(action == 0 || action >= game.playerCount || game.players[action]->cardsLeft <= 0) {
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

bool GetPlayerYesNoChoice(const std::string & text, bool lastCommandInvalid){
    std::vector<std::string> opts = {"Yes","No"};
    return GetPlayerVariableChoice( text, opts,  lastCommandInvalid) == 0;
}


// Returns the index of the string selected from the "opts" list
int GetPlayerVariableChoice(const std::string & text, const std::vector<std::string> & opts, bool lastCommandInvalid){

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
bool GetPlayerConfirmation(const std::string & text){
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