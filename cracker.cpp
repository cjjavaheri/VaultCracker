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
   string guess = "Password";

   cout << "Cracker::getCracking()" << endl;

   // Here you will generate your password guesses
   // Once generated, you send the prospective password to the game system
   // through the bound callback function 'sendPassword'

   Response response = sendPassword(guess);

   cout << ResponseMsg[response.rc] << endl;
}
