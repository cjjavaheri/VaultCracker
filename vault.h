/***************************************************************************//**
 * @file
 ******************************************************************************/
#ifndef __VAULT_H_
#define __VAULT_H_
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <thread>
#include "baseVault.h"
#include "response.h"

#include <fstream>
#include <map>
#include <cmath>

using namespace std;

#define PI 3.141592654

/***************************************************************************//**
 * @class Vault
 *
 * @par Description
 *    This class acts as the interface to an external system.
 ******************************************************************************/
class Vault : public BaseVault
{
    /// A place to store the target password
    string password;

    /// The range of lengths that the password might be
    Range range;

    /// The actual length of the password
    int pwLen;

public:
    /// Constructor
    Vault(int minLen, int maxLen);

    /// The function that will check a password guess against the actual password
    Response sendPassword(string pw);

private:
    /// Private function to calculate a password
    void setPW();
    /// Private function to set the range and actual password length
    void setRange(int, int);

    long double getPassword(string guess, long long int passwordLength, double base);

    string getGuess(long double value, unsigned int length, double base);
    long long int getInteger(string guess, double base);
    long long int  CalculateLength(long double pass, long double min, long double scalar, long 		double length, long double intvalue, long double constant, long double period);
};

#endif
