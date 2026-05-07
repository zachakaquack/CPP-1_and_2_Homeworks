#pragma once
#include "circuit.h"
#include <string>
#include <vector>

using namespace std;
namespace Parse {
// DO NOT CALL! GO THROUGH THE NON UNDER
// assumes the lines are formatted properly
Circuit _parse(vector<string> &lines);

Circuit parse(string &contents);
Circuit parse(vector<string> &lines);
Circuit *getTypeOfCircuit(const string &line);
double getResistorOhms(const string &line);
vector<string> formatLines(string &contents, char delim = '\n');

} // namespace Parse
