#include "cracker.h"
#include <time.h>
#include <vector>

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
    string checklen="";
    map<string, int> FileMap;
    map<string, int>::iterator fit;
    unsigned int fileCounter = 0;
    Response response;
    ofstream fout;

    unsigned int min=0;
    unsigned int max=0;

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

    // Change base in order to change number of characters used.
    double base = 26;
    // Change length in order to change length of password generated.
    unsigned int length = 4;

    fout.open("plot.dat");

    // Here you will generate your password guesses
    // Once generated, you send the prospective password to the game system
    // through the bound callback function 'sendPassword'
    response=sendPassword(truePassword);
    // Brute force the password.
    if (max<5)
    {
        truePassword = bruteForce(min, max, base, FileMap, response);
    }

    //Outputting data to plot.dat for visualization.
    fit = FileMap.begin();
    while (fit != FileMap.end())
    {
        //cout << fileCounter << " " << fit->first << " " << fit->second << endl;
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

string Cracker::bruteForce(unsigned int min, unsigned int max, double base, map<string, int> &FileMap, Response &response)
{
    string guess = "";
    int i;
    
    if(max==0)
    {
        cout<<"\nThere is no password\n";
        return guess;
    }
    
    for (int j=min;j<=max;j++)
    {
    for (i = 0; i < int(pow(base, j)); i++)
    {
        response = sendPassword(guess);
        guess = getGuess(i, j, base);
        FileMap.insert({guess, response.score});
        cout << ResponseMsg[response.rc] << " ";

        if (response.rc == ACCEPTED)
            if (guess == "")
            	{cout << "\nThere is no password\n"; return guess;}
            else
            	return guess;
    }
    }
    return guess;

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


string Cracker::getGuess(double value, unsigned int length, double base)
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
