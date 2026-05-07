// HW5 - Circuits
// Date: -/--/2025
// Class: PROG1204-1
// Authors: <zachakaquack>, <TFleming-discord>, <JustCat80>
//
// C++ 17+ recommended

#include <iomanip>
#include <iostream>

#include "circuit.h"
#include "console.h"
#include "file.h"
#include "parse.h"
#include "prompt.h"

#include "global.h"

using namespace std;

int main() {

#if (defined(_MSVC_LANG) && _MSVC_LANG < 201703L) ||                           \
    (!defined(_MSVC_LANG) && __cplusplus < 201703L)
    cout << Global::Color::red << Global::Color::bold
         << "\nEnsure you are using C++ 17 or higher. \nThis "
            "program will now continue with some features disabled.\n\n"
         << Global::Color::reset;
#endif

    cout << "HW5 - Circuits" << '\n';
    cout << "Solution by: <zachakaquack>, <TFleming-discord>, <JustCat80>"
         << '\n'
         << '\n';

    bool wantToQuit = false;
    do {

        cout << "\n[C]onsole, [F]ile, or [Q]uit? : ";
        char option = ' ';
        cin >> option;

        option = tolower(option);

        cin.clear();
        cin.ignore(1000, '\n');

        Circuit circuit;

        switch (option) {
        case 'c': {
            Console console;
            vector<string> contents = console.circuitLoop();
            circuit = Parse::parse(contents);
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

                cout << "Reading from " << file << '\n';

                string contents = File::read(file);
                circuit = Parse::parse(contents);
            } while (circuit.components().size() == 0);
            break;
        }
        case 'q': {
            wantToQuit = true;
            break;
        }
        default: {
            cout << Global::Color::red << "Invalid option, try again!"
                 << Global::Color::reset << '\n';
            break;
        }
        }

        if (wantToQuit)
            break;

        if (circuit.components().size() == 0) {
            cout << Global::Color::red << "No input found!"
                 << Global::Color::reset << '\n';
            continue;
        }

        cout << Global::Color::bold << "\n\nFinal Result: \n\n"
             << Global::Color::reset;

        const string circuitToString = circuit.toString();
        cout << circuitToString;

        // Save cout state to reset to after changing precision
        ios oldState(nullptr);
        oldState.copyfmt(cout);

        const double ohms = circuit.resistance();
        cout << "\nTotal Resistance: " << fixed << setprecision(14) << ohms
             << "\n\n";

        cout.copyfmt(oldState);

        // outputting
        char picked = ' ';
        do {
            cout << "Output to file? [Y]es/[N]o: ";
            cin >> option;

            picked = tolower(option);

            cin.clear();
            cin.ignore(1000, '\n');

            if (picked == 'y' || picked == 'n')
                break;

            cout << Global::Color::red << "Unknown option: '" << option
                 << "'. Try again! " << Global::Color::reset << '\n';
        } while (picked != 'y' && picked != 'n');

        if (picked == 'n')
            continue;

        const bool writing = true;
        string filePath = promptFile(writing);

        if (filePath.empty()) {
            cout << "Not writing..." << '\n' << '\n';
            continue;
        }

        File::write(filePath, circuitToString);
        cout << "Written to: " << filePath << "\n\n";

    } while (true);

    cout << "\n" << "HW5 Complete." << '\n';

    return 0;
}
