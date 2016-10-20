/***************************************************************************//**
 * @file
 ******************************************************************************/
#ifndef __BASECRACKER_H
#define __BASECRACKER_H
#include "response.h"
#include <functional>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

#define MAX_ALLOWED 24

/***************************************************************************//**
 * @class BaseCracker
 *
 * @par Description
 *    Abstract base class from which actual Cracker classes should be derived
 ******************************************************************************/
class BaseCracker
{
public:
    BaseCracker(); /*!< Constructor */

    virtual void getCracking() = 0; /*! Function to start the intrusion attempt*/

    /// A function pointer exposed so that a callback can be set
    function<Response(string)> sendPassword;
};
#endif
