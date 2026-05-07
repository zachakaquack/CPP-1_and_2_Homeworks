#include "circuit.h"
#include <algorithm>
#include <math.h>
#include <sstream>
#include <vector>

using namespace std;

Circuit::Circuit(const string &name) { this->setName(name); }

string Circuit::setName(string name) {
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    this->m_name = name;
    return name;
}

vector<Circuit *> Circuit::setComponents(vector<Circuit *> list) {
    this->m_components = list;
    return this->m_components;
}
vector<Circuit *> Circuit::appendToComponents(Circuit *component) {
    vector<Circuit *> list = this->components();
    list.push_back(component);
    this->setComponents(list);
    return list;
}
vector<Circuit *> Circuit::appendToComponents(vector<Circuit *> components) {
    vector<Circuit *> list = this->components();
    for (Circuit *component : components) {
        list.push_back(component);
    }
    this->setComponents(list);
    return list;
}

string Circuit::toString(int depth) {
    ostringstream out;

    const string layerString = string(depth * 2, ' ');
    const double ohms = this->resistance();

    out << layerString << "Circuit " << this->m_name << " | " << ohms << " Ohms"
        << '\n';

    depth += 1;
    for (Circuit *component : this->components()) {
        out << component->toString(depth);
    }
    out << layerString << "EndCircuit" << '\n';

    return out.str();
}

string SerialCircuit::toString(int depth) {
    ostringstream out;

    const string layerString = string(depth * 2, ' ');
    out << layerString << "Serial" << '\n';

    depth += 1;
    for (Circuit *component : this->components()) {
        out << component->toString(depth);
    }
    out << layerString << "EndSerial" << '\n';

    return out.str();
}

string ParallelCircuit::toString(int depth) {
    ostringstream out;

    const string layerString = string(depth * 2, ' ');
    out << layerString << "Parallel" << '\n';

    depth += 1;
    for (Circuit *component : this->components()) {
        out << component->toString(depth);
    }
    out << layerString << "EndParallel" << '\n';

    return out.str();
}

string Resistor::toString(int depth) {
    ostringstream out;

    const string layerString = string(depth * 2, ' ');
    out << layerString << "Resistor " << this->resistance() << '\n';

    return out.str();
}

ostream &operator<<(ostream &out, Circuit &circuit) {
    out << circuit.toString();
    return out;
}

// Calculations
double Circuit::calcResistance() {
    double sum = 0;
    for (size_t i = 0; i < this->components().size(); i += 1) {
        sum += this->components()[i]->resistance();
    }
    return sum;
}

double ParallelCircuit::calcResistance() {
    double sum = 0;
    for (size_t i = 0; i < this->components().size(); i += 1) {
        sum += (1 / this->components()[i]->resistance());
    }
    return 1 / sum;
}

double Resistor::setResistance(const double &resistance) {
    this->m_resistance = resistance;
    return resistance;
}
