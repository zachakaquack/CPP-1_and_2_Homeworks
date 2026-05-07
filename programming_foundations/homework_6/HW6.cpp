// HW6 - Chain
// Date: -/--/2025
// Class: PROG1204-1
// Authors: Zach Allen, Thomas Fleming, Elijah Johnson
//
// C++ 17+ REQUIRED

#include <iostream>

#if (defined(_MSVC_LANG) && _MSVC_LANG < 201703L) ||                           \
    (!defined(_MSVC_LANG) && __cplusplus < 201703L)
int main() {
    cout << Global::Colors::Error << Global::Colors::Bold
         << "\nEnsure you are using C++ 17 or higher. \n"
            "PROGRAM NOT CONTINUING.\n\n"
         << Global::Colors::Reset;
    return 0;
}
#endif
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) ||                          \
    (!defined(_MSVC_LANG) && __cplusplus >= 201703L)

#include "events.h"
#include "program_manager.h"
#include <string>
#include <vector>

int main() {

    cout << "HW6 - Chain" << '\n';
    cout << "Solution by Zach Allen, Thomas Fleming, Elijah Johnson" << '\n'
         << '\n';

    ProgramManager manager;
    manager.setTitle("Enter option: [C]onsole, [F]ile, [Q]uit\n>");

    const vector<string> consoleTriggers = {"console", "c"};
    const vector<string> fileTriggers = {"file", "f"};

    const shared_ptr<FileLoop> file = make_shared<FileLoop>(fileTriggers);
    const shared_ptr<ConsoleLoop> console =
        make_shared<ConsoleLoop>(consoleTriggers);

    manager.addEvent(file);
    manager.addEvent(console);
    manager.startMainLoop();

    cout << "\n" << "HW6 Complete." << '\n';

    return 0;
}
#endif
