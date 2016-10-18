#include <iostream>
#include <functional>
#include <future>
#include <cassert>
#include <chrono>
#include "game.h"

using namespace std;

int main()
{
   BaseCracker *cracker = new Cracker();
   BaseVault *vault = new Vault(7, 10);
   Game game(vault, 7, 10);

   // This sets the sendPassword callback for the Cracker object
   game.sendPasswordFunc(cracker->sendPassword);
   game.startGame(cracker);

   return 0;
}
