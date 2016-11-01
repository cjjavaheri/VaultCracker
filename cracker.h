/***************************************************************************//**
 * @file
 ******************************************************************************/
#ifndef __CRACKER_H
#define __CRACKER_H
#include <functional>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>

#include <fstream>
#include <map>
#include <cmath>

#include "baseCracker.h"
#include "response.h"

using namespace std;

#define MAX_ALLOWED 24

/***************************************************************************//**
 * @class Cracker
 *
 * @par Description
 *    The cracker class used to attack a remote system and discover the
 * password.
 ******************************************************************************/
class Cracker : public BaseCracker
{
public:
    Cracker(); /*!< Constructor */

    void getCracking(); /*!< Function to start the intrusion attempt */
    string bruteForce( unsigned int min, unsigned int max, double base, map<string, int> &FileMap, Response 		&response);
private:
    string getGuess(double value, unsigned int length, double base);
    int getPassword(string guess, double base);
};
#endif
