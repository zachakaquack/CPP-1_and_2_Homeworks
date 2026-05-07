#include "parse.h"
#include "circuit.h"
#include "components.h"
#include "file.h"
#include "global.h"
#include <algorithm>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

Circuit Parse::_parse(vector<string> &lines) {
    Circuit *parentCircuit = new Circuit{};
    stack<Circuit *> nodes;
    int foundCircuits = 0;

    for (size_t i = 0; i < lines.size(); i += 1) {
        const string &line = lines[i];

        if (Components::isOpeningComponent(line)) {
            Circuit *childCircuit = getTypeOfCircuit(line);
            childCircuit->setName(line);
            nodes.push(parentCircuit);
            parentCircuit = childCircuit;
            continue;
        }

        if (Components::isLeafNode(line)) {
            Resistor *resistor = new Resistor;
            resistor->setResistance(getResistorOhms(line));
            resistor->setName(line);
            parentCircuit->appendToComponents(resistor);
            continue;
        }

        if (Components::isClosingComponent(line)) {
            nodes.top()->appendToComponents(parentCircuit);
            parentCircuit = nodes.top();
            nodes.pop();
            continue;
        }

        const string CIRCUIT = "circuit ";
        const string CKT = "ckt ";
        if ((line.substr(0, CIRCUIT.size()) == CIRCUIT ||
             line.substr(0, CKT.size()) == CKT) &&
            foundCircuits == 0) {
            // we have the name; now be careful of the | delim
            const vector<string> splitPipe = splitString(line, '|');
            const string name = splitPipe[0].substr(line.find(' ') + 1);
            parentCircuit->setName(name);
            foundCircuits += 1;

            continue;
        } else if ((line.substr(0, CIRCUIT.size()) == CIRCUIT ||
                    line.substr(0, CIRCUIT.size()) == CKT) &&
                   foundCircuits > 0) {
            cout << Global::Color::red
                 << "Multiple Circuits found. Assuming previous input was "
                    "complete."
                 << Global::Color::reset << '\n';
            break;
        }

        if (line == "endcircuit")
            continue;

        cout << Global::Color::red << "invalid line >> " << Global::Color::reset
             << '"' << line << '"' << '\n';
    }

    if (!nodes.empty()) {
        cout << Global::Color::red
             << "Some components were not closed, output may be inaccurate or "
                "incomplete. Please ensure the input file is formatted "
                "correctly."
             << Global::Color::reset << '\n';
        while (!nodes.empty()) {
            nodes.top()->appendToComponents(parentCircuit);
            parentCircuit = nodes.top();
            nodes.pop();
        }
    }

    return *parentCircuit;
}

Circuit Parse::parse(string &contents) {
    vector<string> lines = Parse::formatLines(contents);
    return Parse::_parse(lines);
}

Circuit Parse::parse(vector<string> &lines) {
    // make the lines work
    string str = "";
    for (string line : lines) {
        str += line + '\n';
    }

    vector<string> contents = Parse::formatLines(str);
    return Parse::_parse(contents);
}

Circuit *Parse::getTypeOfCircuit(const string &line) {
    if (line == "parallel" || line[0] == 'p') {
        return new ParallelCircuit{};
    }

    if (line == "serial" || line[0] == 's') {
        return new SerialCircuit{};
    }
    throw invalid_argument("invalid argument found: " + line);
}

double Parse::getResistorOhms(const string &line) {
    // wonderful
    return stod(splitString(line, ' ')[1]);
}

vector<string> Parse::formatLines(string &contents, char delim) {

    vector<string> rawLines = splitString(contents, delim);

    vector<string> lines;

    for (string line : rawLines) {
        vector<string> tempLines = splitString(line, '\r');
        if(!tempLines.empty())
            lines.push_back(tempLines[0]);
    }

    for (string &line : lines) {
        // tolower
        transform(line.begin(), line.end(), line.begin(), ::tolower);

        // remove whitespace
        size_t start = line.find_first_not_of(" \t");
        if (start != string::npos) {
            line = line.substr(start);
        } else {
            line.clear();
        }
    }

    return lines;
}
