#ifndef TTT_GAME_H
#define TTT_GAME_H

#include "GameState.hpp"
#include "texthelper.hpp"
#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include "ai_random.hpp"
#include "ai_simple.hpp"
#include "ai_perfect.hpp"

#define cls() std::cout << CTRL_CLEAR << CTRL_CURSOR_RESET

enum AI_TYPE{AI_SIMPLE, AI_RANDOM, AI_PERFECT};

bool player_goes_first = true;
enum AI_TYPE ai_type = AI_SIMPLE;
unsigned int board_size = 3;

bool is_move_valid(Vec v, const GameState& g)
{
    if (v.y < 0 || v.x < 0) return false;
    if (v.x >= g.size || v.y >= g.size) return false;

    return true;
}

Vec interpret_input(std::string s)
{
    Vec v = Vec(0,0);
    std::string num = "";
    char letter = 0;
    for (int i = 0; i < s.size(); i += 1)
    {
        char c = s[i];
        if (std::isspace(c)) continue;
        if (!std::isalnum(c)) throw std::logic_error("badinput");
        if (std::isdigit(c))
        {
            num += c;
            continue;
        }
        // should be alphabet by this point
        if (letter != 0) throw std::logic_error("badinput");
        letter = std::tolower(c);
    }
    if (num == "") throw std::logic_error("badinput");
    if (!letter) throw std::logic_error("badinput");
    v.x = std::stoi(num)-1;
    v.y = letter - 'a';
    
    return v;
}

void routine_player_move(GameState& game, std::string& move_input ,std::string& errormsg)
{
    std::cout << "ENTER MOVE: " << std::endl << errormsg << std::endl << "> ";
    // Process input
    std::getline(std::cin, move_input);
    try
    {
        Vec player_move = interpret_input(move_input);
        errormsg = "";
        if (is_move_valid(player_move, game))
        {
            if (game.play(player_move.x,player_move.y)) return;
            else 
            {
                errormsg="";errormsg += CLR_RED; errormsg += "\tSpace already occupied"; errormsg += CLR_END;
                return;
            }
        }
        else
        {
            errormsg="";errormsg += CLR_RED; errormsg += "\tSpace out of bounds"; errormsg += CLR_END;
            return;
        }
    }
    catch(std::logic_error)
    {
        errormsg="";errormsg += CLR_RED; errormsg += "\tInvalid input. (examples: a 1, 1 a, b2, 3c)"; errormsg += CLR_END;
        return;
    }
            
}
void routine_ai_move(GameState& game)
{
    Vec move;
    switch (ai_type)
    {
        case AI_RANDOM:
            move = ai_random::get_move(game, (enum PLAYER_ID)game.currentTurn); // yes you can initialize an enum with int C++ compiler but let me help you
            break;
        case AI_SIMPLE:
            move = ai_simple::get_move(game, (enum PLAYER_ID)game.currentTurn);
            break;
        case AI_PERFECT:
            move = ai_perfect::get_move(game, (enum PLAYER_ID)game.currentTurn);
            break;
    }
    // assume AI gets valid move
    game.play(move.x, move.y);
}
void routine_game_end(GameState& game, std::string& name1, std::string& name2)
{
    // Determine winner
    int winner;
    if (game.hasWon(PLAYER_X)) winner = PLAYER_X;
    else if (game.hasWon(PLAYER_O)) winner = PLAYER_O;
    else winner = 2;
    // Send result
    system("CLS");
    std::cout << "    GAME OVER" << std::endl;
    std::cout << game << std::endl; // extra newline is intended
    if (winner == PLAYER_X) std::cout << CLR_RED << name1 << CLR_END << " wins!" << std::endl;
    else if (winner == PLAYER_O) std::cout << CLR_GREEN << name2 << CLR_END << " wins!" << std::endl;
    else std::cout << CLR_YELLOW << "It's a tie!" << CLR_END << std::endl;
    // Exit
    std::cout << std::endl << "Press " << CLR_GREEN << "Enter" << CLR_END << " to exit." << std::endl;
    std::getchar();
}

void screen_title()
{
//    std::cout << CTRL_ENTER_SCREEN; // save current terminal state on start
    system("CLS");
// placeholder (?)
    std::cout << "Tic Tac Toe" << std::endl;
    std::cout << "by Reuben Self, Zach English, Victor Khatri" << std::endl;
    std::cout << std::endl << "Press " << CLR_GREEN << "Enter" << CLR_END << " to start" << std::endl;
    std::getchar();
}
char screen_menu()
{
    system("CLS");
// placeholder (?)
    std::cout << CLR_GREEN << "[1]" << CLR_END << " Play vs Local Player" << std::endl;
    std::cout << CLR_GREEN << "[2]" << CLR_END << " Play vs AI" << std::endl;
    std::cout << CLR_GREEN << "[3]" << CLR_END << " Options" << std::endl;
    std::cout << CLR_GREEN << "[4]" << CLR_END << " Exit" << std::endl;
    std::cout << std::endl << "Select Option: ";
    char c;
    std::cin >> c;
    return c;
}
void screen_game(bool ai_player)
{
    system("CLS");
    std::cin.ignore();

    // Initialize game
    GameState game = GameState(board_size);
    // Get player names
    std::string name1;
    std::string name2;
    if (ai_player)
    {
        if (ai_type == AI_SIMPLE) name2 = "SIMON";
        if (ai_type == AI_RANDOM) name2 = "RANDY";
        if (ai_type == AI_PERFECT) name2 = "EUGENE";
        std::cout << "Give Player Name: ";
        std::getline(std::cin, name1);
        if (!player_goes_first) // switch names
        {
            std::string temp = name1;
            name1 = name2;
            name2 = temp;
        }
    }
    else
    {
        std::cout << "Player 1 Name: " << CLR_RED;
        std::getline(std::cin, name1);
        std::cout << CLR_END << "Player 2 Name: " << CLR_GREEN;
        std::getline(std::cin, name2);
        std::cout << CLR_END;
    }
    // Game loop
    std::string errormsg = "";
    while(!game.done)
    {
        std::string move_input;
        // Send current state
        system("CLS");
        std::cout << CLR_RED << name1 << CLR_END << " vs. " << CLR_GREEN << name2 << CLR_END << std::endl;
        std::cout << std::endl;
        if (game.currentTurn == PLAYER_X) std::cout << CLR_RED << "[X]" << CLR_END << " Turn" << std::endl;
        else std::cout << CLR_GREEN << "[O]" << CLR_END << " Turn" << std::endl;
        std::cout << game;
        if (!ai_player)
        {
            routine_player_move(game, move_input, errormsg);
        }
        else
        {
            if ( ((game.turnCount % 2) == (!player_goes_first))) // Get player move on even index turns if goes first
            {
                routine_player_move(game, move_input, errormsg);
            }
            else // Otherwise query AI move.
            {
                routine_ai_move(game);
            }
        }
    }
    // Game end
    routine_game_end(game, name1, name2);
}

void screen_options()
{
    option:
    system("CLS");

    if (player_goes_first == false) 
    {
    std::cout << CLR_GREEN << "[1]" << CLR_END << " Player Goes First = " CLR_RED << "False" << CLR_END << std::endl;
    }    
    else 
    {
    std::cout << CLR_GREEN << "[1]" << CLR_END << " Player Goes First = " CLR_GREEN << "True" << CLR_END << std::endl;
    }
    if (ai_type == AI_SIMPLE) 
    {
    std::cout << CLR_GREEN << "[2]" << CLR_END << " Simple A.I = " CLR_GREEN << "On" << CLR_END << std::endl;
    std::cout << CLR_GREEN << "[3]" << CLR_END << " Random A.I = " CLR_RED << "Off" << CLR_END << std::endl;
    std::cout << CLR_GREEN << "[4]" << CLR_END << " Perfect A.I = " CLR_RED << "Off" << CLR_END << std::endl;
    }

    else if (ai_type == AI_RANDOM) 
    {
    std::cout << CLR_GREEN << "[2]" << CLR_END << " Simple A.I = " CLR_RED << "Off" << CLR_END << std::endl;
    std::cout << CLR_GREEN << "[3]" << CLR_END << " Random A.I = " CLR_GREEN << "On" << CLR_END << std::endl;
    std::cout << CLR_GREEN << "[4]" << CLR_END << " Perfect A.I = " CLR_RED << "Off" << CLR_END << std::endl;
    }

    else if (ai_type == AI_PERFECT) 
    {
    std::cout << CLR_GREEN << "[2]" << CLR_END << " Simple A.I = " CLR_RED << "Off" << CLR_END << std::endl;
    std::cout << CLR_GREEN << "[3]" << CLR_END << " Random A.I = " CLR_RED << "Off" << CLR_END << std::endl;
    std::cout << CLR_GREEN << "[4]" << CLR_END << " Perfect A.I = " CLR_GREEN << "On" << CLR_END << std::endl;
    }

    std::cout << CLR_GREEN << "[5]" << CLR_END << " Board Size = [" << board_size << ", " << board_size << "]" << std::endl;
    std::cout << CLR_GREEN << "[6]" << CLR_END << " Exit" << std::endl;


    std::cout << std::endl << "Select Option: ";
    int option;
    std::string temp;
    std::getline(std::cin, temp);
    try 
    {
        option = std::stoi(temp); 
    }
    catch (std::invalid_argument) 
    {
        goto option;
    }

    if (option == 1 && player_goes_first == true) 
    {
        player_goes_first = false;
        screen_options();
    }

    else if (option == 1 && player_goes_first == false)
    {
        player_goes_first = true;
        screen_options();
    }

    else if (option == 2) 
    {
        ai_type = AI_SIMPLE;
        screen_options();
    }
    else if (option == 3) 
    {
        ai_type = AI_RANDOM;
        screen_options();
    }

    else if (option == 4) 
    {
        ai_type = AI_PERFECT;
        screen_options();
    }

    else if (option == 5) 
    {
        board_size: 
        system("CLS");
        int x;
        std::cout << "Enter New Board Size: ";
        std::string temp;
        std::getline(std::cin, temp);
        try 
        {
            x = std::stoi(temp); 
        }
        catch (std::invalid_argument) 
        {
            goto board_size;
        }


        std::cout << std::endl;
        if (x > 26 || x < 0) 
        {

        screen_options();

        }

        else {
        board_size = x;
        screen_options();
        }

    }

    else if (option == 6) 
    {
        return;
    }

    else if (option <= 0 || option > 6) {
        screen_options();
    }

}

void screen_exit()
{
    std::cout << CTRL_EXIT_SCREEN; // restore saved terminal state
}

#endif