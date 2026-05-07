#include "parser.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<string> splitString(const string &str, char delim) {
    vector<string> result;
    string token;
    istringstream iss(str);

    while (getline(iss, token, delim)) {
        result.push_back(token);
    }
    return result;
}

vector<string> formatLines(vector<string> &lines) {
    int index;

    // TODO: maybe change this and second one to find_first_of and
    // find_first_not_of?

    // get rid of the beginning of the string
    for (size_t i = 0; i < lines.size(); i += 1) {
        const string &line = lines[i];
        if (!line.empty()) {
            index = i;
            break;
        }
    }
    // index = the first non empty line
    lines.erase(lines.begin(), lines.begin() + index);

    // get rid of the rest of the thing
    index = lines.size();
    for (size_t i = 0; i < lines.size(); i += 1) {
        const string &line = lines[i];
        if (line.empty()) {
            index = i;
            break;
        }
    }

    // index = the first empty line
    lines.erase(lines.begin() + index, lines.end());

    for (string line : lines) {
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        line = trim(line);
    }

    return lines;
}

// https://stackoverflow.com/a/6500499
string trim(string &str) {
    str.erase(str.find_last_not_of(' ') + 1);
    str.erase(0, str.find_first_not_of(' '));
    return str;
}
