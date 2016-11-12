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
    long long int g1;
    long long int g2;
    list<char> ordering;
    map<long double, int> digitOrdering;
    map<long double, long double> digitalOrder;
    map<long double, int>::iterator it;

    unsigned int min=0;
    unsigned int max=0;

    checkLength(min, max);

    response=sendPassword(truePassword);

    digitOrdering = findDigitOrdering(max);

    it = digitOrdering.begin();
    while (it != digitOrdering.end())
    {
        cout << it->first << " " << it->second << endl;
        it++;
    }


    cout << endl << endl << endl;

    // Brute force the password.
    if ( min < 5)
        bruteForce(min, max, base, response);

    //  Binary search
    if (max > 4 && min == max)
	{
		g1 = 0;
   	        g2 = pow(base, max);
        	truePassword = fixedBinarySearch(min, max, base, response, g1, g2);
	}

	g1 = 0;
	if (min <= 5)
		g2 = pow(base, 5);

	else if (min == 6)
		g2 = pow(base, 6);

	else if (min == 7)
		g2 = pow(base, 7);

	else
		g2 = pow(base, 8);
	truePassword = variableBinarySearch(min, max, base, response, g1, g2);

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
    string guess = "";
    unsigned int i;
    unsigned int j;
    guess.resize(min);

    j = 1;
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
    static map<long double, int> digitOrdering;
    static map<long double, int> digit5;
    static map<long double, int> digit6;
    static map<long double, int> digit7;
    static map<long double, int> digit8;
    static map<long double, int>::iterator digitIt;
    map<int, char>::iterator cit;
    unsigned int digit;
    string guess = "";
    unsigned int counter;

    if (IntegerMap.empty())
        IntegerMap = getIntegerMap();


    if (digit5.empty())
        digit5 = findDigitOrdering(5);

    if (digit6.empty())
        digit6 = findDigitOrdering(6);

    if (digit7.empty())
        digit7 = findDigitOrdering(7);

    if (digit8.empty())
        digit8 = findDigitOrdering(8);

    if (length == 5)
        digitOrdering = digit5;

    if (length == 6)
        digitOrdering = digit6;

    if (length == 7)
        digitOrdering = digit7;

    if (length == 8)
        digitOrdering = digit8;


    counter = 1;
    guess.resize(length);
    digitIt = digitOrdering.end();
    digitIt--;

    while (counter < length + 1)
    {
        digit = int(value / pow(base, length - counter));
        cit = IntegerMap.find(digit);
        guess[guess.size() - digitIt->second] = cit->second;
        value = (value / pow(base, length - counter) - digit) * pow(base, length - 			counter);
        value = nearbyint(value);
        digitIt--;
        counter++;
    }

    return guess;

}



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
    map<char, int>::iterator cit;


    if (CharacterMap.empty())
        CharacterMap = getCharacterMap();


    if (digit5.empty())
        digit5 = findDigitOrdering(5);

    if (digit6.empty())
        digit6 = findDigitOrdering(6);

    if (digit7.empty())
        digit7 = findDigitOrdering(7);

    if (digit8.empty())
        digit8 = findDigitOrdering(8);

    if (guess.size() == 5)
        digitOrdering = digit5;

    if (guess.size() == 6)
        digitOrdering = digit6;

    if (guess.size() == 7)
        digitOrdering = digit7;

    if (guess.size() == 8)
        digitOrdering = digit8;

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

string Cracker::fixedBinarySearch(int smallestLength, int largestLength, double base, Response &response, long long int g1,
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
    unsigned int length;
    long long int counter = 0;


    length = largestLength;

    if (length <= 6)
    {
        multiplier = 1.05;
    }

    else if (length == 7)
    {
        multiplier = 1.05;
    }

    else
    {
        multiplier = 1.05;
    }


    while ( response.rc != ACCEPTED)
    {
        found = false;
        while (!found)
        {
            firstMin = FindMin(length, base, response, initialValue,initialValue + value);
            counter++;
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
                    multiplier = multiplier - 0.0001;
                }
                else if (length == 7)
                {
                    multiplier = multiplier - 0.001;
                }
                else
                {
                    multiplier = multiplier - 0.0001;
                }
                value = 1;
                initialValue = 0.0;

            }

        }


    }

    return firstMin;


}


   string Cracker::variableBinarySearch(int smallestLength, int largestLength, double base, Response &response, long long int g1, long long int  g2)
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


    if (smallestLength <= 5)
	length = 5;

    else if (smallestLength == 6)
	length = 6;

    else if (smallestLength == 7)
	length = 7;

    else
	length = 8;

    if (length <= 6)
    {
        multiplier = 1.05;
    }

    else if (length == 7)
    {
        multiplier = 1.05;
    }

    else
    {
        multiplier = 1.05;
    }


    while ( response.rc != ACCEPTED)
    {
        found = false;
        while (!found)
        {
            firstMin = FindMin(length, base, response, initialValue,initialValue + value);
            counter++;
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
                    multiplier = multiplier - 0.0001;
                }
                else if (length == 7)
                {
                    multiplier = multiplier - 0.001;
                }
                else
                {
                    multiplier = multiplier - 0.0001;
                }
                value = 1;
                initialValue = 0.0;

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
            CharacterMap.insert({*orderingIt, i});
            orderingIt++;
        }
    }
    return CharacterMap;

}


map<int, char> Cracker::getIntegerMap()
{
    Response response;
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
            IntegerMap.insert({i, *orderingIt});
            orderingIt++;
        }
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



list<char> Cracker::findOrdering(Response &response)
{
    cout << "Entered character ordering function    --------------------------------- " << endl;
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

    checkLength(min, max);
    guess.resize(max);

    digitOrdering = findDigitOrdering(guess.size());
    digitIt = digitOrdering.begin();
    j = digitIt->second;

    for (i = 0; i < guess.size(); i++)
    {
        guess[i] = 'a';
    }

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
    while (differenceIt != scoreDifferences.end())
    {
        ordering.push_front(differenceIt->second);
        i++;
        differenceIt++;
    }



    cout << "The ordering is: " ;
    orderingIt = ordering.begin();
    while (orderingIt != ordering.end())
    {
        cout << *orderingIt;
        orderingIt++;
    }

    return ordering;
}


/***************************************************************************//**
 * @brief digitalOrdering function is used to find the significance of digits
 *
 * @par Description
 *    This function will test each digit for scores. The digit with the largest
 *    change in score, calculated by variance, will be the most significant.
 *    The digit with the smallest change in score will be the least significant.
 *    Everything else will be in between.
 *
 * @param[in] min - The length of the password digitalOrdering is testing
 *
 * @return Returns a map pairing password positions and their significance
 ******************************************************************************/

map<long double, long double> Cracker::digitalOrdering(const unsigned int &length)
{
    // Store the scores of all the tried passwords
    vector<long double> scores;

    // Store all the variances
    vector<long double> vars;
    for (unsigned int i = 0; i < length; i++)
    {
        vars.push_back(0);
    }

    // A map for the significance of each digit. First = Significance, Second = Position
    map<long double, long double> whateverYouWant;
    for (int x = 0; x < 100; x++)
    {
        // DOC stands for Digital Ordering Cracker, because this string will be used to find significant digits
        string doc = "";
        // Generate a random string of the called in length
        for (unsigned int j = 0; j < length; j++)
        {
            doc += validChars[rand() % 74];
        }
        // test each position within DOC
        for (unsigned int i = 0; i < doc.length(); i++)
        {
            scores.clear();
            // Record Scores of each position with all characters
            for (unsigned int k = 0; k < 74; k++)
            {
                doc[i] = validChars[k];
                scores.push_back((sendPassword(doc)).score);
            }
            // Find the average of the scores
            long double avg = 0;
            for (unsigned int k = 0; k < scores.size(); k++)
                avg += scores[k];
            avg = avg / scores.size();
            // Calculate the change in scores with variance
            long double var = 0;
            for (unsigned int k = 0; k < scores.size(); k++)
            {
                var += pow(avg - scores[k], 2);
            }
            var = var / (scores.size() - 1);
            // Add all the position tests together
            vars[i] += var;
        }
    }

    // Map the change in scores to the position
    for (unsigned int i = 0; i < length; i++)
    {
        whateverYouWant[vars[i]] = i;
    }
    // Return the map
    return whateverYouWant;
}

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

    for (i = 0; i < guess.size(); i++)
    {
        guess[i] = 'a';
    }

    for (j = 1; j <= guess.size(); j++)
    {

        for (i = 0; i < 74; i++)
        {
            guess[guess.size() - j] = validChars[i];
            response = sendPassword(guess);
            scores.insert({guess[guess.size() - j], response.score});
        }

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

