#include "console.h"
#include "components.h"
#include "global.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#elif __linux__
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#else
#error "Unknown compiler"
#endif

Console::Console() {}

std::vector<std::string> Console::circuitLoop() {
    this->setTerminalHeight();
    this->setDefaultCircuit();
    this->drawCircuit();
    return this->combineBuffers(false);
}

void Console::setDefaultCircuit() {

    // get the name
    cout << "Enter Name of Circuit >: ";
    string input = "";
    getline(cin, input);

    this->appendToFrontBuffer("Circuit " + input);
    // this->appendToFrontBuffer("Parallel");
    // this->appendToFrontBuffer("Resistor 100");
    // this->appendToFrontBuffer("Serial");
    // this->appendToFrontBuffer("Resistor 300");
    //
    // this->appendToBackBuffer("EndSerial");
    // this->appendToBackBuffer("EndParallel");
    this->appendToBackBuffer("EndCircuit");
}

void Console::clearScreen() {
    // Source -
    // https://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor
    // Posted by Evgeny Gavrin
    // Retrieved 2025-11-05, License - CC BY-SA 4.0

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    system("cls");
#elif __APPLE__
    system("clear");
#elif __linux__
    std::cout << "\033[2J\033[1;1H";
#else
#error "Unknown compiler"
#endif
}

void Console::setTerminalHeight() {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    this->m_terminalHeight = rows;
#elif __APPLE__
    // idk how apple works
    this->m_terminalHeight = 10;
#elif __linux__
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    this->m_terminalHeight = w.ws_row;
#else
#error "Unknown compiler"
#endif
}

void Console::drawCircuit() {
    while (true) {
        this->clearScreen();
        unsigned short int bufferSize = (this->frontBuffer().size());
        unsigned short int center =
            (this->m_terminalHeight / 2) - (bufferSize)-1;
        for (int i = 0; i < center; i += 1) {
            std::cout << '\n';
        }

        // printing
        this->printBuffer();

        // handle input
        std::cout << '\n';
        std::string input;
        this->printErrorMessage();
        this->getComponent("Enter Component ([H]elp): ", input);

        if (input.empty()) {
            continue;
        }

        if (input == "break") {
            return;
        }

        // no errors have occured, remove the message
        this->setErrorMessage("");

        if (input == "h") {
            this->printHelpMessage();
            continue;
        }

        // if (input == "d") {
        //     int lineNum;
        //     std::vector<std::string> buff = this->combineBuffers(false);
        //
        //     this->getInt("Enter Line #: ", lineNum, 1, buff.size() - 1);
        //
        //     if (size_t(lineNum) == 1) {
        //         this->setErrorMessage("Cannot delete Circuit!");
        //         continue;
        //     }
        //
        //     if (size_t(lineNum) == buff.size() - 1) {
        //         this->setErrorMessage("Cannot delete EndCircuit!");
        //         continue;
        //     }
        //     lineNum--;
        //
        //     // if a leaf node, it's easy
        //     if (Components::isLeafNode(buff[lineNum])) {
        //         if (lineNum > this->backBuffer().size()) {
        //             std::cout << "FRONT" << '\n';
        //             std::vector<std::string> front = buff;
        //             front.erase(front.begin() + lineNum);
        //             front.erase(front.begin() + lineNum, front.end());
        //             this->setFrontBuffer(front);
        //         } else {
        //             std::cout << "BACK" << '\n';
        //         }
        //
        //         // buff = this->combineBuffers();
        //         // buff.erase(buff.begin() + lineNum);
        //         // this->separateCombinedBuffer(buff);
        //         continue;
        //     }
        //
        //     // okay, now we know it's not a leaf node, and not something we
        //     // can't delete
        //
        //     // worry about whether its an end component or not
        //     int endNum;
        //     if (Components::isClosingComponent(buff[lineNum])) {
        //         this->setErrorMessage("No deleting from an ending
        //         component!"); continue;
        //         // // literally just do the stack "calculations" in reverse,
        //         but
        //         // // swap the two
        //         //
        //         // // get the buff from the start onwards
        //         // std::vector<std::string> newBuff = buff;
        //         // // only get the buffer from the last line we need to the
        //         end
        //         // newBuff.erase(newBuff.begin() + lineNum + 1,
        //         newBuff.end());
        //         // std::stack<std::string> lineStack;
        //         //
        //         // // reverse order
        //         // for (size_t i = newBuff.size() - 1; i >= 0; i -= 1) {
        //         //     const string &line = newBuff[i];
        //         //
        //         //     // this means this is the "split" in between the two
        //         //     // buffers; so we have to be careful now and add an
        //         extra
        //         //     if (line.empty()) {
        //         //         continue;
        //         //     }
        //         //
        //         //     if (line == newBuff[newBuff.size() - 1]) {
        //         //         lineStack.push(line);
        //         //         continue;
        //         //     }
        //         //
        //         //     // line.substr turns "EndPar.." to "Par.."
        //         //     if (line ==
        //         //         lineStack.top().substr(3, lineStack.top().size()))
        //         {
        //         //         if (lineStack.size() == 1) {
        //         //             endNum = i + 1;
        //         //             break;
        //         //         }
        //         //         lineStack.pop();
        //         //     }
        //         // }
        //         //
        //         // std::swap(lineNum, endNum);
        //         // lineNum -= 1;
        //         //
        //         // // we have to adjust some of the numbers just cause i suck
        //         at
        //         // // programming:
        //         // // when the component is in the back buffer, the
        //         EndCircuit
        //         // line
        //         // // gets deleted; this is to remedy that
        //         // if (size_t(lineNum) > this->frontBuffer().size() &&
        //         //     size_t(endNum) > this->frontBuffer().size()) {
        //         //     endNum -= 1;
        //         // }
        //     } else {
        //         // okay its an opening component, this is easy
        //         endNum = this->findClosingComponent(buff, lineNum);
        //     }
        //
        //     // now, do we want to delete the node with or without
        //     // its children?
        //
        //     cout << "Do you want to delete INSIDE as well? [Y]es, [N]o: ";
        //     string input = "";
        //     getline(cin, input);
        //     transform(input.begin(), input.end(), input.begin(), ::tolower);
        //
        //     if (input[0] != 'y' && input[0] != 'n') {
        //         this->setErrorMessage("Invalid Option!: " + input);
        //         continue;
        //     }
        //
        //     if (input[0] == 'n') {
        //         std::vector<std::string> front = buff;
        //         front.erase(front.begin() + lineNum);
        //         front.erase(front.begin() + lineNum + 1, front.end());
        //
        //         std::vector<std::string> back = buff;
        //         back.erase(back.begin(), back.begin() + endNum + 1);
        //         this->setFrontBuffer(front);
        //         this->setBackBuffer(back);
        //
        //         continue;
        //     }
        //
        //     // yes, delete insides
        //     // only keep up to line number, and from end num to end of the
        //     // buffer
        //     std::vector<std::string> front = buff;
        //     front.erase(front.begin() + lineNum, front.end());
        //
        //     std::vector<std::string> back = buff;
        //     back.erase(back.begin(), back.begin() + endNum + 1);
        //     this->setFrontBuffer(front);
        //     this->setBackBuffer(back);
        //
        //     continue;
        // }

        if (input == "j") {
            this->moveToFrontBuffer();
            continue;
        }

        if (input == "k") {
            this->moveToBackBuffer();
            continue;
        }

        // no movement, no help, and no break; so this is a component
        this->appendToFrontBuffer(input);

        if (!Components::isLeafNode(input)) {
            this->insertToBackBuffer(0, "End" + input);
        }
    }
}

int Console::findClosingComponent(const std::vector<std::string> &buff,
                                  const int &start) {

    // get the buff from the start onwards
    std::vector<std::string> newBuff = buff;
    // only get the buffer from the starting line number to the end
    // newBuff.erase(newBuff.begin(), newBuff.begin() + start);
    std::stack<std::string> lineStack;

    for (size_t i = 0; i < newBuff.size(); i += 1) {
        const string &line = newBuff[i];

        if (line.substr(0, 7) == "Circuit" || line.substr(0, 7) == "ckt") {
            continue;
        }

        if (Components::isOpeningComponent(line)) {
            lineStack.push(line);
            continue;
        }

        if (line == "End" + lineStack.top()) {
            if (lineStack.size() == 1) {
                // return "+ start" to offset from the beginning
                return i + start;
            }
            lineStack.pop();
        }
    }

    return 0;
}

void Console::separateCombinedBuffer(const std::vector<std::string> &combined) {
    std::vector<std::string> front;
    std::vector<std::string> back;
    bool isFront = true;

    for (const std::string &line : combined) {
        std::cout << "line: " << line << '\n';
        if (line.empty()) {
            isFront = false;
            std::cout << "found empty! is no longer front" << '\n';
            continue;
        }

        if (isFront) {
            front.push_back(line);
        } else {
            back.push_back(line);
        }
    }

    this->setFrontBuffer(front);
    this->setBackBuffer(back);
}

void Console::appendToFrontBuffer(const std::string &line) {
    std::vector<std::string> buffer = this->frontBuffer();
    buffer.push_back(line);
    this->setFrontBuffer(buffer);
}

void Console::setFrontBuffer(const std::vector<std::string> &buff) {
    this->m_frontBuffer = buff;
}

void Console::insertToFrontBuffer(const int &index, const std::string &line) {
    std::vector<std::string> buffer = this->frontBuffer();
    buffer.insert(buffer.begin() + index, line);
    this->setFrontBuffer(buffer);
}

void Console::appendToBackBuffer(const std::string &line) {
    std::vector<std::string> buffer = this->backBuffer();
    buffer.push_back(line);
    this->setBackBuffer(buffer);
}

void Console::setBackBuffer(const std::vector<std::string> &buff) {
    this->m_backBuffer = buff;
}

void Console::insertToBackBuffer(const int &index, const std::string &line) {
    std::vector<std::string> buffer = this->backBuffer();
    buffer.insert(buffer.begin() + index, line);
    this->setBackBuffer(buffer);
}

void Console::printHelpMessage() {

    const std::vector<std::string> lines = {
        "Valid Inputs:",
        Global::Color::red + "COMPONENTS:" + Global::Color::reset,
        "\t- [P]arallel",
        "\t- [S]erial",
        "\t- [R]esistor / <number>\n",
        Global::Color::red + "EXTRA:" + Global::Color::reset,
        "\t- J (Nagivate Down)",
        "\t- K (Nagivate Up)",
        // "\t- [D]elete",
        "\t- [Break] (Finish Circuit)",
        "\t- [H]elp",
    };

    this->clearScreen();
    unsigned short int contentSize = lines.size();
    unsigned short int center =
        (this->m_terminalHeight / 2) - (contentSize / 2) - 1;
    for (int i = 0; i < center; i += 1) {
        std::cout << '\n';
    }

    for (const std::string &line : lines) {
        std::cout << line << '\n';
    }

    std::cout << '\n' << "Press <Enter> to continue." << '\n';
    std::cin.get();
    return;
}

void Console::setErrorMessage(const std::string &msg) {
    this->m_errorMessage = Global::Color::red + msg + Global::Color::reset;
}

void Console::printErrorMessage() {
    if (m_errorMessage.empty()) {
        return;
    }
    std::cout << this->m_errorMessage << '\n';
}

void Console::println(const std::string &line, const int &i) {
    const std::string PADDING_PER = "  ";
    std::string padding = "";
    for (int i = 0; i < depth; i += 1) {
        padding += PADDING_PER;
    }
    if (i == -1) {
        std::cout << ' ' << "  " << padding << line << '\n';
        return;
    }
    std::cout << i << "  " << padding << line << '\n';
}

void Console::printBuffer() {

    std::vector<std::string> buffer = this->combineBuffers();
    std::stack<std::string> nodes;
    int lineNumber = 0;

    for (size_t i = 0; i < buffer.size(); i += 1) {
        const string &line = buffer[i];

        // this means this is the "split" in between the two buffers; so print
        // our beloved arrow
        if (line.empty()) {
            std::string arrowHead = "";
            for (int i = 0; i < this->depth; i += 1) {
                arrowHead += "--";
            }
            std::cout << Global::Color::red + arrowHead + ">" +
                             Global::Color::reset
                      << '\n';
            continue;
        }

        if (Components::isOpeningComponent(line)) {
            lineNumber++;
            this->println(line, lineNumber);
            this->depth++;

            if (i >= buffer.size() - 1) {
                continue;
            }
            if (!Components::isClosingComponent(buffer[i + 1])) {
                continue;
            }

            this->println("...", -1);

            continue;
        }

        if (Components::isClosingComponent(line)) {
            this->depth--;
        }

        // if its a leaf node, it also goes here
        // (also Circuit <name>)
        lineNumber++;
        this->println(line, lineNumber);
    }
}

void Console::moveToFrontBuffer() {

    // move to front is moving "downwards"
    // be careful of going below the EndCircuit

    std::vector<std::string> front = this->frontBuffer();
    std::vector<std::string> back = this->backBuffer();

    if (back.size() == 1) {
        this->setErrorMessage("Cannot go any lower!");
        return;
    }

    this->appendToFrontBuffer(back[0]);
    back.erase(back.begin());

    this->setBackBuffer(back);
}

void Console::moveToBackBuffer() {

    // move to back is moving "upwards"
    // be careful of going above the Circuit <NAME>

    std::vector<std::string> front = this->frontBuffer();
    std::vector<std::string> back = this->backBuffer();

    if (front.size() == 1) {
        this->setErrorMessage("Cannot go any higher!");
        return;
    }

    this->insertToBackBuffer(0, front[front.size() - 1]);

    front.pop_back();

    this->setFrontBuffer(front);
}

std::vector<std::string> Console::combineBuffers(const bool &addArrow) {
    std::vector<std::string> front = this->frontBuffer();
    std::vector<std::string> back = this->backBuffer();
    std::vector<std::string> buff;

    for (const std::string &line : front) {
        buff.push_back(line);
    }
    if (addArrow) {
        buff.push_back("");
    }
    for (const std::string &line : back) {
        buff.push_back(line);
    }
    return buff;
}

bool Console::getInt(const std::string &question, int &value, const int &min,
                     const int &max) {
    while (true) {
        std::cout << question;
        std::string input;
        std::getline(std::cin, input);
        try {
            // call int initializer on string (stoi = string to int)
            int i;

            if (input == "quit") {
                value = -1;
                return false;
            }

            i = stoi(input);
            // if out of custom bounds, go straight to catch
            if (min != 0 && max != 0) {
                if (i < min || i > max) {
                    throw std::out_of_range("");
                }
            }

            // return true to indicate more values are wanted
            value = i;
            return true;
        } catch (std::invalid_argument const &ex) {
            std::cout << "Invalid answer! try again." << '\n';
        } catch (std::out_of_range const &ex) {
            std::cout << "Out of range! try again." << '\n';
        }
    }
}

bool Console::getComponent(const std::string &question, std::string &value) {

    cout << question;
    string input = "";
    getline(cin, input);

    // extra case of just a number instead of typing "resistor"
    try {
        stoi(input);
        value = "Resistor " + input;
    } catch (const invalid_argument &e) {
        //
    };

    transform(input.begin(), input.end(), input.begin(), ::tolower);

    for (string extra : Global::CONSOLE_EXTRA_ALLOWED_INPUTS) {
        if (input == extra) {
            value = input;
            return true;
        }
    }

    for (string component : Global::OPENING_COMPONENTS) {
        if (input[0] == component[0]) {
            value = char(toupper(input[0])) + component.substr(1);
            return true;
        }
    }

    if (input == "resistor" || input[0] == 'r') {
        int ohms = -1;
        getInt("Enter Resistor Ohms: ", ohms, 0, 0);
        value = "Resistor " + to_string(ohms);
        return true;
    }
    this->setErrorMessage("Invalid Component: " + input);
    return false;
}
