#ifndef __GAME_H_
#define __GAME_H_
#include <iostream>
#include <future>
#include "response.h"
#include "vault.h"
#include "cracker.h"

using namespace std;
using namespace placeholders;

class Game
{
    BaseVault *vault;
    BaseCracker *cracker;
    Range range;
public:
    Game(BaseVault *v, int minLen, int maxLen);

    void sendPasswordFunc(function<Response(string)>& callback);
    Response sendPassword(string str);
    void startGame(BaseCracker *c);
};

#endif
