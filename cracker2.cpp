/**************************************************************************//**
 * @file cracker2.cpp
 *
 * @brief The file which contains all of the function definitions for cracker2.h
 *
 * @author: Cameron Javaheri, Garfield Tong, Soham Naik
 *
 * @par Class:
 * CSC300 Data Structures
 *
 * @date:   Fall 2016
 *
 ******************************************************************************/

#include "cracker2.h"
#include <time.h>
#include <vector>
#include <fstream>
#include <map>
#include <cmath>

/***************************************************************************//**
 * @brief Constructor
 ******************************************************************************/
Cracker::Cracker() {}

/***************************************************************************//**
 * @author Cameron Javaheri
 * @brief This is called to start the intrusion attempt
 *
 * @par Description
 *    This function is called by the game system once the Vault is created
 * and the callback function is bound.  It is assumed that the game system
 * has started the attempt timer and 'the clock is ticking' at this point.
 ******************************************************************************/
void Cracker::getCracking()
{
    string truePassword = "";
    string globalMin = "";
    Response response;
    double base = 74;
    long long int g1;
    long long int g2;
    list<char> ordering;
    static map<long double, int> digitOrdering;
    static map<long double, int> digit5;
    static map<long double, int> digit6;
    static map<long double, int> digit7;
    static map<long double, int> digit8;
    unsigned int length = 0;
    vector<map<long double, int>> correctOrdering;
    map<long double, int>::iterator it;
    string guess = "aaaaaa";
    unsigned int min=0;
    unsigned int max=0;

    //gets correct digit ordering for length 5-8
    if (digit5.empty())
    {
        digit5 = findDigitOrdering(5);
        correctOrdering.push_back(digit5);
    }

    if (digit6.empty())
    {
        digit6 = findDigitOrdering(6);
        correctOrdering.push_back(digit6);
    }
    if (digit7.empty())
    {
        digit7 = findDigitOrdering(7);
        correctOrdering.push_back(digit7);
    }
    if (digit8.empty())
    {
        digit8 = findDigitOrdering(8);
        correctOrdering.push_back(digit8);
    }

    //finds correct length
    if (digitOrdering.empty())
        digitOrdering = findLength(correctOrdering);

    //finds range of length of password
    checkLength(min, max);

    //correct length of the password
    length = digitOrdering.size();


    // Brute force the password.
    if ( min < 5)
        bruteForce(min, max, base, response);

    //  Binary search
    if (max > 4)
    {
        min = length;
        max = length;
        g1 = 0;
        g2 = pow(base, length);
        truePassword = fixedBinarySearch(min, max, base, response, g1, g2);
    }
    return;

}

/***************************************************************************//**
 * @author Cameron Javaheri
 * @brief A brute force algorithm to find the password.
 *
 * @par Description
 *    This function generates all possible permutations for a fixed password length
 * with a given base.
 *
 * @param[in] min - min length of the password.
 * @param[in] max - max length of the password.
 * @param[in] base - The number of characters used in the guesses.
 * @param[in,out] response - The response that the vault sends back to the cracker.
 *
 * @return The actual password to the vault.
 ******************************************************************************/
void Cracker::bruteForce(unsigned int min, unsigned int max, double base, Response &response)
{
    string guess = "";
    unsigned int i;
    unsigned int j;
    guess.resize(min);

    j = 1;
    //bruteforces for length<5
    while (guess.size() < 5)
    {
        for (i = 0; i < guess.size(); i++)
            guess[i] = 'a';
        findCombinations(guess, guess.size(), response);
        if (response.rc == ACCEPTED)
            return;
        guess.resize(min + j);
        j++;
    }

}

/***************************************************************************//**
 * @author Cameron Javaheri
 *
 * @brief checkLength is called to find the exact length of the password
 *
 * @par Description
 *    This function will be used to find the range minimum and maximum and use
 *    that to find the specific length of the password
 *
 * @param[in] min - will pass back the range minimum.
 * @param[in] max - will pass back the range maximum.
 ******************************************************************************/

void Cracker::checkLength(unsigned int &min, unsigned int &max)
{
    string checklen="";
    Response getlen=sendPassword(checklen);
    //for checking min and max length of password
    for(int i=0; i < 12; i++)
    {
        getlen=sendPassword(checklen);
        if(getlen.rc == PW_TOO_SHORT)
        {
            min++;
        }
        max++;

        if (getlen.rc == PW_TOO_LONG)
        {
            max -= 2;
            break;
        }
        checklen+="a";
    }
}

/***************************************************************************//**
 * @author Cameron Javaheri
 * @brief getGuess is an algorithm used to generate a password guess given a number.
 *
 * @par Description
 *    This function generates a unique password guess given a unique number. The number
 * must be an integer, but has a data type of double due to the calculations and
 * the rounding. What this function does specifically is create a character map of
 * all of the possible characters, and then treats the value the user inputs as a number
 * in the given base provided by the user. It then calculates each letter using that base.
 * Finally, it returns a permutation (password guess).
 *
 * @param[in] value - The number used to generate the password guess. Must be given as int
 * but the data type provided is double because of the calculations done.
 * @param[in] length - The length of the password.
 * @param[in] base - The number of characters to use in the guess.
 *
 * @return A string which is the permuatation represented by the value sent in
 * by the user.
 ******************************************************************************/
string Cracker::getGuess(long double value, unsigned int length, double base)
{
    static map<int, char> IntegerMap; //maps each digit with an integer
    static map<long double, int> digitOrdering;
    static map<long double, int> digit5;
    static map<long double, int> digit6;
    static map<long double, int> digit7;
    static map<long double, int> digit8;
    static map<long double, int>::iterator digitIt;
    vector<map<long double, int>> correctOrdering;
    map<int, char>::iterator cit;
    unsigned int digit;
    string guess = "";
    unsigned int counter;

    //gets a map with integers and characters
    if (IntegerMap.empty())
        IntegerMap = getIntegerMap();

    //finds digit ordering for each length
    if (digit5.empty())
    {
        digit5 = findDigitOrdering(5);
        correctOrdering.push_back(digit5);
    }

    if (digit6.empty())
    {
        digit6 = findDigitOrdering(6);
        correctOrdering.push_back(digit6);
    }
    if (digit7.empty())
    {
        digit7 = findDigitOrdering(7);
        correctOrdering.push_back(digit7);
    }
    if (digit8.empty())
    {
        digit8 = findDigitOrdering(8);
        correctOrdering.push_back(digit8);
    }

    //finds length of the password by looking at ordering
    if (digitOrdering.empty())
        digitOrdering = findLength(correctOrdering);

    //gets correct length of password
    length = digitOrdering.size();

    counter = 1;
    guess.resize(length);
    digitIt = digitOrdering.end();
    digitIt--;

    while (counter < length + 1)
    {
        //digit is the quotient which we use to find the correponding character
        //inside of the integer map
        digit = int(value / pow(base, length - counter));

        //finds the character corresponding to the digit
        cit = IntegerMap.find(digit);

        guess[guess.size() - digitIt->second] = cit->second;

        //Integer value of the remainder
        value = (value / ((long double)pow(base, length - counter)) - ((long double)digit)) * ((long double)pow(base, length - counter));

        //checking to make sure the remainder is an integer
        value = nearbyint(value);
        digitIt--;
        counter++;
    }

    return guess;

}

/***************************************************************************//**
 * @author Cameron Javaheri
 *
 * @par Description
 *    Converts a password guess to a long long integer which will be graphed
 *
 * @param[in] guess -guessed password
 * @param[in] base - base 74
 *
 * @return The long long int representation of the guess
 ******************************************************************************/
long long int Cracker::getPassword(string guess, double base)
{
    long long int j = 0;
    long long int sum = 0;
    static map<char, int> CharacterMap;
    static map<long double, int> digitOrdering;
    static map<long double, int> digit5;
    static map<long double, int> digit6;
    static map<long double, int> digit7;
    static map<long double, int> digit8;
    static map<long double, int>::iterator digitIt;
    vector<map<long double, int>> correctOrdering;
    map<char, int>::iterator cit;
    static int counter = 0;

    counter++;


    if (CharacterMap.empty())
        CharacterMap = getCharacterMap();


    //finds digit ordering for each length
    if (digit5.empty())
    {
        digit5 = findDigitOrdering(5);
        correctOrdering.push_back(digit5);
    }

    if (digit6.empty())
    {
        digit6 = findDigitOrdering(6);
        correctOrdering.push_back(digit6);
    }
    if (digit7.empty())
    {
        digit7 = findDigitOrdering(7);
        correctOrdering.push_back(digit7);
    }
    if (digit8.empty())
    {
        digit8 = findDigitOrdering(8);
        correctOrdering.push_back(digit8);
    }

    //gets correct ordering
    if (digitOrdering.empty())
        digitOrdering = findLength(correctOrdering);


    guess.resize(digitOrdering.size());

    //converts string to its respective integer value
    digitIt = digitOrdering.begin();
    while (digitIt != digitOrdering.end())
    {
        cit = CharacterMap.find(guess[guess.size() - digitIt->second]);
        sum += (cit->second * pow(base, j));
        j++;
        digitIt++;
    }

    return sum;
}

/***************************************************************************//**
 * @author Cameron Javaheri, Soham Naik
 *
 * @brief binarySeach is an algorithm used to find the correct password using a binary search
 * algorithm section by section.
 *
 * @par Description
 *    This recursive function searches the entire search space of the vault. It starts out
 * with a multiplier which dictates the range of each section to search. This multiplier is added to
 * the initial value. This gives a range for the findmin function to search. The findmin function
 * then searches that range. If the password is not found, it returns back to the binary search function
 * to check for a larger section until it finds a minimium. Once a minimum is found the initialValue gets
 * reset, and it starts searching small ranges again. It continues to do this until the binary search
 * has searched the entire search space. If the password is not found, it starts searching the search space
 * from the beginning and continues to repeat this process until the password is found.
 *
 * @param[in] smallestLength - minimum length of the password
 * @param[in] largestLength - maximum length of the password
 * @param[in] base - base 74
 * @param[in,out] response - response after sending the password to the vault
 * @param[in] g1 - left integer representation of the string of the range to be searched
 * @param[in] g2 - right integer representation of the string of the range to be searched
 *
 * @return Correct Password
 ******************************************************************************/
string Cracker::fixedBinarySearch(unsigned int smallestLength, unsigned int largestLength, double base, Response &response, long long int g1, long long int  g2)
{
    string min;
    string firstMin;
    string g3Next;
    string g3Prev;
    bool found = false;
    long long int g3;
    long double g3NextScore;
    long double g3PrevScore;
    long double g3Score;
    long double value = 1.0;
    long double initialValue = 0.0;
    long double multiplier;
    unsigned int length;
    long long int counter = 0;


    length = largestLength;

    //sets multipler for each length
    if (length <= 6)
    {
        multiplier = 1.50;
    }

    else if (length == 7)
    {
        multiplier = 1.50;
    }

    else
    {
        multiplier = 1.50;
    }

    //runs until password is found
    while ( response.rc != ACCEPTED)
    {
        found = false;
        while (!found)
        {
            //finding a minimum by sending a range to search
            firstMin = FindMin(length, base, response, initialValue,initialValue + value);
            counter++;

            //increasing length of search space
            initialValue = initialValue + value;
            value *= multiplier;

            //integer value of a minimum in a seached section
            g3 = getPassword(firstMin, base);

            //the very next value of the minimum
            g3Next = getGuess(g3 + 1, length, base);

            //the previous value of the minimum
            g3Prev = getGuess(g3 - 1, length, base);

            //checks to see if the minimum, the next value and the prev value is the
            //password
            response = sendPassword(firstMin);
            if (response.rc == ACCEPTED)
                return firstMin;
            g3Score = response.score;
            response = sendPassword(g3Next);
            if (response.rc == ACCEPTED)
                return g3Next;
            g3NextScore = response.score;
            response = sendPassword(g3Prev);
            if (response.rc == ACCEPTED)
                return g3Prev;
            g3PrevScore = response.score;

            //checks to see if g3 is the minimum so that we can search the next
            //section
            if (g3Score < g3NextScore && g3Score < g3PrevScore)
            {
                found = true;
                value = 1;
                initialValue = g3;
            }

            //Checking to see if we reached the end of the search space.
            //If we reach the end of the search space, we restart with a smaller
            //multiplier
            if (initialValue > pow(base, length) - 1)
            {
                found = true;
                if (length <= 6)
                {
                    multiplier = multiplier - 0.001;
                }
                else if (length == 7)
                {
                    multiplier = multiplier - 0.001;
                }
                else
                {
                    multiplier = multiplier - 0.001;
                }
                value = 1;
                initialValue = 0.0;

            }

            //increases multipler if number of guesses exceed 5000
            if (counter > 5000)
            {
                counter = 0;
                multiplier = sqrt(exp(multiplier));
            }

            //if multiplier gets too larger, decrease it
            if (multiplier > 10.0)
            {
                multiplier = pow(multiplier, 0.20);
            }

        }

    }

    return firstMin; //true password
}

/***************************************************************************//**
 * @author Cameron Javaheri, Soham Naik
 *
 *  @brief A function to find minimums.
 *
 * @par Description Navigates itself to the minimum value of a specific search space
 *      in the vault using binary search algorithm
 *
 *
 * @param[in] length - length of the guess
 * @param[in] base - base 74
 * @param[in,out] response - response sent by the vault
 * @param[in] g1 - left integer representation of the string of the range to be searched
 * @param[in] g2 - right integer representation of the string of the range to be searched
 *
 * @return the minimum of the range between g1 and g2
 * @return the mid value of g1 and g2 if the minimum is not found
 ******************************************************************************/
string Cracker::FindMin(int length, double base, Response &response, long long int g1,
                        long long int  g2)
{
    long long int g3 = ceil((g1 + g2) / 2.0);
    string guess1;
    string guess2;
    string guess3;
    string temp = "";
    string empty = "";
    long double s1;
    long double s2;
    long double s3;
    long double g3NextScore;
    long double g3PrevScore;
    string g3Next;
    string g3Prev;
    static deque<long long int> vg1;
    static deque<long long int> vg2;

    //gets the guess just after the minimum value
    g3Next = getGuess(g3 + 1, length, base);

    //gets the guess just before the minimum value
    g3Prev = getGuess(g3 - 1, length, base);

    response = sendPassword(g3Next);
    if (response.rc == ACCEPTED)
        return g3Next;
    g3NextScore = response.score;
    response = sendPassword(g3Prev);
    if (response.rc == ACCEPTED)
        return g3Prev;
    g3PrevScore = response.score;

    //gets the string representation of the integer values passed in
    guess1 = getGuess(g1, length, base); // left
    guess2 = getGuess(g2, length, base); // right
    guess3 = getGuess(g3, length, base); // mid

    //checks if g1, g2 or g3 is the password
    response = sendPassword(guess1);
    if (response.rc == ACCEPTED)
        return guess1;
    s1 = response.score;

    response = sendPassword(guess2);
    if (response.rc == ACCEPTED)
        return guess2;
    s2 = response.score;

    response = sendPassword(guess3);
    if (response.rc == ACCEPTED)
        return guess3;
    s3 = response.score;


    //If item is located at a minimum return the item
    if (g3NextScore > s3 && g3PrevScore > s3)
        return guess3;

    // If item is not found in search just return
    else if (g1 == g3 || g2 == g3)
        return guess3;

    //to prevent the function from going into an infinite loop as sometimes
    //the left is never equal to the right and so it never exits the binary search
    //function
    else if (abs(g1 - g2) < 7)
        return guess3;

    // s1 is the min value
    else if (s1 <= s3 && s1 <= s2)
    {

        return FindMin(length, base, response, g1 + 1, g3 - 1);
    }

    //s2 is the min value
    else if (s2 <= s3 && s2 <= s1)
    {

        return FindMin(length, base, response, g3 + 1, g2 - 1);
    }

    //s3 is the min value
    else
    {


        if (s1 <= s2)
            return FindMin(length, base, response, g3 - 1, g1 + 1);

        else
            return FindMin(length, base, response, g3 + 1, g2 - 1);
    }

}

/***************************************************************************//**
 * @author Cameron Javaheri
 *
 * @brief This function gets a map of characters mapped to digits.
 *
 * @par Description This function creates a map with the correct character
 * ordering by calling the findOrdering function.
 *
 * @return A map with characters mapped to digits
 ******************************************************************************/
map<char,int> Cracker::getCharacterMap()
{
    Response response;
    static map<char, int> CharacterMap;
    static list<char> ordering;
    list<char>::iterator orderingIt;
    int i;


    if (ordering.empty())
        ordering = findOrdering(response);

    if (CharacterMap.empty())
    {
        orderingIt = ordering.begin();

        for (i = 0; i < 74; i++)
        {
            //mapping each character to a digit
            CharacterMap.insert({*orderingIt, i});
            orderingIt++;
        }
    }
    return CharacterMap;

}

/***************************************************************************//**
 * @author Cameron Javaheri
 *
 * @brief This function gets a map with integers mapped to characters.
 *
 *
 * @par Description Maps integers to characters by calling findOrdering function.
 * The difference between this function and the getCharacterMap function is that
 * this function maps integers to characters and the opposite in the latter.
 *
 * @return A map with integers mapped to characters
 ******************************************************************************/
map<int, char> Cracker::getIntegerMap()
{
    Response response;
    //initialzing only once to speed up program
    static list<char> ordering;
    static map<int, char> IntegerMap;

    list<char>::iterator orderingIt;
    map<int, char>::iterator cit;
    int i;

    if (ordering.empty())
        ordering = findOrdering(response);

    if (IntegerMap.empty())
    {
        orderingIt = ordering.begin();

        for (i = 0; i < 74; i++)
        {
            //mapping integer to character
            IntegerMap.insert({i, *orderingIt});
            orderingIt++;
        }
    }

    return IntegerMap;
}

/***************************************************************************//**
 * @author Cameron Javaheri, Soham Naik
 *
 * @brief Brute forces combinations for a specific length.
 *
 * @par Description Brute forces every possible combination for length 1-4
 *
 * @param[in] guess - string representation of the guess
 * @param[in] length - length of the guess
 * @param[in,out] response - response sent by the vault
 *
 ******************************************************************************/
void Cracker::findCombinations(string guess, int length, Response &response)
{
    int i;
    int j;
    int k;
    int l;

    //brute forcing password of length 4 by changing 1 character
    //at a time
    if (length == 4)
    {
        for (l = 0; l < 74; l++)
        {
            guess[guess.size() - 4] = validChars[l];
            for (k = 0; k < 74; k++)
            {
                guess[guess.size() - 3] = validChars[k];
                for (j = 0; j < 74; j++)
                {
                    guess[guess.size() - 2] = validChars[j];
                    for (i = 0; i < 74; i++)
                    {
                        guess[guess.size() - 1] = validChars[i];
                        response = sendPassword(guess);
                        if (response.rc == ACCEPTED)
                            return;
                    }
                }
            }
        }
    }

    //brute forcing password of length 3 by changing 1 character
    //at a time
    else if (length == 3)
    {
        for (k = 0; k < 74; k++)
        {
            guess[guess.size() - 3] = validChars[k];
            for (j = 0; j < 74; j++)
            {
                guess[guess.size() - 2] = validChars[j];
                for (i = 0; i < 74; i++)
                {
                    guess[guess.size() - 1] = validChars[i];
                    response = sendPassword(guess);
                    if (response.rc == ACCEPTED)
                        return;
                }
            }
        }
    }

    //brute forcing password of length 2 by changing 1 character
    //at a time
    else if (length == 2)
    {
        for (j = 0; j < 74; j++)
        {
            guess[guess.size() - 2] = validChars[j];
            for (i = 0; i < 74; i++)
            {
                guess[guess.size() - 1] = validChars[i];
                response = sendPassword(guess);
                if (response.rc == ACCEPTED)
                    return;
            }
        }
    }

    //brute forcing password of length 1
    else
    {
        for (i = 0; i < 74; i++)
        {
            guess[guess.size() - 1] = validChars[i];
            response = sendPassword(guess);
            if (response.rc == ACCEPTED)
                return;
        }
    }

}

/***************************************************************************//**
 * @author Cameron Javaheri, Soham Naik
 *
* @brief Finds the character ordering.
 *
 * @par Description findOrdering function finds the correct character ordering by
 * first sending 74 guesses of the least significant digit. It then stores those
 * values in a map. Then it looks at the specific score of one of the password guesses
 * and compares that score to the other 73 scores by taking the difference of the scores.
 * These differences end up giving the correct character ordering by looking at all of
 * the scores with the negative values all the way to the scores with high positive values.
 *
 * @param[in,out] response - response sent by the vault
 *
 * @return A list with the correct character ordering
 ******************************************************************************/
list<char> Cracker::findOrdering(Response &response)
{
    string guess = "";
    unsigned int i;
    long double difference;
    map<char, long double> scores;
    map<char, long double>::iterator it;
    map<char, long double>::iterator compare;
    map<long double, char> scoreDifferences;
    map<long double, char> duplicates;
    map<long double, char>::iterator differenceIt;
    map<long double, char>::iterator duplicatesIt;
    map<long double, int> digitOrdering;
    map<long double, int>::iterator digitIt;
    unsigned int j;
    list<char> ordering;
    list<char>::iterator orderingIt;
    unsigned int min = 0;
    unsigned int max = 0;

    //checks for range of password
    checkLength(min, max);
    guess.resize(max);

    //finds digit ordering for guess size
    digitOrdering = findDigitOrdering(guess.size());
    digitIt = digitOrdering.begin();
    j = digitIt->second;

    //fills the string with 'a' till full
    for (i = 0; i < guess.size(); i++)
    {
        guess[i] = 'a';
    }

    //changes one letter at a time of the least significant digit
    for (i = 0; i < 74; i++)
    {
        guess[guess.size() - j] = validChars[i];
        response = sendPassword(guess);
        scores.insert({guess[guess.size() - j], response.score});
        guess[guess.size() - j] = validChars[i];
        response = sendPassword(guess);
        scores.insert({guess[guess.size() - j], response.score});
    }

    it = scores.begin();
    compare = it;
    //compares 73 scores to 1 specific score
    for (i = 0; i < 74; i++)
    {
        difference = it->second - compare->second;
        differenceIt = scoreDifferences.find(difference);

        scoreDifferences.insert({difference, compare->first});

        compare++;
    }


    i = 0;

    differenceIt = scoreDifferences.begin();
    orderingIt = ordering.begin();
    //filling the list with the ordering
    while (differenceIt != scoreDifferences.end())
    {
        ordering.push_front(differenceIt->second);
        i++;
        differenceIt++;
    }

    return ordering;
}

/***************************************************************************//**
 * @author Cameron Javaheri
 *
 * @brief finds the digit ordering.
 *
 * @par Description First this function fills a string with 'a' until full.
 * Then it keeps n-1 letters constant and changes one letter at a time and
 * saves the scores. Then it compares 73 of the scores to 1 specific score.
 * From this, it calculates the difference between each score with the score
 * chosen. This gives the significance of simply one digit, this algorithm
 * has to be ran n times where n is the length of the password. Finally,
 * it stores these significances in a map so that they are ordered from least
 * significant to most significant. The other number stored with the significance
 * tells the actual position of the digit.
 *
 * @param[in] length - length of the digit ordering
 *
 * @return A map containing the correct digit ordering
 ******************************************************************************/
map<long double, int> Cracker::findDigitOrdering(int length)
{
    string guess;
    unsigned int j;
    Response response;
    unsigned int i;
    map<char, long double> scores;
    map<char, long double>::iterator it;
    map<char, long double>::iterator compare;
    map<long double, char>::iterator differenceIt;
    map<long double, char> scoreDifferences;
    long double difference;
    map<long double, int> ordering;
    long double sum;

    guess.resize(length);

    //fills the string with 'a' till full
    for (i = 0; i < guess.size(); i++)
    {
        guess[i] = 'a';
    }

    //Finds the digit significance for each character
    for (j = 1; j <= guess.size(); j++)
    {
        //changes one letter at a time
        for (i = 0; i < 74; i++)
        {
            guess[guess.size() - j] = validChars[i];
            response = sendPassword(guess);
            scores.insert({guess[guess.size() - j], response.score});
        }

        //compares 1 score to the other 73 scores
        it = scores.begin();
        compare = it;
        for (i = 0; i < 74; i++)
        {
            difference = abs(it->second - compare->second);
            differenceIt = scoreDifferences.find(difference);

            scoreDifferences.insert({difference, compare->first});

            compare++;
        }

        differenceIt = scoreDifferences.begin();
        sum = 0;
        //adding up all the scores to ge the significance
        while (differenceIt != scoreDifferences.end())
        {
            sum = sum + differenceIt->first;
            differenceIt++;
        }

        sum = sum;
        ordering.insert({sum, j});
        scores.clear();
        scoreDifferences.clear();
    }

    return ordering;

}

/***************************************************************************//**
 * @author Cameron Javaheri, Soham Naik
 *
 * @brief finds the specific length of the password.
 *
 * @par Description This function determines the length by looking at the
 * digit orderings. The digit orderings 5, 6, 7, 8 and are passed in by a
 * vector of maps. Then, we use an iterator to walk through every map
 * comparing each element in each map correspondingly. The length of
 * the digit ordering with the smallest value will be the correct length.
 * If any digit ordering has duplicates, then the length is incorrect.
 *
 * @param[in] digitOrderings - vector of maps containing digit ordering of 5-8
 *
 * @return A map containing the digit ordering for the correct length
 ******************************************************************************/
map<long double, int> Cracker::findLength(vector<map<long double, int>> digitOrderings)
{
    unsigned int min = 0;
    unsigned int max = 0;
    map<long double, int>::iterator digit5It;
    map<long double, int>::iterator digit6It;
    map<long double, int>::iterator digit7It;
    map<long double, int>::iterator digit8It;
    map<long double, int>::iterator prev;
    int counter;
    unsigned int possibleLength;

    checkLength(min, max);

    //iterators for digit orderings
    digit5It =  digitOrderings[0].begin();
    digit6It = digitOrderings[1].begin();
    digit7It = digitOrderings[2].begin();
    digit8It = digitOrderings[3].begin();

    if (min == 5 && max == 5)
        return digitOrderings[0];

    if (min == 6 && max == 6)
        return digitOrderings[1];

    if (min == 7 && max == 7)
        return digitOrderings[2];

    if (min == 8 && max == 8)
        return digitOrderings[3];

    //if the range is 6-8
    if (min == 6 && max == 8)
    {
        counter = 0;
        possibleLength = 8;
        prev = digitOrderings[3].begin();
        digit8It++;

        //checking for duplicates for digit ordering of length 8
        while (digit8It != digitOrderings[3].end())
        {
            if (nearbyint(digit8It->first / prev->first) == 1)
            {
                if (counter == 0)
                    possibleLength--;

                counter++;
            }
            prev++;
            digit8It++;
        }

        counter = 0;
        prev = digitOrderings[2].begin();
        digit7It++;

        //checking for duplicates for digit ordering of length 7
        while (digit7It != digitOrderings[2].end())
        {
            if (nearbyint(digit7It->first / prev->first) == 1)
            {
                if (counter == 0)
                    possibleLength--;

                counter++;
            }
            prev++;
            digit7It++;
        }


        if (possibleLength == 6)
            return digitOrderings[1];


        if (possibleLength == 7)
            return digitOrderings[2];

        if (possibleLength == 8 )
            return digitOrderings[3];

        digit8It = digitOrderings[3].begin();
        digit7It = digitOrderings[2].begin();

        //comparing each value of the same index number of each ordering
        //to see which one is the smallest
        while (digit6It != digitOrderings[1].end())
        {
            if (digit6It->first < digit7It->first && digit6It->first < digit8It->first)
                return digitOrderings[1];

            if (digit7It->first < digit6It->first && digit7It->first < digit8It->first)
                return digitOrderings[2];

            if (digit8It->first < digit6It->first && digit8It->first < digit7It->first)
                return digitOrderings[3];

            if (digit6It->first == digit7It->first && digit6It->first == digit8It->first && digit7It->first == digit8It->first)
            {
                digit6It++;
                digit7It++;
                digit8It++;
            }

            //if first value of digit ordering 6 us equal to first value of
            //digit ordering 7
            else if (digit6It->first == digit7It->first)
            {
                while (digit6It != digitOrderings[1].end())
                {
                    if (digit6It->first < digit7It->first)
                        return digitOrderings[1];

                    else if (digit7It->first < digit6It->first)
                        return digitOrderings[2];

                    else if (digit6It->first == digit7It->first)
                    {
                        digit6It++;
                        digit7It++;
                    }
                }

                return digitOrderings[2];
            }

            //if first value of digit ordering 6 us equal to first value of
            //digit ordering 8
            else if (digit6It->first == digit8It->first)
            {
                while (digit6It != digitOrderings[1].end())
                {
                    if (digit6It->first < digit8It->first)
                        return digitOrderings[1];

                    else if (digit8It->first < digit6It->first)
                        return digitOrderings[3];

                    else if (digit6It->first == digit8It->first)
                    {
                        digit6It++;
                        digit8It++;
                    }
                }

                return digitOrderings[3];
            }

            //if first value of digit ordering 7 us equal to first value of
            //digit ordering 8
            else if (digit7It->first == digit8It->first)
            {
                while (digit7It != digitOrderings[2].end())
                {
                    if (digit7It->first < digit8It->first)
                        return digitOrderings[2];

                    else if (digit8It->first < digit7It->first)
                        return digitOrderings[3];

                    else if (digit7It->first == digit8It->first)
                    {
                        digit7It++;
                        digit8It++;
                    }
                }

                return digitOrderings[3];
            }
        }

        while (digit7It != digitOrderings[2].end())
        {
            if (digit7It->first < digit8It->first)
                return digitOrderings[2];

            if (digit8It->first < digit7It->first)
                return digitOrderings[3];

            else
            {
                digit7It++;
                digit8It++;
            }
        }

        return digitOrderings[3];

    }

    //checks for length 5-7
    else
    {
        counter = 0;
        possibleLength = 7;
        prev = digitOrderings[2].begin();
        digit7It++;

        //checks for duplicates
        while (digit7It != digitOrderings[2].end())
        {
            if (nearbyint(digit7It->first / prev->first) == 1)
            {
                if (counter == 0)
                    possibleLength--;

                counter++;
            }
            prev++;
            digit7It++;
        }
        counter = 0;
        prev = digitOrderings[1].begin();
        digit6It++;

        //checks for duplicates
        while (digit6It != digitOrderings[1].end())
        {
            if (nearbyint(digit6It->first / prev->first) == 1)
            {
                if (counter == 0)
                    possibleLength--;

                counter++;
            }
            prev++;
            digit6It++;
        }


        if (possibleLength == 5)
            return digitOrderings[0];

        if (possibleLength == 6)
        {
            return digitOrderings[1];
        }

        if (possibleLength == 7 )
            return digitOrderings[2];

        digit7It = digitOrderings[2].begin();
        digit6It = digitOrderings[1].begin();


        //comparing each value of the same index number of each ordering
        //to see which one is the smallest
        while (digit5It != digitOrderings[0].end())
        {
            if (digit5It->first < digit6It->first && digit5It->first < digit7It->first)
                return digitOrderings[0];

            if (digit6It->first < digit5It->first && digit6It->first < digit7It->first)
                return digitOrderings[1];

            if (digit7It->first < digit5It->first && digit7It->first < digit6It->first)
                return digitOrderings[2];

            if (digit5It->first == digit6It->first && digit5It->first == digit7It->first && digit6It->first == digit7It->first)
            {
                digit5It++;
                digit6It++;
                digit7It++;
            }

            else if (digit5It->first == digit6It->first)
            {
                while (digit5It != digitOrderings[0].end())
                {
                    if (digit5It->first < digit6It->first)
                        return digitOrderings[0];

                    else if (digit6It->first < digit5It->first)
                        return digitOrderings[1];

                    else if (digit5It->first == digit6It->first)
                    {
                        digit5It++;
                        digit6It++;
                    }
                }

                return digitOrderings[1];
            }

            else if (digit5It->first == digit7It->first)
            {
                while (digit5It != digitOrderings[0].end())
                {

                    if (digit5It->first < digit7It->first)
                        return digitOrderings[0];

                    else if (digit7It->first < digit5It->first)
                        return digitOrderings[2];

                    else if (digit5It->first == digit7It->first)
                    {
                        digit5It++;
                        digit7It++;
                    }
                }

                return digitOrderings[2];
            }

            else if (digit6It->first == digit7It->first)
            {
                while (digit6It != digitOrderings[1].end())
                {

                    if (digit6It->first < digit7It->first)
                        return digitOrderings[1];

                    else if (digit7It->first < digit6It->first)
                        return digitOrderings[2];

                    else if (digit6It->first == digit7It->first)
                    {
                        digit6It++;
                        digit7It++;
                    }
                }

                return digitOrderings[2];
            }
        }

        while (digit6It != digitOrderings[1].end())
        {
            if (digit6It->first < digit7It->first)
                return digitOrderings[1];

            if (digit7It->first < digit6It->first)
                return digitOrderings[2];

            else
            {
                digit6It++;
                digit7It++;
            }
        }
        return digitOrderings[2];
    }
}
