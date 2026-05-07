#include "files.h"
#include "destination.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

namespace File {

string read(const string &fileName) {

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

// load into a vector of destinations
vector<Destination> loadDestinations(const string &fileName) {

    vector<Destination> destinations;
    string contents = read(fileName);

    cout << "\nLoading from: " << fileName << "...\n";

    // if there is an invalid destination, only set if one isnt set already

    if (contents.empty())
        return destinations;

    // split by semicolon and add to destinations
    vector<string> splitNewLine = splitString(contents, g_DESTINATION_DELIMITER);
    
    for (string split : splitNewLine) {
        if(destinations.size() > 101) {
            cout << "Destinations limit reached, ignoring further entries."<< '\n';
            return destinations;
        }

        // check for an uncompleted line
        if (split.empty())
            continue;

        vector<string> splitSpace = splitString(split, g_VALUE_DELIMITER);

        // check for (name x y)
        if (splitSpace.size() == 2) {
            if (destinations.size() != 0 && destinations[0].name() == "!depot") {
                cout << " \033[31mMalformed Line Found: " << split << ';' << '\n';
                cout << " Ignoring because a depot is already set!\033[0m" << '\n';
                continue;
            }

            cerr << " \033[31mMalformed Line Found: " << split << "\033[0m" << '\n';
            int x = safeStringtoInt(splitSpace[0]);
            int y = safeStringtoInt(splitSpace[1]);
            Location depot{x, y};
            destinations.emplace(destinations.begin(), "!depot", depot);
            cout << " Treating it as depot: " << depot << '\n';
            continue;
        }

        const int SPLIT_COUNT = 3;
        if (splitSpace.size() != SPLIT_COUNT) {
            cerr << " \033[31mMalformed Line Found: " << split << "\033[0m" << '\n';
            const string INVALID_NAME = "invalid";
            const int INVALID_LOC = -1;
            destinations.emplace_back(INVALID_NAME, INVALID_LOC, INVALID_LOC);
            continue;
        }

        string name = splitSpace[0];

        // attempt to convert into an int; if not do -1.
        // though keep the other value
        int x = safeStringtoInt(splitSpace[1]);
        int y = safeStringtoInt(splitSpace[2]);

        // depot check
        if (destinations.size() == 0 || (name == "!depot" && destinations[0].name() != "!depot")) {
            destinations.emplace(destinations.begin(), name, x, y);
            continue;
        } else if(name == "!depot") {
            cout << " \033[31mDepot Found: " << split << ';' << '\n';
            cout << " Ignoring because a depot is already set!\033[0m" << '\n';
            continue;
        }

        if (isDuplicate(destinations, name, x, y)) {
            continue;
        }

        // construct here
        destinations.emplace_back(name, x, y);
    }
    cout << '\n';

    return destinations;
}

void write(Destination &dest, const string &fileName) {
    ofstream file(fileName);
    file << dest;
    file.close();
}

void write(vector<Destination> &destinations, const string &fileName) {
    ofstream file(fileName);

    for (Destination dest : destinations) {
        file << dest;
    }
    file.close();
}

} // namespace File

// split string by delimiter
vector<string> splitString(const string &str, char delim) {
    vector<string> result;
    string token;
    istringstream iss(str);

    // read characters from stream into token until delim. when delim found,
    // append the token, and continue reading the rest of the stream. getline
    // returns false when end of string
    while (getline(iss, token, delim)) {
        result.push_back(token);
    }
    return result;
}

int safeStringtoInt(const string &str) {
    try {
        return stoi(str);
    } catch (invalid_argument) {
        return -1;
    }
}
