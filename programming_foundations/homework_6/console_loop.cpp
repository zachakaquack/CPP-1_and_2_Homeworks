#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) ||                          \
    (!defined(_MSVC_LANG) && __cplusplus >= 201703L)

#include "chain.h"
#include "console.h"
#include "events.h"
#include "file_manager.h"
#include "parser.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void ConsoleLoop::mainLoop() {
    //
    Console console;
    vector<string> lines = console.begin();

    console.clearScreen();
    Chain chain{lines};
    chain.print();

    string contents = "";
    for (const string &line : lines) {
        contents += line;
    }

    FileManager manager;
    manager.askSaveFile(contents);
}
// void ConsoleLoop::mainLoop() {
//     /*
//     start_x start_y
//     num_links
//     length rotation
//     */
//     string x, y, length, rotation, num_links;
//
//     cout << "Input starting X and Y..." << '\n';
//     cout << "X : ";
//
//     while (true) {
//         try {
//             getline(cin, x);
//             stod(x);
//             break;
//         } catch (...) {
//             cout << "Invalid input, try again : ";
//         }
//     }
//
//     cout << "Y : ";
//     while (true) {
//         try {
//             getline(cin, y);
//             stod(y);
//             break;
//         } catch (...) {
//             cout << "Invalid input, try again : ";
//         }
//     }
//
//     cout << "Number of links in the chain : ";
//     while (true) {
//         try {
//             getline(cin, num_links);
//             stoi(num_links);
//             break;
//         } catch (...) {
//             cout << "Invalid input, try again : ";
//         }
//     }
//
//     ostringstream oss;
//
//     // build the information
//     oss << fixed << setprecision(1) << x << " " << y << "\n";
//     oss << fixed << setprecision(1) << num_links << '\n';
//
//     // build the links
//     for (int i = 0; i < stoi(num_links); i++) {
//         cout << "Link " << i + 1 << " length : ";
//         while (true) {
//             try {
//                 getline(cin, length);
//                 stod(length);
//                 break;
//             } catch (...) {
//                 cout << "Invalid input, try again : ";
//             }
//         }
//
//         cout << "Link " << i + 1 << " rotation : ";
//         while (true) {
//             try {
//                 getline(cin, rotation);
//                 stod(rotation);
//                 rotation.append("\n");
//                 break;
//             } catch (...) {
//                 cout << "Invalid input, try again : ";
//             }
//         }
//
//         oss << fixed << setprecision(1) << length << " " << rotation;
//     }
//
//     // if we get here, input is good! (hopefully)
//
//     string contents = oss.str();
//     cout << contents;
//
//     vector<string> lines = splitString(contents, '\n');
//
//     Chain chain{lines};
//     chain.print();
//
//     FileManager manager;
//     manager.askSaveFile(contents);
// }
#endif
