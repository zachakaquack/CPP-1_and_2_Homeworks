#include <iostream>
#include <vector>

class Console {

  public:
    Console();
    ~Console() {};
    std::vector<std::string> circuitLoop();

  private:
    void clearScreen();
    void drawCircuit();

    bool getInt(const std::string &question, int &value, const int &min = 0,
                const int &max = 0);
    bool getComponent(const std::string &question, std::string &value);

    int findClosingComponent(const std::vector<std::string> &buff,
                             const int &start);

    void setFrontBuffer(const std::vector<std::string> &buff);
    void setBackBuffer(const std::vector<std::string> &buff);

    void appendToFrontBuffer(const std::string &line);
    void appendToBackBuffer(const std::string &line);

    void insertToFrontBuffer(const int &index, const std::string &line);
    void insertToBackBuffer(const int &index, const std::string &line);

    // move the "closest component to the arrow" to the other buffer
    void moveToFrontBuffer();
    void moveToBackBuffer();

    std::vector<std::string> combineBuffers(const bool &addArrow = true);
    void separateCombinedBuffer(const std::vector<std::string> &combined);

    std::vector<std::string> frontBuffer() { return this->m_frontBuffer; }
    std::vector<std::string> backBuffer() { return this->m_backBuffer; }

    std::vector<std::string> m_frontBuffer;
    std::vector<std::string> m_backBuffer;
    short int depth = 0;

    // stuff to center the circuit
    unsigned short int m_terminalHeight;
    void setTerminalHeight();

    // default settings for the circuit in console
    void setDefaultCircuit();

    // print the combined buffer (autoamtically combines front and back)
    void printBuffer();

    // print one line of the console (takes depth into account)
    void println(const std::string &line, const int &i);

    // the help stuff
    void printHelpMessage();

    // not an actual error, but something like that
    void printErrorMessage();
    void setErrorMessage(const std::string &msg);
    std::string m_errorMessage;
};
