// HW4 - Delivery
// Date: 10/26/2025
// Class: PROG1204-1
// Authors: <zachakaquack>, <TFleming-discord>, <JustCat80>
//
// C++ 17+ recommended

#include <iomanip>
#include <iostream>

#include "console.h"
#include "destination.h"
#include "files.h"
#include "prompt.h"

using namespace std;

int main() {

#if (defined(_MSVC_LANG) && _MSVC_LANG < 201703L) ||                           \
    (!defined(_MSVC_LANG) && __cplusplus < 201703L)
    cout << "\n\033[1m\033[31mEnsure you are using C++ 17 or higher. \nThis "
            "program will now continue with some features disabled.\033[0m\n\n";
#endif

    cout << "HW4 - Delivery" << '\n';
    cout << "Solution by <zachakaquack>, <TFleming-discord>, <JustCat80>"
         << '\n'
         << '\n';

    bool wantToQuit = false;
    while (true) {
        vector<Destination> dests;
        // Handle mode selection
        do {

            if (wantToQuit)
                break;

            cout << "Input from [C]onsole, [F]ile, or [Q]uit: ";
            char option = ' ';
            cin >> option;

            option = tolower(option);

            cin.clear();
            cin.ignore(1000, '\n');

            switch (option) {
            case 'c': {
                dests = Console::getDestinations();

                // break: all destinations gotten from console that we want
                break;
            }
            case 'f': {
                do {
                    const bool writing = false;
                    string file = promptFile(writing);

                    if (file.empty()) {
                        cout << '\n' << '\n';
                        break;
                    }
                    dests = File::loadDestinations(file);
                } while (dests.size() == 0);

                // break: loaded all that we want from file
                break;
            }
            case 'q': {
                wantToQuit = true;
                // break: we want to quit
                break;
            }
            default: {
                cout << "Invalid option, try again!" << '\n';
                break;
            }
            }
        } while (dests.size() == 0);

        if (wantToQuit)
            break;

        // all the input is now complete, continue
        cout << '\n';

        // destinations is the list of VALID!!! destinations (makes life easier)
        vector<Destination> destinations;
        for (Destination d : dests) {
            if (d.isValid()) {
                destinations.push_back(d);
            }
        }

        // actually calculate the route
        double distance = fullRoute(destinations);

        cout << "Route: " << '\n';
        // print in order
        // print out if i == the visited number (kinda sucks but it works)
        vector<Destination> visitedOrder;
        for (size_t i = 0; i < destinations.size(); i += 1) {
            for (Destination &d : destinations) {
                if (i == d.visited()) {
                    Location loc = d.location();
                    cout << setw(10) << d.name() << " " << loc << " "
                         << setw(12) << d.distance() << " miles to next\n";
                    visitedOrder.push_back(d);
                }
            }
        }
        cout << setw(10) << "!depot" << "\n";
        cout << " Total Distance Traveled: " << distance << " miles" << '\n';
        cout << " Total number of stops: " << visitedOrder.size() - 1 << "\n\n";
        cout << "Output to file? [Y]es/[N]o: ";

        char option = ' ';
        cin >> option;

        cin.clear();
        cin.ignore(1000, '\n');

        option = tolower(option);

        if (option != 'y') {
            cout << "Not writing..." << '\n' << '\n';
            continue;
        }

        const bool writing = true;
        string file = promptFile(writing);

        if (file.empty()) {
            cout << "Not writing..." << '\n' << '\n';
            continue;
        }

        File::write(visitedOrder, file);
        cout << "File saved to " << file << '\n' << '\n';

    } // while(true) (the main loop)
    // this loop breaks when we are done doing stuff

    std::cout << '\n' << "HW4 Complete" << '\n';
}
