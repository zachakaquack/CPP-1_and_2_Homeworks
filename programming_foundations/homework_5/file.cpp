#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "file.h"

using namespace std;

string File::read(const string &fileName) {

    // streams
    ifstream fileIn(fileName, ios::in);
    ostringstream os;

    if (!fileIn) {
        cerr << "File \"" << fileName << "\" failed to read, or file not found!"
             << '\n';
        return "";
    }

    // reading
    os << fileIn.rdbuf();
    fileIn.close();

    return os.str();
}

void File::write(const string &path, const string &contents) {
    ofstream file(path);
    file << contents;
    file.close();
}

vector<string> splitString(const string &str, char delim) {
    vector<string> result;
    string token;
    istringstream iss(str);

    while (getline(iss, token, delim)) {
        result.push_back(token);
    }
    return result;
}
