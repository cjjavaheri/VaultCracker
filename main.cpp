/******************************************************************************//**
 * @file main.cpp
 *
 * @brief A Red-Blue cyber security program
 *
 * @mainpage Program 2 - Red-Blue Cyber security exercise
 *
 * @section course_section Course Information
 * 
 * @author Cameron Javaheri, Garfield Tong, Soham Naik
 *
 * @date 13th Nov, 2016
 *
 * @par Professor:
 *           Dr. Paul Hinker
 *
 * @par Course:
 *           CSC300 - 1 - 10am 11am
 *           CSC300 - 2 - 2pm 3pm
 *
 * @par Location:
 *           McLaury 310 
 *
 * @details 
 *	This program is an exercise in red-blue cybersecurity. The goal
 * 	was to create a cracker that could find the password that a vault
 *	generated. There were a few rules that the vault had to abide by.
 *	The password had to be at a local minimum, the cracker had a specific
 *	length of time to find the password, some vaults had character and digit
 *	reordering, some vaults had multiple minimums, and some vaults had a
 *	combination of the latter. The main goal of this program was to develop
 *	algorithms and data structures to solve the given problem.
 *
 * @par Compiling instructions:
 *           Use cracker.cpp for the following vaults:
 *           vault5a, vault6
 *           Use cracker2.cpp for the rest, that is:
 *           vault0, vault1, vault2, vault3, vault4a, vault4b, vault4c,5b,5c7
 *
 *           When using the respective vaults, please change the cracker name in the
 *           makefile.
 *
 * @par Modifications and Development Timeline:
 *  < href="https://gitlab.mcs.sdsmt.edu/7394551/PA2.git">
 * 
 ******************************************************************************/

#include <iostream>
#include <functional>
#include <future>
#include <cassert>
#include <chrono>
#include "game.h"

using namespace std;

/**************************************************************************//**
 * @brief main function
 *
 *@return 0 if successful
 *
 ******************************************************************************/
int main()
{
    BaseCracker *cracker = new Cracker();
    BaseVault *vault = new Level6::Vault(6, 8);
    Game game(vault, 6, 8);

    // This sets the sendPassword callback for the Cracker object
    game.sendPasswordFunc(cracker->sendPassword);
    game.startGame(cracker);

    return 0;
}
