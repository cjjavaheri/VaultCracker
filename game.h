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
#include "level4c.h"
#include "level5a.h"
#include "level5b.h"

using namespace std;
using namespace placeholders;

class Game
{
   BaseVault *vault;
   BaseCracker *cracker;
   Range range;
   chrono::duration<double> vaultTime;
   chrono::duration<double> crackTime;
   chrono::time_point<chrono::system_clock> startVault, startCrack;
   double timeLimit;
public:
   Game(BaseVault *v, int minLen, int maxLen, double maxTime=10.0);

   void sendPasswordFunc(function<Response(string)>& callback);
   Response sendPassword(string str);
   void startGame(BaseCracker *c);
};

#endif
