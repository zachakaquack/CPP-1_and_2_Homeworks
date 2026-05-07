#pragma once

#include "destination.h"
#include <iostream>

using namespace std;

// all console related things
namespace Console {

// beings the main loop of prompting. calculation will begin when this function
// ends
vector<Destination> getDestinations();

// prompting stuffs
// prompt for name
bool getName(string &name, const string &question);

// prompt for int
bool getInt(int &value, const string &question, int const &valmin = 0,
            int const &valmax = 1000);

}; // namespace Console
