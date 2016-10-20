#include "game.h"

/***************************************************************************//**
 * @brief Constructor
 *
 * @param[in] v - The vault object
 * @param[in] minLen - The minimum password length acceptable
 * @param[in] maxLen - The maximum password length acceptable
 ******************************************************************************/
Game::Game(BaseVault *v, int minLen, int maxLen) : vault(v), range(Range(minLen,maxLen)) {}

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
    if (str.length() < get<0>(range))
        return Response(PW_TOO_SHORT, 0.0);

    if (str.length() > get<1>(range))
        return Response(PW_TOO_LONG, 0.0);

    // Check for time limit exceeded here

    // Stop the timer related to cracker computation
    Response response = vault->sendPassword(str);
    // Start the timer related to cracker computation

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

    // Start the timer
    cracker->getCracking();
}
