/***************************************************************************//**
 * @file
 ******************************************************************************/
#ifndef __BASEVAULT_H_
#define __BASEVAULT_H_
#include "response.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <thread>

using namespace std;

/***************************************************************************//**
 * @class BaseVault
 *
 * @par Description
 *    This abstract base class acts as the interface to an external system.
 ******************************************************************************/
class BaseVault
{
public:
    /// Constructor
    BaseVault();

    /// The function that will check a password guess against the actual password
    virtual Response sendPassword(string pw) = 0;
};

#endif
