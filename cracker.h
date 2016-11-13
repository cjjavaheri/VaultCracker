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
#include <list>

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
    void bruteForce( unsigned int min, unsigned int max, double base, Response &response);
    string binarySearch(int length, double base, Response &response, long long int g1,
                         long long int  g2);


    
private:
    void checkLength(unsigned int &min, unsigned int &max);
    string getGuess(long double value, unsigned int length, double base);
    long long int getPassword(string guess, double base);
    char getNextChar(char some_char, double base);
    map<char, int> getCharacterMap();
    map<int, char> getIntegerMap();
    char getPrevChar(char some_char, double base);
    string FindMax(int length, double base, Response &response, long long int g1,
                             long long int  g2);


    string FindMin(int length, double base, Response &response, long long int g1,
                            long long int g2);

    void findCombinations(string guess, int length, Response &response);

    list<char> findOrdering(Response &response);


};
#endif

