#pragma once
#include "destination.h"
#include <fstream>
#include <string>

using namespace std;

namespace File {

// Used to easily unify formatting
const char g_DESTINATION_DELIMITER = '\n';
const char g_VALUE_DELIMITER = ' ';

// reads a file
string read(const string &fileName);

// load into a vector of destinations
vector<Destination> loadDestinations(const string &fileName);

// write the actual things
void write(Destination &dest, const string &fileName);
void write(vector<Destination> &destinations, const string &fileName);

} // namespace File

vector<string> splitString(const string &str, char delim);
int safeStringtoInt(const string &str);
