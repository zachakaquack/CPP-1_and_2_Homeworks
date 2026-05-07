#pragma once

#include <string>
#include <vector>

using namespace std;

namespace Global {

const vector<string> OPENING_COMPONENTS{"parallel", "serial"};
const vector<string> LEAF_COMPONENTS{"resistor"};

// up (k), down (j), help, delete, and break (obv)
// vim bindings lol
// if you change these, make sure to change the accepted inputs in
// Console::drawCircuit, and also the help message
// const vector<string> CONSOLE_EXTRA_ALLOWED_INPUTS{"k", "j", "h", "d",
// "break"};
const vector<string> CONSOLE_EXTRA_ALLOWED_INPUTS{"k", "j", "h", "break"};

namespace Color {
const string reset = "\033[0m";
const string bold = "\033[1m";
const string red = "\033[31m";
} // namespace Color

} // namespace Global
