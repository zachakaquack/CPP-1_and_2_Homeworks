#include "program_manager.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>

using namespace std;
void ProgramManager::startMainLoop() {
    while (true) {
        this->printError();
        this->printTitle();

        string input = "";
        getline(cin, input);

        if (tolower(input[0]) == 'q') {
            return;
        }

        if (input.empty()) {
            continue;
        }

        this->callEvent(input);
    }
}

string ProgramManager::setTitle(const string &title) {
    this->m_title = title;
    return title;
}

string ProgramManager::setErrorMessage(const string &error) {
    this->m_error = error;
    return error;
}

void ProgramManager::addEvent(const shared_ptr<BaseEvent> &event) {
    this->m_events.push_back(event);
}

void ProgramManager::callEvent(const string &input) {
    for (shared_ptr<BaseEvent> ptr : this->m_events) {

        string lower = input;
        transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        for (string trigger : ptr->triggers()) {
            if (trigger == input) {
                this->setErrorMessage("");
                ptr->mainLoop();
                return;
            }
        }
    }

    this->setErrorMessage("Option \"" + input + "\" not found!");
}

void ProgramManager::printError() {
    if (!this->m_error.empty()) {
        cout << '\n' << this->error() << '\n';
    }
}
