#ifndef __GAME_H_
#define __GAME_H_
#include <chrono>
#include <iostream>
#include <fstream>
#include <future>
#include "response.h"
#include "vault.h"
#include "cracker.h"
#include "level0.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4a.h"
#include "level4b.h"
#include "level4c.h"
#include "level5a.h"
#include "level5b.h"
#include "level5c.h"
#include "level6.h"
#include "level7.h"

using namespace std;
using namespace placeholders;

/***************************************************************************//**
 * @class Game
 *
 * @par Description
 *    Sets the game up
 ******************************************************************************/
class Game
{
   ///pointer to BaseVault
   BaseVault *vault;
   
   ///pointer to BaseCracker
   BaseCracker *cracker;
   
   ///Range of the password
   Range range;
   
   ///timer for vault
   chrono::duration<double> vaultTime;
   
   ///timer for cracker
   chrono::duration<double> crackTime;
   
   ///timer
   chrono::time_point<chrono::system_clock> startVault, startCrack;
   
   ///time limit for timer
   double timeLimit;
public:

   ///constructor which sets the initial game up
   Game(BaseVault *v, int minLen, int maxLen, double maxTime=10.0);

   ///binds .. 
   void sendPasswordFunc(function<Response(string)>& callback);
   
   ///this function takes in a guess and sends it to the vault and returns a response
   Response sendPassword(string str);
   
   ///starts game
   void startGame(BaseCracker *c);
};

#endif
