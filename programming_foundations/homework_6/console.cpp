#include "console.h"
#include "global.h"
#include "parser.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define WIN32_LEAN_AND_MEAN 0
#include <windows.h>
#elif __linux__
// #include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#else
#error "Unknown compiler"
#endif

Console::Console() {}

vector<string> Console::begin() {
    this->clearScreen();

    this->m_terminalHeight = this->getTerminalHeight();

    // load default information
    this->appendBuffer("0.0 0.0");
    this->appendBuffer("3");
    this->appendBuffer("4 45.0");
    this->appendBuffer("3 90.0");
    this->appendBuffer("2 -45.0");

    this->appendInfoBuffer("Start: ");
    this->appendInfoBuffer("Count: ");
    this->appendInfoBuffer("1: ");
    this->appendInfoBuffer("2: ");
    this->appendInfoBuffer("3: ");

    // main loop
    while (true) {
        this->clearScreen();
        this->center(this->buffer().size());
        this->printBuffers();
        this->printErrorMessage();
        this->setErrorMessage("");
        Action action = this->getAction();

        if (action == Action::DOWN) {
            if (this->m_lineIndex < this->buffer().size() - 1) {
                this->m_lineIndex += 1;
            }
        }

        if (action == Action::UP) {
            if (this->m_lineIndex > 0) {
                this->m_lineIndex -= 1;
            }
        }

        if (action == Action::TOP) {
            this->m_lineIndex = 0;
        }

        if (action == Action::BOTTOM) {
            this->m_lineIndex = this->buffer().size() - 1;
        }

        if (action == Action::HELP) {
            this->printHelp();
        }

        if (action == Action::EDIT) {
            string newLine = this->editLine();

            // editLine returns an empty string if the format is invalid
            // (aka dont accept)
            if (!newLine.empty()) {
                this->m_buffer[this->m_lineIndex] = newLine;
            }
        }

        if (action == Action::DONE) {
            const int currentCount = this->getLinkCount();
            if (currentCount < 1) {
                this->setErrorMessage(
                    "Cannot finish with less than one chain link!");
                continue;
            }

            return this->buffer();
        }

        if (action == Action::NEW) {
            const int currentCount = this->getLinkCount();
            this->setLinkCountInBuffer(currentCount + 1);
            this->appendChainLinkPair("0.0 0.0");
        }
        if (action == Action::REMOVE) {
            const int currentCount = this->getLinkCount();
            this->setLinkCountInBuffer(currentCount - 1);
            this->deleteCurrentLine();
        }
    }
}

int Console::getTerminalHeight() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return rows;
#elif __APPLE__
    // idk how apple works
    return 10;
#elif __linux__
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
#else
#error "Unknown compiler"
#endif
}

void Console::setLinkCountInBuffer(const int &x) {
    vector<string> buff = this->buffer();
    buff[this->CHAIN_LINK_COUNT_INDEX] = to_string(x);
    this->setBuffer(buff);
}

int Console::getLinkCount() {
    vector<string> buff = this->buffer();
    return stoi(buff[this->CHAIN_LINK_COUNT_INDEX]);
}

void Console::deleteCurrentLine() {
    if (this->m_lineIndex < this->CHAIN_LINKS_STARTING_INDEX) {
        this->setErrorMessage("Cannot delete line " +
                              to_string(this->m_lineIndex + 1) + "!");
        return;
    }

    vector<string> buff = this->buffer();
    vector<string> info = this->infoBuffer();

    if (this->m_lineIndex > buff.size() - 1) {
        this->setErrorMessage(
            "Somehow, you are out of range! Cannot delete this line!");
        // failsafe, just return to top
        this->m_lineIndex = 0;
        return;
    }

    buff.erase(buff.begin() + this->m_lineIndex);
    info.erase(info.begin() + this->m_lineIndex);

    // redo the line numbers in the info buffer
    const size_t LINE_COUNT = info.size() - this->CHAIN_LINKS_STARTING_INDEX;
    for (size_t i = 0; i < LINE_COUNT; i += 1) {
        const int lineNumber = i + 1;
        const string stringLineNumber = to_string(lineNumber) + ": ";

        // add "this->CHAIN_LINK_COUNT_INDEX" for the correct offset
        info[i + this->CHAIN_LINKS_STARTING_INDEX] = stringLineNumber;
    }

    // change count
    const int currentCount = this->getLinkCount();
    this->setLinkCountInBuffer(currentCount - 1);

    // move cursor to an appropriate position if needed
    if (this->m_lineIndex >= buff.size()) {
        this->m_lineIndex = buff.size() - 1;
    } else {
        this->m_lineIndex -= 1;
    }

    this->setInfoBuffer(info);
    this->setBuffer(buff);
}

string Console::editLine() {
    while (true) {
        this->clearScreen();

        // the whole "editing line" garbanzo takes up 5 lines
        const int LINE_COUNT = 5;
        this->center(LINE_COUNT);

        const size_t index = this->m_lineIndex;
        std::cout << "Editing line:" << '\n';
        std::cout << Global::Colors::Directory << this->buffer()[index]
                  << Global::Colors::Reset << '\n'
                  << '\n';

        const bool EDITING_CHAIN_LINK_COUNT =
            (index == this->CHAIN_LINK_COUNT_INDEX);

        string format = "Format: " + Global::Colors::Directory + "X.X Y.Y" +
                        Global::Colors::Reset + " | (length (cm)) (rotation)";
        if (EDITING_CHAIN_LINK_COUNT) {
            format = "Format: " + Global::Colors::Directory + "X" +
                     Global::Colors::Reset + " | (length (cm)) (rotation)";
        } else if (this->m_lineIndex == 0) {
            // editing starting position; change format
            format = "Format: " + Global::Colors::Directory + "X.X Y.Y" +
                     Global::Colors::Reset + " | (X Coordinate) (Y Coordinate)";
        }
        std::cout << format << '\n' << "> ";

        std::cout << Global::Colors::Directory;
        std::string input;
        getline(cin, input);
        std::cout << Global::Colors::Reset;

        input = this->formatLine(input);

        const vector<string> splitInput = splitString(input, ' ');

        // editing the starting position: this is a special case, because the
        // input is (x, y) rather than (length, angle)
        if (this->m_lineIndex == 0) {
            const double x = stod(splitInput[0]);
            const double y = stod(splitInput[0]);

            if (x < 0 || x > 1000) {
                this->setErrorMessage(
                    "Invalid Input! Range must be between 0.0-1000.0.");
                return "";
            }

            if (y < 0 || y > 1000) {
                this->setErrorMessage(
                    "Invalid Input! Range must be between 0.0-1000.0.");
                return "";
            }

            return input;
        }

        if (!EDITING_CHAIN_LINK_COUNT) {
            // make sure that there's two entries
            if (splitInput.size() != 2) {
                this->setErrorMessage("Invalid Format!");
                return "";
            }

            // make sure both are in the valid range
            const double length = stod(splitInput[0]);
            const double angle = stod(splitInput[1]);

            // outside of 1 - 100cm range
            if (length < 1 || length > 100) {
                this->setErrorMessage(
                    "Invalid Input! Range must be between 1-100cm.");
                return "";
            }

            // outside of -90 - 90 degree range
            if (angle < -90 || length > 90) {
                this->setErrorMessage(
                    "Invalid Input! Range must be between -90 & 90 degrees.");
                return "";
            }

            // input is valid
            return input;
        }

        if (splitInput.size() != 1) {
            this->setErrorMessage("Invalid Format! 2 Inputs Required.");
            return "";
        }

        // we are editing chain link count now
        int newChainLinkCount;
        // make sure it's valid
        try {
            newChainLinkCount = stoi(input);
        } catch (const std::invalid_argument &e) {
            this->setErrorMessage("Invalid Input: " + input);
            return "";
        } catch (const std::out_of_range &e) {
            this->setErrorMessage("Out of range: " + input);
            return "";
        }

        // outside of 1 - 100 range
        if (newChainLinkCount < 1 || newChainLinkCount > 100) {
            this->setErrorMessage(
                "Invalid Input! Range must be between 1 and 100 chains.");
            return "";
        }

        const int currentChainLinkCount = this->getLinkCount();
        if (newChainLinkCount < currentChainLinkCount) {
            // gotta remove some (just remove from the front)
            const int difference = currentChainLinkCount - newChainLinkCount;
            vector<string> buff = this->buffer();
            vector<string> info = this->infoBuffer();
            for (int i = 0; i < difference; i += 1) {
                buff.pop_back();
                info.pop_back();
            }
            this->setBuffer(buff);
            this->setInfoBuffer(info);

        } else if (newChainLinkCount > currentChainLinkCount) {
            // add some more
            const int difference = newChainLinkCount - currentChainLinkCount;
            for (int i = 0; i < difference; i += 1) {
                this->appendChainLinkPair("0.0 0.0");
            }
        }
        // if it's equal, just do nothing

        return input;
    }
}

void Console::center(const int &contentSize) {
    unsigned short int center =
        (this->m_terminalHeight / 2) - (contentSize / 2) - 1;
    for (int i = 0; i < center; i += 1) {
        std::cout << '\n';
    }
}

void Console::printHelp() {
    // the comments prevent the clang formatter from making it weird, dont
    // touchy please

    // clang-format off
    // yes, manual formatting in the string. get rolled losers
    const std::vector<std::string> lines = {
        "    Up: " + Global::Colors::Directory + "[k], [w]" + Global::Colors::Reset,
        "  Down: " + Global::Colors::Directory + "[j], [s]" + Global::Colors::Reset,
        "   Top: " + Global::Colors::Directory + "[t]" + Global::Colors::Reset,
        "Bottom: " + Global::Colors::Directory + "[b]" + Global::Colors::Reset,
        "Finish: " + Global::Colors::Directory + "[f]" + Global::Colors::Reset,
        "Create: " + Global::Colors::Directory + "[n]" + Global::Colors::Reset,
        "Delete: " + Global::Colors::Directory + "[d]" + Global::Colors::Reset,
        "  Edit: " + Global::Colors::Directory + "[e]" + Global::Colors::Reset,
        "  Help: " + Global::Colors::Directory + "[h]" + Global::Colors::Reset
    };
    // clang-format on

    this->clearScreen();
    int contentSize = lines.size();
    this->center(contentSize);

    for (const std::string &line : lines) {
        std::cout << line << '\n';
    }

    std::cout << '\n'
              << "Press " << Global::Colors::Directory << "<Enter>"
              << Global::Colors::Reset << " to continue." << '\n';
    std::cin.get();
    return;
}

string Console::formatLine(string &line) {
    // check for any non numeric excluding the dot
    {
        string temp = "";
        for (size_t i = 0; i < line.size(); i += 1) {
            char c = line[i];
            const string valid = "- 1234567890.";
            // if string in valid chars
            if (valid.find(c) != string::npos) {
                temp += c;
            } else {
            }
        }
        line = temp;
    }
    // remove whitespaces
    line = trim(line);

    // remove duplicate spaces by removing all spaces and then inserting a
    // space in the appropriate place
    if (line.find(' ') != string::npos) {
        const int spaceIndex = line.find_first_of(' ');
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        line.insert(line.begin() + spaceIndex, ' ');
    }

    return line;
}

void Console::printBuffers() {
    if (this->buffer().size() != this->infoBuffer().size()) {
        throw out_of_range("Buffer and Info Buffer sizes do not match!!!!");
    }

    for (size_t i = 0; i < this->buffer().size(); i += 1) {
        const string infoLine = this->infoBuffer()[i];
        const string bufferLine = this->buffer()[i]; // bad nvim plugin lol

        string arrow = "";
        if (i == this->m_lineIndex) {
            arrow = "-> ";
        }

        // align it
        ostringstream oss;
        oss << setw(4) << arrow << setw(this->infoBuffer()[0].size())
            << infoLine << setw(0) << Global::Colors::Directory << bufferLine
            << Global::Colors::Reset;

        std::cout << oss.str() << '\n';
    }
    std::cout << '\n';
}

Console::Action Console::getAction() {
    while (true) {
        std::cout << "\nEnter Action ([H]elp)\n> ";

        std::string input;
        getline(cin, input);

        switch (tolower(input[0])) {
        case 'h': {
            return Action::HELP;
        }
        case 's':
        case 'j': {
            return Action::DOWN;
        }
        case 'w':
        case 'k': {
            return Action::UP;
        }
        case 'e': {
            return Action::EDIT;
        }
        case 'f': {
            return Action::DONE;
        }
        case 'd': {
            return Action::REMOVE;
        }
        case 'n': {
            return Action::NEW;
        }
        case 't': {
            return Action::TOP;
        }

        case 'b': {
            return Action::BOTTOM;
        }
        }
        this->setErrorMessage("Unknown action: " + input);
        return Action::NONE;
    }
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

// bullshit

vector<string> Console::setBuffer(const vector<string> &buff) {
    this->m_buffer = buff;
    return buff;
}
vector<string> Console::setInfoBuffer(const vector<string> &buff) {
    this->m_infoBuffer = buff;
    return buff;
}

// appending
vector<string> Console::appendBuffer(const string &x) {
    this->m_buffer.push_back(x);
    return this->buffer();
}
vector<string> Console::appendInfoBuffer(const string &x) {
    this->m_infoBuffer.push_back(x);
    return this->infoBuffer();
}

// inserting
vector<string> Console::insertBuffer(const int &index, const string &x) {
    vector<string> b = this->buffer();
    b.insert(b.begin() + index, x);
    this->setBuffer(b);
    return b;
}
vector<string> Console::insertInfoBuffer(const int &index, const string &x) {
    vector<string> b = this->infoBuffer();
    b.insert(b.begin() + index, x);
    this->setInfoBuffer(b);
    return b;
}

string Console::setErrorMessage(const string &x) {
    string str = Global::Colors::Error + x + Global::Colors::Reset;
    this->m_errorMessage = str;
    return x;
}

void Console::appendChainLinkPair(const string &x) {
    this->appendBuffer(x);
    const int num = this->buffer().size() - this->CHAIN_LINKS_STARTING_INDEX;
    const string str = to_string(num) + ": ";
    this->appendInfoBuffer(str);
}
