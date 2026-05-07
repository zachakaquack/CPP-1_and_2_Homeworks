#pragma once

#include "base_event.h"
#include <memory>
#include <string>
#include <vector>

using namespace std;
class ProgramManager {
  public:
    ProgramManager() {};

    void startMainLoop();
    void addEvent(const shared_ptr<BaseEvent> &event);
    void callEvent(const string &trigger);

    string title() { return this->m_title; }
    string setTitle(const string &title);

    string error() { return this->m_error; }
    string setErrorMessage(const string &error);

  private:
    vector<shared_ptr<BaseEvent>> m_events;
    string m_title = "no title set";
    string m_error = "";

    void printTitle() { cout << this->title(); }
    void printError();
};
