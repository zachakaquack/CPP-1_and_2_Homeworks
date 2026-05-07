#pragma once
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

// Base Circuit
class Circuit {
  public:
    // defaults
    Circuit(const string &name = "Generic Circuit");
    ~Circuit() { /* do nothing */ };

    // bloats
    string setName(string name);
    string name() { return this->m_name; }

    // the total resistance of the entire circuit
    virtual double resistance() { return this->calcResistance(); }

    vector<Circuit *> setComponents(vector<Circuit *> list);
    vector<Circuit *> components() { return this->m_components; }
    vector<Circuit *> appendToComponents(Circuit *component);
    vector<Circuit *> appendToComponents(vector<Circuit *> components);


	virtual string toString(int depth = 0);
	friend ostream &operator<<(ostream &out, Circuit &cir);

  private:
    double calcResistance();

    string m_name = "";
    vector<Circuit *> m_components;
};

// we dont actually need anything here...
// it follows the same calculations and everything as Circuit
class SerialCircuit : public Circuit {
  public:
    SerialCircuit() {};
    ~SerialCircuit() { /* do nothing */ };

	string toString(int depth = 0) override;

  private:
    vector<Circuit *> m_components;
};

class ParallelCircuit : public Circuit {
  public:
    ParallelCircuit() {};
    ~ParallelCircuit() { /* do nothing */ };

    // bloats
    double resistance() override { return this->calcResistance(); }

	string toString(int depth = 0) override;

  private:
    // the actual calculations
    // 1/TR = 1/(sum of all resistors)
    double calcResistance();

    // to be calculated once all components are defined
    vector<Circuit *> m_components;
};

class Resistor : public Circuit {
  public:
    Resistor() {};
    ~Resistor() { /* do nothing */ };

    // Userdefined for Resistors, no limits specified
    double setResistance(const double &resistance);
    double resistance() override { return this->m_resistance; }

	string toString(int depth = 0) override;

  private:
    double m_resistance = 0;
};