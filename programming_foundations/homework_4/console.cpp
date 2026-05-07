#include "console.h"
#include "destination.h"
#include <algorithm>
#include <iostream>

using namespace std;

vector<Destination> Console::getDestinations() {

    vector<Destination> destinations;
    const int LIMIT = 101;
    const string INVALID_NAME = "invalid";
    const string DEPOT_NAME = "!depot";
    const int INVALID_DISTANCE = -1;

    // While vector<Destination> destinations within the 100 limit (excluding the depot)...
    while (destinations.size() <= LIMIT) {
        string name = INVALID_NAME;
        int x = INVALID_DISTANCE;
        int y = INVALID_DISTANCE;

        // Prompt the user for Destination input
        cout << "Enter \"quit\" to stop entering destinations." << '\n';

        // depot time
        if (destinations.size() < 1) {
            if (!getInt(x, "  Enter Depot X Value [0-1000]>: "))
                break;

            if (!getInt(y, "  Enter Depot Y Value [0-1000]>: "))
                break;

            destinations.emplace_back(DEPOT_NAME, x, y);
            cout << '\n';
            continue;
        }

        // break if "quit" is typed
        if (!getName(name, " Enter Name: [No Spaces, No Commas]>: "))
            break;

        if (!getInt(x, "  Enter X Value [0-1000]>: "))
            break;

        if (!getInt(y, "  Enter Y Value [0-1000]>: "))
            break;

        if (isDuplicate(destinations, name, x, y)) {
            cout << '"' << name << "\" has not been added to the list. "
                 << "\n\n";
            continue;
        }

        destinations.emplace_back(name, x, y);
        cout << '\n';
    }

    return destinations;
}

// Prompts the user for Destination name.
// Disallows the name "!depot".
// Early returns false and sets Destination name to "invalid" if user inputs
// "quit".
bool Console::getName(string &name, const string &question) {

    while (true) {
        cout << question;

        string input;
        getline(cin, input);

        if (input == "quit") {
            name = "invalid";
            return false;
        }

        replace(input.begin(), input.end(), ' ', '_');
        replace(input.begin(), input.end(), ',', '_');

        // empty check
        if (input.empty()) {
            return true;
        }

        // reserved name
        if (input == "!depot") {
            cout << "\"!depot\" is a reserved name! Try again." << '\n';
            continue;
        }

        name = input;
        return true;
    }
}

// Prompts the user for Destination-related int.
// Disallows values not between argument-provided minimum and maximum value.
// Early returns false and sets int -1 if user inputs "quit".
bool Console::getInt(int &value, const string &question, int const &valmin,
                     int const &valmax) {
    while (true) {
        cout << question;

        string input;
        getline(cin, input);
        try {
            // call int initializer on string (stoi = string to int)
            int i;

            if (input == "quit") {
                value = -1;
                return false;
            }

            i = stoi(input);
            // if out of custom bounds, go straight to catch
            if (valmin != -1 && valmax != -1) {
                if (i < valmin || i > valmax) {
                    throw out_of_range("");
                }
            }

            // return true to indicate more values are wanted
            value = i;
            return true;
        } catch (invalid_argument const &ex) {
            cout << "Invalid answer! try again." << '\n';
        } catch (out_of_range const &ex) {
            cout << "Out of range! try again." << '\n';
        }
    }
}
