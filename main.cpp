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
    BaseVault *vault = new Level4a::Vault(6, 6);
    Game game(vault,6, 6);

    // This sets the sendPassword callback for the Cracker object
    game.sendPasswordFunc(cracker->sendPassword);
    game.startGame(cracker);

    return 0;
}
