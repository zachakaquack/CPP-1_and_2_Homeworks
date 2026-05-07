#pragma once

#include "linked_list.h"
#include "parser.h"
#include "polar.h"
#include <memory>
#include <vector>

using namespace std;
class Chain : public LinkedList {
  public:
    Chain(vector<string> &unformattedLines)
        : m_lines(formatLines(unformattedLines)) {
        this->calculateEndChain();
    };
    void calculateEndChain();
    void print();

    vector<string> lines() { return this->m_lines; }
    int chainCount() { return this->m_chainCount; }
    shared_ptr<Polar::Position> starting() { return this->m_starting; }
    Polar::Position end() { return this->m_end; }

  private:
    vector<string> m_lines{};

    int m_chainCount = 0;
    shared_ptr<Polar::Position> m_starting;

    Polar::Position m_end{0, 0, 0, 0};
};
