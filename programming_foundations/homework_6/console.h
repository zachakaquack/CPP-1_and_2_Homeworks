#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Console {
  public:
    Console();

    // generic
    vector<string> begin();
    void printBuffers();
    void clearScreen();

    // buffer functions
    // getters
    vector<string> buffer() { return this->m_buffer; }
    vector<string> infoBuffer() { return this->m_infoBuffer; }

    // setters
    vector<string> setBuffer(const vector<string> &buff);
    vector<string> setInfoBuffer(const vector<string> &buff);

    // appending
    vector<string> appendBuffer(const string &x);
    vector<string> appendInfoBuffer(const string &x);

    // inserting
    vector<string> insertBuffer(const int &index, const string &x);
    vector<string> insertInfoBuffer(const int &index, const string &x);

    void appendChainLinkPair(const string &x);

    string errorMessage() { return this->m_errorMessage; }
    string setErrorMessage(const string &x);

    enum Action {
        UP,     // navigate up
        DOWN,   // navigate down
        EDIT,   // edit line
        DONE,   // finish up
        NEW,    // new chain link
        REMOVE, // delete current chain link
        NONE,   // nothing happened
        TOP,    // navigate to top of console
        BOTTOM, // navigate to bottom of console
        HELP    // print help menu
    };

    string editLine();
    void printHelp();
    void center(const int &contentSize);

  private:
    Console::Action getAction();
    void deleteCurrentLine();

    // buffer bs
    vector<string> m_buffer;
    // info buffer prepends each line of the buffer; indicates information like
    // line numbers, and what type of line it is (starting position, chain
    // count, chain vector)
    vector<string> m_infoBuffer;

    // what line the cursor is on
    size_t m_lineIndex = 0;

    // random
    unsigned short int m_terminalHeight;
    string m_errorMessage;
    void printErrorMessage() { std::cout << this->errorMessage(); }
    string formatLine(string &line);
    void setLinkCountInBuffer(const int &x);
    int getLinkCount();
    int getTerminalHeight();

    // consts
    const size_t CHAIN_LINK_COUNT_INDEX = 1;
    const size_t CHAIN_LINKS_STARTING_INDEX = 2;
};
