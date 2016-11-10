/***************************************************************************//**
 * @file
 ******************************************************************************/
#ifndef __LEVEL5C_H_
#define __LEVEL5C_H_
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <thread>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "baseVault.h"
#include "response.h"

using namespace std;

namespace Level5c
{
/***************************************************************************//**
 * @class Vault
 *
 * @par Description
 *    This class is where you will implement your defense strategy.
 ******************************************************************************/
class Vault : public BaseVault
{
   string password;
   Range range;
   unsigned pwLen;
   string vChars;
   vector<int> digits;

public:
   /// Constructor
   Vault(unsigned int minLen, unsigned int maxLen);

   /// The function that will check a password guess against the actual password
   Response sendPassword(string pw);

private:
   long double calculateScore(string);
   void setPW();
   void setRange(unsigned int, unsigned int);
   void scrambleDigits();
};
}
#endif