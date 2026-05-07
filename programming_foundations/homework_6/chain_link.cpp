#include "chain_link.h"
#include "polar.h"
#define _USE_MATH_DEFINES
#include <math.h> // C-specific include
//#include <cmath>
#include <memory>

using namespace std;
ChainLink::ChainLink(const shared_ptr<Polar::Position> &anchor, double &length,
                     double &relativeRotation) {

    anchor->relativeRotation = relativeRotation;
    this->setAnchor(anchor);
    this->setLength(length);
    Polar::Position end = this->calculateEnd();
    this->setEnd(end);
}

shared_ptr<Polar::Position>
ChainLink::setAnchor(const shared_ptr<Polar::Position> &x) {
    this->m_anchor = x;
    return x;
}

Polar::Position ChainLink::setEnd(Polar::Position &x) {
    this->m_end = x;
    return x;
}

double ChainLink::setLength(double &x) {
    this->m_length = x;
    return x;
}

Polar::Position ChainLink::calculateEnd() {

    Polar::Position pos{0, 0, 0, 0};
    shared_ptr<Polar::Position> anchor = this->anchor();

    const double abs = anchor->absoluteRotation + anchor->relativeRotation;

    const double radians = abs * (M_PI / 180);
    const double x = anchor->positionX + this->length() * cos(radians);
    const double y = anchor->positionY + this->length() * sin(radians);

    pos.absoluteRotation = abs;
    pos.relativeRotation = this->anchor()->relativeRotation;
    pos.positionX = x;
    pos.positionY = y;

    return pos;
}
