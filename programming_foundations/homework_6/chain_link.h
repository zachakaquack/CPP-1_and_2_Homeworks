#pragma once

#include "polar.h"
#include <memory>

using namespace std;
class ChainLink {
  public:
    ChainLink(const shared_ptr<Polar::Position> &anchor, double &length,
              double &relativeRotation);
    ~ChainLink() {}

    shared_ptr<Polar::Position> anchor() { return this->m_anchor; }
    shared_ptr<Polar::Position> setAnchor(const shared_ptr<Polar::Position> &x);

    Polar::Position end() { return this->m_end; }
    Polar::Position setEnd(Polar::Position &x);

    double length() { return this->m_length; }
    double setLength(double &x);

    Polar::Position calculateEnd();

  private:
    shared_ptr<Polar::Position> m_anchor;
    Polar::Position m_end{0, 0, 0, 0};
    double m_length;
};
