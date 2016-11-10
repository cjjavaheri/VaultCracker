#include "cracker.h"
#include <time.h>
#include <vector>
#include <fstream>
#include <map>
#include <cmath>

string getGuess(double value, unsigned int length, double base);

/***************************************************************************//**
 * @brief Constructor
 ******************************************************************************/
Cracker::Cracker() {}

/***************************************************************************//**
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
    //map<long long int, long double> FileMap;
    //map<long long int, long double>::iterator fit;
    string globalMin = "";
    Response response;
    double base = 74;
    unsigned int length = 0;
    long long int g1 = 0;
    long long int g2 = pow(base, length) - 1;

    unsigned int min=0;
    unsigned int max=0;

    checkLength(min, max);

    cout << "Min: " << min << endl;
    cout << "Max: " << max << endl;

    if (min == max)
        length = max;

    response=sendPassword(truePassword);

    // Brute force the password.
     if ( min < 5)
     {
        bruteForce(min, max, base, response);
     }

    // Binary search
     else { truePassword = binarySearch(length, base, response, g1, g2);}

    // Character reordering
    //truePassword = FindSingleMin(response, length);

   // cout << "True password: " << truePassword << endl;

	return;

}

/***************************************************************************//**
 * @brief A brute force algorithm to find the password.
 *
 * @par Description
 *    This function generates all possible permutations for a fixed password length
 * with a given base.
 *
 * @param[in] length - The length of the password.
 * @param[in] base - The number of characters used in the guesses.
 * @param[in,out] FileMap - A map containing all of the guesses in order to
 * generate a graph for the permuatations as a visualization.
 * @param[in,out] response - The response that the vault sends back to the cracker.
 *
 * @return The actual password to the vault.
 ******************************************************************************/

void Cracker::bruteForce(unsigned int min, unsigned int max, double base, Response &response)
{
    string guess = "aaaa";

    findCombinations(guess, guess.size(), response);

    if (response.rc == ACCEPTED)
		return;

    guess.resize(3);
   
    findCombinations(guess, guess.size(), response);

    if (response.rc == ACCEPTED)
	return;

    guess.resize(2);

    findCombinations(guess, guess.size(), response);

    if (response.rc == ACCEPTED)
	return;

    guess.resize(1);

    findCombinations(guess, guess.size(), response);

    if (response.rc == ACCEPTED)
	return;

}

/***************************************************************************//**
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
 * param[in] base - The number of characters to use in the guess.
 *
 * @return A string which is the permuatation represented by the value sent in
 * by the user.
 ******************************************************************************/


string Cracker::getGuess(long double value, unsigned int length, double base)
{
    static map<int, char> IntegerMap;
    map<int, char>::iterator cit;
    unsigned int digit;
    string guess = "";
    unsigned int counter;

    if (IntegerMap.empty())
        IntegerMap = getIntegerMap();

    counter = 1;
    guess.resize(length);
    while (counter < length + 1)
    {
        digit = int(value / pow(base, length - counter));
        cit = IntegerMap.find(digit);
        guess[guess.size() - length + (counter - 1)] = cit->second;
        value = (value / pow(base, length - counter) - digit) * pow(base, length - counter);
        value = nearbyint(value);
        counter++;
    }

    return guess;

}

long long int Cracker::getPassword(string guess, double base)
{
    long long int i;
    long long int j = 0;
    long long int sum = 0;
    static map<char, int> CharacterMap;
    map<char, int>::iterator cit;
    if (CharacterMap.empty())
        CharacterMap = getCharacterMap();

    for (i = guess.length() - 1; i > -1; i--, j++)
    {
        cit = CharacterMap.find(guess[i]);
        sum += (cit->second * pow(base, j));
    }

    return sum;
}

string Cracker::binarySearch(int length, double base, Response &response, long long int g1,
                             long long int  g2)
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

    if (length <= 6)
    {
        multiplier = 1.10;
    }

    else if (length == 7)
    {
        multiplier = 1.10;
    }

    else
    {
        multiplier = 1.10;
    }


    while ( response.rc != ACCEPTED)
    {
        found = false;
        while (!found)
        {
            firstMin = FindMin(length, base, response, initialValue,initialValue + value);
            initialValue = initialValue + value;
            value *= multiplier;
            g3 = getPassword(firstMin, base);
            g3Next = getGuess(g3 + 1, length, base);
            g3Prev = getGuess(g3 - 1, length, base);
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
            if (g3Score < g3NextScore && g3Score < g3PrevScore)
            {
                found = true;
                value = 1;
                initialValue = g3;
            }

            if (initialValue > pow(base, length) - 1)
            {
                found = true;
                if (length <= 6)
                {
                    multiplier = multiplier - .05;
                }
                else if (length == 7)
                {
                    multiplier -= 0.02;
                }
                else
                {
                    multiplier -= 0.02;
                }
                value = 1;
                initialValue = 1.0;

            }

        }


    }


    return firstMin;


}

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

    g3Next = getGuess(g3 + 1, length, base);
    g3Prev = getGuess(g3 - 1, length, base);
    response = sendPassword(g3Next);
    if (response.rc == ACCEPTED)
        return g3Next;
    g3NextScore = response.score;
    response = sendPassword(g3Prev);
    if (response.rc == ACCEPTED)
        return g3Prev;
    g3PrevScore = response.score;


    guess1 = getGuess(g1, length, base);
    guess2 = getGuess(g2, length, base);
    guess3 = getGuess(g3, length, base);


    //  cout << guess3 << endl;

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


/*string Cracker::FindMax(int length, double base, Response &response, long long int g1,
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

    guess1 = getGuess(g1, length, base);
    guess2 = getGuess(g2, length, base);
    guess3 = getGuess(g3, length, base);



    if (g3 + 1 == pow(base, length))
        return empty;

    if (g3 - 1 < 0)
        return empty;


    g3Next = getGuess(g3 + 1, length, base);
    g3Prev = getGuess(g3 - 1, length, base);


    response = sendPassword(g3Next);
    g3NextScore = response.score;
    response = sendPassword(g3Prev);
    g3PrevScore = response.score;


    response = sendPassword(guess1);
    s1 = response.score;
    response = sendPassword(guess2);
    s2 = response.score;
    response = sendPassword(guess3);
    s3 = response.score;




 //   FileMap.insert({getPassword(guess1, base),  s1});
   // FileMap.insert({getPassword(guess2, base), s2});
   // FileMap.insert({getPassword(guess3, base), s3});



    if (g3NextScore < s3 && g3PrevScore < s3)
        return guess3;


    // s1 is the max value
    else if (s1 >= s3 && s1 >= s2)
    {

        return FindMax(length, base, response, g1 + 1, g3 - 1);
    }

    //s2 is the max value
    else if (s2 >= s3 && s2 >= s1)
    {

        return FindMax(length, base, response, g3 + 1, g2 - 1);
    }


    //s3 is the max value
    else
    {


        if (s1 >= s2)
            return FindMax(length, base, response, g3 - 1, g1 + 1);

        else
            return FindMax(length, base, response, g3 + 1, g2 - 1);
    }

}*/



map<char,int> Cracker::getCharacterMap()
{
    static map<char, int> CharacterMap;
    int i;
    char nextChar = 'a';

    if (CharacterMap.empty())
    {
        for (i = 0; i < 26; i++)
        {
            CharacterMap.insert({ nextChar, i });
            nextChar += 1;
        }
        nextChar = 'A';
        for (i = 26; i < 52; i++)
        {
            CharacterMap.insert({ nextChar, i });
            nextChar += 1;
        }
        CharacterMap.insert({ '!', 52 });
        CharacterMap.insert({ '@', 53 });
        CharacterMap.insert({ '#', 54 });
        CharacterMap.insert({ '$', 55 });
        CharacterMap.insert({ '%', 56 });
        CharacterMap.insert({ '^', 57 });
        CharacterMap.insert({ '&', 58 });
        CharacterMap.insert({ '*', 59 });
        CharacterMap.insert({ '(', 60 });
        CharacterMap.insert({ ')', 61 });
        CharacterMap.insert({ '_', 62 });
        CharacterMap.insert({ '+', 63 });
        CharacterMap.insert({ '=', 64 });
        CharacterMap.insert({ ':', 65 });
        CharacterMap.insert({ ';', 66 });
        CharacterMap.insert({ '~', 67 });
        CharacterMap.insert({ '?', 68 });
        CharacterMap.insert({ '.', 69 });
        CharacterMap.insert({ '<', 70 });
        CharacterMap.insert({ '>', 71 });
        CharacterMap.insert({ ']', 72 });
        CharacterMap.insert({ '[', 73 });
    }

    return CharacterMap;

}


map<int, char> Cracker::getIntegerMap()
{

    static map<int, char> IntegerMap;
    map<int, char>::iterator cit;
    int i;
    static char nextChar = 'a';
    if (IntegerMap.empty())
    {
        for (i = 0; i < 26; i++)
        {
            IntegerMap.insert({ i, nextChar });
            nextChar += 1;
        }
        nextChar = 'A';
        for (i = 26; i < 52; i++)
        {
            IntegerMap.insert({ i, nextChar });
            nextChar += 1;
        }
        IntegerMap.insert({ 52, '!' });
        IntegerMap.insert({ 53, '@' });
        IntegerMap.insert({ 54, '#' });
        IntegerMap.insert({ 55, '$' });
        IntegerMap.insert({ 56, '%' });
        IntegerMap.insert({ 57, '^' });
        IntegerMap.insert({ 58, '&' });
        IntegerMap.insert({ 59, '*' });
        IntegerMap.insert({ 60, '(' });
        IntegerMap.insert({ 61, ')' });
        IntegerMap.insert({ 62, '_' });
        IntegerMap.insert({ 63, '+' });
        IntegerMap.insert({ 64, '=' });
        IntegerMap.insert({ 65, ':' });
        IntegerMap.insert({ 66, ';' });
        IntegerMap.insert({ 67, '~' });
        IntegerMap.insert({ 68, '?' });
        IntegerMap.insert({ 69, '.' });
        IntegerMap.insert({ 70, '<' });
        IntegerMap.insert({ 71, '>' });
        IntegerMap.insert({ 72, ']' });
        IntegerMap.insert({ 73, '[' });
    }

    return IntegerMap;
}


void Cracker::findCombinations(string guess, int length, Response &response)
{
    int i;
    int j;
    int k;
    int l;
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


string Cracker::FindSingleMin(Response &response, unsigned int length)
{
    string guess = "";
    long double score;
    map<long double, char> mymap;
    map<long double, char>::iterator it;
    long long int g1;
    long long int g2;
    int i;
    int j = 0;

    guess.resize(length);
    for (i = 0; i < length; i++)
    {
        guess[i] = 'a';
    }
    response = sendPassword(guess);
    score = response.score;
    while (j < guess.size())
    {
        for (i = 0; i < 74; i++)
        {
            guess[j] = validChars[i];
            response = sendPassword(guess);
            if (response.rc == ACCEPTED)
                return guess;
            if (response.score < score)
            {
                mymap.insert({response.score, guess[j]});
            }

        }
        it = mymap.begin();
        score = it->first;
        guess[j] = it->second;
        j++;
    }


}
