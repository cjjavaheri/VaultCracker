#include "cracker.h"
#include <time.h>
#include <vector>

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
    string guess = "aaaa";
    string truePassword = "";
    string checklen="";
    map<char, int> CharacterMap;
    map<char, int>::iterator cit;
    map<string, int> mymap;
    map<string, int>::iterator it;
    double value;
    int digit;
    int i;
    double base = 26;
    char nextChar = 'a';
    unsigned int length = 4;
    unsigned int counter;
    unsigned int fileCounter = 0;
    Response response;
    ofstream fout;


    fout.open("plot.dat");


    cout << "Cracker::getCracking()" << endl;

        // Here you will generate your password guesses
        // Once generated, you send the prospective password to the game system
        // through the bound callback function 'sendPassword'

        cout << ResponseMsg[response.rc] << " ";

    

// Start of brute force algorithm
   for (i = 0; i < 26; i++)
   {
       CharacterMap.insert({ i, nextChar });
       nextChar += 1;
   }
 
   for (i = 0; i < int(pow(base, length)); i++)
   {
       counter = 1;
       value = i;
       while (counter < length + 1)
       {
           digit = int(value / pow(base, length - counter));
           cit = CharacterMap.find(digit);
           guess[guess.size() - length + (counter - 1)] = cit->second;
           value = (value / pow(base, length - counter) - digit) * pow(base, length - counter);
           value = nearbyint(value);
           counter++;
       }
       response = sendPassword(guess);
       mymap.insert({guess, response.score});
	cout << ResponseMsg[response.rc] << " ";
	if (response.rc == ACCEPTED)
		truePassword = guess;
   }


// End of brute force algorithm.

    //Outputting data to plot.dat
    it = mymap.begin();
    while (it != mymap.end())
	{
		cout << fileCounter << " " << it->first << " " << it->second << endl;
		fout << fileCounter << " " <<  it->second << endl;
		fileCounter++;
		it++;
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
