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
    string guess = "";
    string truePassword = "";
    string checklen="";
    map<char, int> CharacterMap;
    map<char, int>::iterator cit;
    map<string, int> FileMap;
    map<string, int>::iterator fit;
    double value;
    int digit;
    int i;
    // Change base in order to change number of characters used.
    double base = 26;
    char nextChar = 'a';
    // Change length in order to change length of password generated.
    unsigned int length = 4;
    unsigned int counter;
    unsigned int fileCounter = 0;
    guess.resize(length);
    Response response;
    ofstream fout;


    fout.open("plot.dat");


    cout << "Cracker::getCracking()" << endl;

        // Here you will generate your password guesses
        // Once generated, you send the prospective password to the game system
        // through the bound callback function 'sendPassword'

        cout << ResponseMsg[response.rc] << " ";

    

// Start of brute force algorithm


   for (i = 0; i < int(pow(base, length)); i++)
   {
	guess = getGuess(i, length, base);
	response = sendPassword(guess);
	FileMap.insert({guess, response.score});
	cout << ResponseMsg[response.rc] << " ";

	if (response.rc == ACCEPTED)
		truePassword = guess;
   }



// End of brute force algorithm.

    //Outputting data to plot.dat
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
   /* unsigned int min=0;
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
    }*/
}







string getGuess(double value, unsigned int length, double base)
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
