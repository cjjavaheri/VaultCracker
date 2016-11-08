/***************************************************************************//**
 * @file
 ******************************************************************************/
#ifndef __LEVEL5A_H_
#define __LEVEL5A_H_
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <thread>
#include <unordered_map>
#include "baseVault.h"
#include "response.h"

using namespace std;

namespace Level5a
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

public:
   /// Constructor
   Vault(unsigned int minLen, unsigned int maxLen);

   /// The function that will check a password guess against the actual password
   Response sendPassword(string pw);

private:
   long double calculateScore(string);
   void setPW();
   void setRange(unsigned int, unsigned int);
};
}
#endif
