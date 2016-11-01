#ifndef __RESPONSE_H
#define __RESPONSE_H
#include <string>
#include <tuple>

using namespace std;

enum ResponseCode {WAITING, PW_TOO_LONG, PW_TOO_SHORT, ACCEPTED, TIME_LIMIT};

typedef tuple<unsigned int, unsigned int> Range;

const string ResponseMsg[] =
{
    "Waiting for Credentials",
    "Password Too Long",
    "Password Too Short",
    "Password Accepted",
    "Time Limit Exceeded",
};

const string validChars = {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+=:;~?.<>]["};

struct Response
{
    ResponseCode rc;
    long double score;

    Response(ResponseCode responseCode=WAITING, long double sc=0.0) : rc(responseCode), score(sc) {}
};

#endif
