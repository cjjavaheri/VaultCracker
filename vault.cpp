/***************************************************************************//**
 * @file
 ******************************************************************************/
#include "vault.h"
#include <cmath>
#include <fstream>
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
    while ((rand() % 100) > 50) pwLen++;
    pwLen = 4;
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
    for (unsigned int i = 0 ; i < pwLen ; i++)
    {
        char ch = validChars[rand() % 26]; // Original is 74. Changed for testing
        password += ch;
    }
    password = "pass";
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
    double score = 0.0;
    Response response(WAITING, 100.0);
    cout << "Vault::sendPassword(" << guess << ")\n";
    ofstream fout;
    fout.open("plot.dat",ios::app);
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


    if(guess==password)
        {response=Response(ACCEPTED,score);
         return response;}
    
    else if (guess.length() == password.length())
    {
        for (i = 0; i < password.length(); i++)
        {
            score += abs(guess.at(i) - password.at(i));
//            if (password.at(i) == guess.at(i))
//            {
//                correct = true;
//            }
//            else
//            {
//                correct = false;
//                break;
//            }
        }
        //sending score and the guess to a file plot.data for graphing
        cout << "In loop" << endl;
        if(fout.is_open()){fout<<guess<<" " <<score<<endl;}
    }
    
    if(fout.is_open()){fout.close();}
    response = Response(WAITING, score);

    return response;
}
