#include "components.h"
#include "global.h"
#include <algorithm>
#include <string>

using namespace std;
bool Components::isOpeningComponent(string line) {
    transform(line.begin(), line.end(), line.begin(), ::tolower);
    for (const string &opening : Global::OPENING_COMPONENTS) {
        if (line == opening) {
            return true;
        }
        if(line[0] == opening[0]) return true;
    }
    return false;
}

bool Components::isClosingComponent(string line) {
    transform(line.begin(), line.end(), line.begin(), ::tolower);
    for (const string &opening : Global::OPENING_COMPONENTS) {
        if (line == "end" + opening) {
            return true;
        }
    }
    return false;
}

bool Components::isLeafNode(string line) {
    transform(line.begin(), line.end(), line.begin(), ::tolower);
    for (const string &leaf : Global::LEAF_COMPONENTS) {
        if (line.substr(0, leaf.size()) == leaf) {
            return true;
        }
        if(line[0] == leaf[0]) return true;
    }
    return false;
}
