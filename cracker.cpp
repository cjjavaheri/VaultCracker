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
    double base = 74; //base to convert the string in
    unsigned int length = 0;
    long long int g1 = 0; //the score on the left in Binary search
    long long int g2 = pow(base, length) - 1;	//the score on the right in binary search
    list<char> ordering;	//contains the correct character ordering

    unsigned int min=0; //minimum length of the password
    unsigned int max=0; //maximum length of the password

    //checks the range of the length of password
    checkLength(min, max);

    // Brute force the password.
     if ( min < 5)
     {
        bruteForce(min, max, base, response);
     }

    g1 = 0;
    g2 = pow(base, 8);
    // using binary search on length's greater than 4 
    if (max > 4)
    {
       truePassword = binarySearch(min, max, base, response, g1, g2);
    }

    return;

}

/***************************************************************************//**
 * @brief A brute force algorithm to find the password for lengths 1-4
 *
 * @par Description
 *    This function generates all possible permutations for a fixed password length
 * with a given base.
 *
 * @param[in] min - minimum length of the password
 * @param[in] max - maximum length of the password
 * @param[in] base - base 74
 * @param[in,out] response - The response that the vault sends back to the cracker.
 *
 * @return The actual password to the vault.
 ******************************************************************************/

void Cracker::bruteForce(unsigned int min, unsigned int max, double base, Response &response)
{
    string guess = "aaaa";

    //does the actual brute forcing for length 4 and gets response 
    findCombinations(guess, guess.size(), response);

    if (response.rc == ACCEPTED)
		return;

    guess.resize(3);
    //brute forces length 3
    findCombinations(guess, guess.size(), response);

    if (response.rc == ACCEPTED)
	return;

    //brute forces length 2
    guess.resize(2);

    findCombinations(guess, guess.size(), response);

    if (response.rc == ACCEPTED)
	return;

    //brute forces length 1
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
 * @param[in,out] min - will pass back the range minimum.
 * @param[in,out] max - will pass back the range maximum.
 ******************************************************************************/

void Cracker::checkLength(unsigned int &min, unsigned int &max)
{
    string checklen="";
    Response getlen=sendPassword(checklen);
    //for checking min and max length of password by sending a guess of a
    //specific length and getting the response
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
    static map<int, char> IntegerMap; //maps each digit with an integer
    map<int, char>::iterator cit; 
    unsigned int digit;
    string guess = "";
    unsigned int counter;

    //gets a map with integers and characters
    if (IntegerMap.empty())
        IntegerMap = getIntegerMap();

    counter = 1;
    
    //resizing guess to the length of the password guess 
    guess.resize(length);
    
    
    while (counter < length + 1)
    {
        //digit is the quotient which we use to find the correponding character
        //inside of the integer map
        digit = int(value / pow(base, length - counter));
        
        //finds the character corresponding to the digit
        cit = IntegerMap.find(digit);
        
        //placing the correct letter in the correct position of the string
        guess[guess.size() - length + (counter - 1)] = cit->second;
        
        //Integer value of the remainder
        value = (value / pow(base, length - counter) - digit) * pow(base, length - counter);
        
        //checking to make sure the remainder is an integer
        value = nearbyint(value);
        
        //incrementing counter
        counter++;
    }

    return guess;

}

/***************************************************************************//**
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
    long long int i;
    long long int j = 0;
    long long int sum = 0;
    static map<char, int> CharacterMap;
    map<char, int>::iterator cit;
    
    //maps each character to an int value
    if (CharacterMap.empty())
        CharacterMap = getCharacterMap();

    //calaulates the integer representation of the string 
    for (i = guess.length() - 1; i > -1; i--, j++)
    {
        //finds the integer representation of the character in the map
        cit = CharacterMap.find(guess[i]);
        
        //converts the string to base 74
        sum += (cit->second * pow(base, j));
    }

    return sum;
}

/***************************************************************************//**
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
string Cracker::binarySearch(int smallestLength, int largestLength, double base, Response &response, long long int g1, long long int  g2)
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
    int counter = 0;
    int length;

    //checking the minimum length to set the minimum length of the password
    if (smallestLength <= 5)
	length = 5;

    if (smallestLength == 6)
	length = 6;

    //sets multiplier for each length
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

    //runs till password is found
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
            
            //the very next value of the minimun
            g3Next = getGuess(g3 + 1, length, base);
            
            //the previous value of the minimum
            g3Prev = getGuess(g3 - 1, length, base);
            
            //checks to see if the minimum, the next value and the prev value is the 
            //paswword
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
                initialValue = 1.0;

            }
            //searches each length 4000 times and if its not found, we increment the
            //length and restart the search
	    if (counter > 4000)
	    {
 		multiplier = multiplier - 0.001;
		counter = 0;
		value = 1;
		initialValue = 1.0;
		length++;
	   }

	   //if the password is not found within 4000 searches for the maximum length,
	   //we set length to minimum and increase the multiplier and start searching
	   //the vault again
	   if (length > largestLength)
           {
		//increasing multiplier to search a bigger space
		multiplier = sqrt(exp(multiplier));
		
                if (smallestLength > 4)
                    length = smallestLength;

                else
                    length = 5;
	   }
        }
    }

    //returns the true password
    return firstMin;
}

/***************************************************************************//**
 * @brief binarySeach 
 *
 * @par Description
 *    
 *
 * @param[in] length - length of the guess
 * @param[in] base - base 74
 * @param[in,out] response - response sent by the vault
 * @param[in] g1 - left integer representation of the string of the range to be searched
 * @param[in] g2 - right integer representation of the string of the range to be searched
 *
 * @return 
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


    else if (abs(g1 - g2) < 7)
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
list<char> ordering;
list<char>::iterator orderingIt;
unsigned int min = 0;
unsigned int max = 0;

checkLength(min, max);
guess.resize(max);

for (i = 0; i < guess.size(); i++)
{
	guess[i] = 'a';
}

for (i = 0; i < 74; i++)
{
	guess[guess.size() - 1] = validChars[i];
	response = sendPassword(guess);
	scores.insert({guess[guess.size() - 1], response.score});
}

it = scores.begin();
compare = it;
for (i = 0; i < 74; i++)
{
	difference = it->second - compare->second;
	differenceIt = scoreDifferences.find(difference);

		scoreDifferences.insert({difference, compare->first});

/*	else
		duplicates.insert({difference, compare->first});
*/	
	compare++;
}


/*	differenceIt = scoreDifferences.begin();
	duplicatesIt = duplicates.begin();
	ordering.push_back(differenceIt->second);
	guess[guess.size() - 1] = differenceIt->second;
	response = sendPassword(guess);
	pivotScore = response.score;
	differenceIt++;

*/
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
