#pragma once

#include <iostream>
#include <vector>

using namespace std;
class BaseEvent {
  public:
    BaseEvent(const vector<string> triggers) : m_triggers(triggers) {};

    virtual void mainLoop() { cout << "base loop" << '\n'; }
    const vector<string> triggers() { return this->m_triggers; }

  private:
    vector<string> m_triggers;
};
