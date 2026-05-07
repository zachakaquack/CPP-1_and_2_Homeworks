#pragma once

#include <string>
#include <vector>

using namespace std;

namespace Global {

// up (k), down (j), help, delete, and break (obv)
// vim bindings lol
// if you change these, make sure to change the accepted inputs in
// Console::drawCircuit, and also the help message
// const vector<string> CONSOLE_EXTRA_ALLOWED_INPUTS{"k", "j", "h", "d",
// "break"};
const vector<string> CONSOLE_EXTRA_ALLOWED_INPUTS{"k", "j", "h", "break"};

namespace Colors {
const string Reset = "\033[0m";
const string Directory = "\033[94m";
const string Error = "\033[31m";
const string Bold = "\033[1m";

// https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
const string TextFile = "\033[1;7m";
} // namespace Colors

} // namespace Global
