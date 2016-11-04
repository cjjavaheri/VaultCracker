/***************************************************************************//**
 * @file
 ******************************************************************************/
#include "vault.h"
#include <cmath>
#include <fstream>

#include <fstream>
#include <map>
#include <cmath>
/***************************************************************************//**
 * @brief Vault Constructor
 *
 * @par Description
 *    Typically you do not want to do significant work in a constructor because
 * objects can be created and destroyed at a high rate (application dependent).
 * In this case, it's unlikely that multiple Vault objects will be created
 * during a single run and the game mechanics do not lend themselves to having
 * the game system (abstraction layer) call the various Vault object's member
 * functions to do initialization, we'll bend the rule a bit.
 *
 *    Your job is to make sure that after your Vault object is instantiated,
 * it is ready to receive and respond to 'sendPassword' calls.
 ******************************************************************************/
Vault::Vault(int minLen, int maxLen)
{
    cout << "Vault::Vault starting construction\n";
    srand(time(NULL));
    setRange(minLen, maxLen);
    setPW();
    cout << "Vault::Vault finished construction\n";
}

/***************************************************************************//**
 * @brief Set the range of the acceptable password lengths
 *
 * @param[in] minLen - The minimum acceptable password length
 * @param[in] maxLen - The maximum acceptable password length
 ******************************************************************************/
void Vault::setRange(int minLen, int maxLen)
{
    range = Range(minLen, maxLen);
    pwLen = minLen;
    while (pwLen < maxLen && (rand() % 100) > 50) pwLen++;
    cout << "Vault::setRange - [" << minLen << " to " << maxLen << "] :: [" << pwLen << "]\n";
}

/***************************************************************************//**
 * @brief Set the password for this instance of the Vault
 *
 * Here is one of the places you might make changes to make your defense
 * stronger.  Combining this and your score calculator will be important.
 ******************************************************************************/
void Vault::setPW()
{
    for (int i = 0 ; i < pwLen ; i++)
    {
        char ch = validChars[rand() % 74]; // Original is 74. Changed for testing
        password += ch;
    }

	password = "]]]";
    cout << "Generated a random password\n" << password << endl;
}

/***************************************************************************//**
 * @brief A function called by an external function trying to gain access
 *
 * @param[in] guess - The attempted password
 *
 * @return A response object containing a ResponseCode and a score
 ******************************************************************************/
Response Vault::sendPassword(string guess)
{

    unsigned int i;
    // bool correct = false;
    long double score = 0.0;
    long double pass;
    Response response(WAITING, 100.0);
    cout << "Vault::sendPassword(" << guess << ")\n";
    
    

// Here is where you will take the password guess and compare it with the
// password that you created.  If the guess is correct then you are required
// to create a Response object with the ResponseCode field set to ACCEPTED
//
// If the guess is not correct, you must return a Response object that
// has a ResponseCode field set to WAITING.  The Response object must also
// have the score field set.  See problem writeup for details and restrictions
// for score.

// The generation of the score given a guess vs the actual password is going
// to be an important part of your defense.  Think about how this will impact
// how easy or hard it will be to guess your password.



								/*
    long long int psum=0;
    long long int gsum=0;

    for (i = 0; i < password.length(); i++)
        psum=psum*74 + validChars.find(password[i]) + 1;
    for (i = 0; i < guess.length(); i++)
        gsum = gsum*74 + validChars.find(guess[i]) + 1;

    score=abs(gsum-psum);
    //sending score and the guess to a file plot.data for graphing

    if (score == 0)
    {
        response = Response(ACCEPTED, score);
        cout<<"\nscore is "<<score<<endl;
    }
    else
        response = Response(WAITING, score);

				*/
	pass = getPassword(guess, 74);
	cout<<endl<<pass<<endl;
	pass=pass*6.28318530718;
	
	score = sin(double(pass));
	if(guess==password)
        {return Response(ACCEPTED, score);}
        
	response = Response(WAITING, score);
	

    return response;
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


string Vault::getGuess(long double value, unsigned int length, double base)
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

long double Vault::getPassword(string guess, double base)
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
    
    cout<<endl<<sum<<endl;
    //(long double)(sum)=((long double)(sum))/((long double)(pow(74,4)));
    //cout<<endl<<sum<<endl;
    return (long double)(sum)/((long double)(pow(74,3)));
}
