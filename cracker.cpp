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
    map<string, long double> FileMap;
    map<string, long double>::iterator fit;
    unsigned int fileCounter = 0;
    Response response;
    ofstream fout;

    unsigned int min=0;
    unsigned int max=0;

    checkLength(min, max);
    // Change base in order to change number of characters used.
    double base = 74;
    unsigned int length = 4;
    long long int g1 = 0;
    long long int g2 = pow(base, length) - 1;

    fout.open("plot.dat");

    // Here you will generate your password guesses
    // Once generated, you send the prospective password to the game system
    // through the bound callback function 'sendPassword'
    response=sendPassword(truePassword);

	
	 // Brute force the password.
    if (max<5)
    {
        truePassword = bruteForce(min, max, base, FileMap, response);
        cout<<"\nafter true password "<<response.score<<" " <<truePassword<<"\n";
    }

	  //if (max == 4 && response.rc != ACCEPTED)
   		// truePassword = binarySearch(length, base, response, g1, g2, FileMap);




    //Outputting data to plot.dat for visualization.
    fit = FileMap.begin();
    while (fit != FileMap.end())
    {
        cout << fileCounter << " " << fit->first << " " << fit->second << endl;
        fout << fileCounter << " " <<  fit->second << endl;
        fileCounter++;
        fit++;
    }

    fout.close();


     cout << "True password: " << truePassword << endl;

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

string Cracker::bruteForce(unsigned int min, unsigned int max, double base, map<string, long double> &FileMap, Response &response)
{
    string guess = "";
    long long int i;

    if(max==0)
    {
        cout<<"\nThere is no password\n";
        return guess;
    }

	// original: j <= max, j=min
    for (unsigned int j=3; j<=3; j++)
    {
  		
        for (i = 0; i < int(pow(base, j)); i++)
        {
            cout<<endl<<guess<<endl;
            response = sendPassword(guess);
            if (response.rc == ACCEPTED)
            {
                if (guess == "")
                {
                    cout << "\nThere is no password\n";
                }
                else
                {
                    cout<<"\nreturns guess\n score is "<<response.score<<"\n";
                }
                FileMap.insert({guess, response.score});
                cout<<"\nguess is "<<guess<<" " <<"response.score is "<<response.score<<"\n";
                cout << ResponseMsg[response.rc] << " ";
                return guess;
            }
            		
            guess = getGuess(i, j, base);
            FileMap.insert({guess, response.score});
            cout << ResponseMsg[response.rc] << " ";
      
        }
    }
    return guess;

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
    static map<int, char> CharacterMap;
    map<int, char>::iterator cit;
    int i;
    unsigned int digit;
    static char nextChar = 'a';
    string guess = "";
    unsigned int counter;

    if (CharacterMap.empty())
    {
        for (i = 0; i < 26; i++)
        {
            CharacterMap.insert({ i, nextChar });
            nextChar += 1;
        }
        nextChar = 'A';
        for (i = 26; i < 52; i++)
        {
            CharacterMap.insert({ i, nextChar });
            nextChar += 1;
        }
        CharacterMap.insert({ 52, '!' });
        CharacterMap.insert({ 53, '@' });
        CharacterMap.insert({ 54, '#' });
        CharacterMap.insert({ 55, '$' });
        CharacterMap.insert({ 56, '%' });
        CharacterMap.insert({ 57, '^' });
        CharacterMap.insert({ 58, '&' });
        CharacterMap.insert({ 59, '*' });
        CharacterMap.insert({ 60, '(' });
        CharacterMap.insert({ 61, ')' });
        CharacterMap.insert({ 62, '_' });
        CharacterMap.insert({ 63, '+' });
        CharacterMap.insert({ 64, '=' });
        CharacterMap.insert({ 65, ':' });
        CharacterMap.insert({ 66, ';' });
        CharacterMap.insert({ 67, '~' });
        CharacterMap.insert({ 68, '?' });
        CharacterMap.insert({ 69, '.' });
        CharacterMap.insert({ 70, '<' });
        CharacterMap.insert({ 71, '>' });
        CharacterMap.insert({ 72, ']' });
        CharacterMap.insert({ 73, '[' });
    }

    counter = 1;
    guess.resize(length);
    while (counter < length + 1)
    {
        digit = int(value / pow(base, length - counter));
        cit = CharacterMap.find(digit);
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

    for (i = guess.length() - 1; i > -1; i--, j++)
    {
        cit = CharacterMap.find(guess[i]);
        sum += (cit->second * pow(base, j));
    }

    return sum;
}

string Cracker::binarySearch(int length, double base, Response &response, long long int g1,
                             long long int  g2, map<string, long double> &FileMap)
{
    long long int g3 = ceil((g1 + g2) / 2.0);
    string guess1;
    string guess2;
    string guess3;
    string temp = "";
    long long int s1;
    long long int s2;
    long long int s3;

    guess1 = getGuess(g1, length, base);
    guess2 = getGuess(g2, length, base);
    guess3 = getGuess(g3, length, base);


    response = sendPassword(guess1);
    if (response.rc == ACCEPTED)
    {
        FileMap.insert({guess1, s1});
        return guess1;
    }
    s1 = response.score;
    response = sendPassword(guess2);
    if (response.rc == ACCEPTED)
    {
        FileMap.insert({guess2, s2});
        return guess2;
    }
    s2 = response.score;
    response = sendPassword(guess3);
    if (response.rc == ACCEPTED)
    {
        FileMap.insert({guess3, s3});
        return guess3;
    }
    s3 = response.score;

    FileMap.insert({guess1, s1});
    FileMap.insert({guess2, s2});
    FileMap.insert({guess3, s3});


	// s1 is the min value
	if (s1 <= s3 && s1 <= s2)
	{

		return binarySearch(length, base, response, g1 + 1, g3 - 1, FileMap);
	}

	//s2 is the min value
	else if (s2 <= s3 && s2 <= s1)
	{

		return binarySearch(length, base, response, g3 + 1, g2 - 1, FileMap);
	}


	//s3 is the min value
	else
	{


		if (s1 <= s2)
			return binarySearch(length, base, response, g3 - 1, g1 + 1, FileMap);

		else
			return binarySearch(length, base, response, g3 + 1, g2 - 1, FileMap);
	}





}


char Cracker::getNextChar(char some_char, double base)
{
        map<char, int> CharacterMap;
	map<char,int>::iterator cit;

	CharacterMap = getMap();


	cit = CharacterMap.find(some_char);
	if (cit->second == base - 1)
	{
		return 'a';
	}
	else
	{
		cit++;
		return cit->first;
	}

}

char Cracker::getPrevChar(char some_char, double base)
{
        map<char, int> CharacterMap;
	map<char,int>::iterator cit;
	int i;

	CharacterMap = getMap();


	cit = CharacterMap.find(some_char);
	if (cit->second == 0)
	{
		for (i = 0; i < base; i++)
			cit++;

		return cit->first;
	}
	else
	{
		cit--;
		return cit->first;
	}

}

map<char,int> Cracker::getMap()
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
