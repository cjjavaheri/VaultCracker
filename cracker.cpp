#include "cracker.h"

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
    string checklen="";


    for (unsigned int i = 0 ; i < 8 ; i++)
    {
        char ch = validChars[rand() % 74];
        guess += ch;
    }

    cout << "Cracker::getCracking()" << endl;

    unsigned int min=0;
    unsigned int max=0;

    Response getlen=sendPassword(checklen);
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
    // Here you will generate your password guesses
    // Once generated, you send the prospective password to the game system
    // through the bound callback function 'sendPassword'

    Response response = sendPassword(guess);

    cout << ResponseMsg[response.rc] << response.score << endl;
}
