/**************************************************************************//**
 * @file cracker2.h
 * 
 * @brief The file which contains all of the function prototypes for cracker2.
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
#include <vector>
#include <deque>
#include <algorithm>

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

	/*!< Function to start the intrusion attempt */
    void getCracking(); 
    
    /*!< Function to brute force a password. */
    void bruteForce( unsigned int min, unsigned int max, double base, Response &response); 
    
 	/*!< Uses binary search on specific search space. */
    string fixedBinarySearch(unsigned int smallestLength, unsigned int largestLength, double base, Response &response, long long int g1, long long int  g2); 
    
	/*!< Checks range of password */
    void checkLength(unsigned int &min, unsigned int &max);  
    
    /*!< gets string representation of integer */
    string getGuess(long double value, unsigned int length, double base); 
    
    /*!< Gets integer representation of the guess. */
    long long int getPassword(string guess, double base); 
   
    /*!< Maps character to integer */
    map<char, int> getCharacterMap(); 
    
   /*!< Maps integer to character */
    map<int, char> getIntegerMap(); 

    /*!< Gets the minimum value of g1 and g2 */
    string FindMin(int length, double base, Response &response, long long int g1, 
                            long long int g2);


	/*!< Bruteforces for length 1 -4 */
    void findCombinations(string guess, int length, Response &response); 

 	/*!< Gets list with correct character ordering. */
    list<char> findOrdering(Response &response); 
    
  	/*!< This function is called to find signifcant digits. */
    map<long double, long double> digitalOrdering(const unsigned int &length); 

	 /*!< Finds digit ordering for a given length. */
    map<long double, int> findDigitOrdering(int length); 

	/*!< Finds correct length of the password */
    map<long double, int> findLength(vector<map<long double, int>> digitOrderings); 

};
#endif
