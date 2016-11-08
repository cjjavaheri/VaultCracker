#include "game.h"

/***************************************************************************//**
 * @brief Constructor
 *
 * @param[in] v - The vault object
 * @param[in] minLen - The minimum password length acceptable
 * @param[in] maxLen - The maximum password length acceptable
 ******************************************************************************/
Game::Game(BaseVault *v, int minLen, int maxLen, double maxTime) : vault(v), range(Range(minLen,maxLen)), vaultTime(0), crackTime(0), timeLimit(maxTime) {}

/***************************************************************************//**
 * @brief A passthrough method that links Cracker and Vault
 *
 * @par Description
 *    This function is called by the Cracker module and calls the Vault
 * module.
 *
 * @param[in] str - The password guess sent from the Cracker object
 *
 * @return - The response to be returned to the Cracker object.
 *
 *  * ResponseCode::PW_TOO_SHORT if the attempted password is too short
 *  * ResponseCode::PW_TOO_LONG if the attempted password is too long
 *  * ResponseCode::TIME_LIMIT if the cracking attempt has exceeded the time
 *                  limit.
 *  * Response - This is a pass through response from the Vault
 ******************************************************************************/
Response Game::sendPassword(string str)
{
#ifndef GAME_TIME
   static unsigned int count = 0;
   static ofstream fout("plot.dat", std::ofstream::app);
#endif
   if (str.length() < get<0>(range))
      return Response(PW_TOO_SHORT, 0.0);

   if (str.length() > get<1>(range))
      return Response(PW_TOO_LONG, 0.0);

   // Check for time limit exceeded here
  
   // Stop the timer related to cracker computation, start the Vault timer
   startVault = chrono::system_clock::now(); 
   crackTime += (startVault - startCrack);

   Response response = vault->sendPassword(str);

#ifndef GAME_TIME
   fout << str << " " << count++ << " " << response.score << endl;
#endif

   startCrack = chrono::system_clock::now();
   vaultTime += (startCrack - startVault);

#ifdef GAME_TIME
   if (response.rc == ACCEPTED)
   {
      cout << "Password found! :: " << str << endl;
      cout << "Cracker Elapsed time " << crackTime.count() << " seconds\n";
      cout << "Vault Elapsed time " << vaultTime.count() << " seconds\n";
      cout << "Shutting down . . . " << endl;
      exit(-1);
   }
   if (crackTime.count() > timeLimit)
   {
      cout << "Cracker Elapsed time " << crackTime.count() << " seconds\n";
      cout << "Vault Elapsed time " << vaultTime.count() << " seconds\n";
      cout << "Time limit exceeded . . . Shutting down\n";
      exit(-1);
   }
#else // If it's not GAME_TIME then report success but continue recording
   if (response.rc == ACCEPTED)
   {
      cout << "Password found at :: " << str << endl;
      cout << "Cracker Elapsed time " << crackTime.count() << " seconds\n";
      cout << "Vault Elapsed time " << vaultTime.count() << " seconds\n";
   } 
#endif
   // Start the timer related to cracker computation, stop the Vault timer

   return response;
}

/***************************************************************************//**
 * @brief Set the callback function
 *
 * @param[in] function<Response(string)> - callback to be bound to
 ******************************************************************************/
void Game::sendPasswordFunc(function<Response(string)>& callback)
{
   callback = bind(&Game::sendPassword, this, _1);
}

void Game::startGame(BaseCracker *c)
{
   cracker = c;

   startCrack = chrono::system_clock::now();
   // Start the timer
   cracker->getCracking();
}
