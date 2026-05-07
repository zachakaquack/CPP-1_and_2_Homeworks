#pragma once

#include <iostream>
#include <stack>
#include <vector>

using namespace std;

namespace File {

string read(const string &fileName);

void write(const string &path, const string &contents);

} // namespace File

vector<string> splitString(const string &str, char delim);
