/**************************************************************************//**
 * @file cracker.h
 * 
 * @brief The file which contains all of the function prototypes for cracker.
 *
 * @author: Cameron Javaheri, Garfield Tong, Soham Naik
 *
 * @par Class:
 * CSC300 Data Structures
 *
 * @date:   Fall 2016
 *
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

/*!
 * @brief its a constant
 */
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
    
    ///Function to brute force a password
    void bruteForce( unsigned int min, unsigned int max, double base, Response &response);
    
    ///uses binary search on specific search space
    string binarySearch(int smallestLength, int largestLength, double base, Response &response, long long int g1,
                         long long int  g2);


    
    ///checks range of password
    void checkLength(unsigned int &min, unsigned int &max);
    
    ///gets string representation of an integer
    string getGuess(long double value, unsigned int length, double base);
    
    ///gets integer representation of the guess
    long long int getPassword(string guess, double base);
    
    ///maps character to integer
    map<char, int> getCharacterMap();
    
    ///maps integer to character
    map<int, char> getIntegerMap();
    
    ///Gets the minimum value of g1 and g2
    string FindMin(int length, double base, Response &response, long long int g1,
                            long long int g2);

    ///bruteforces for length 1-4
    void findCombinations(string guess, int length, Response &response);

    ///gets list with corret character ordering
    list<char> findOrdering(Response &response);

};
#endif

