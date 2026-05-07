#pragma once

namespace Polar {
struct Position {
    double positionX = 0;
    double positionY = 0;
    double absoluteRotation = 0;
    double relativeRotation = 0;

    // to use std::make_shared, we need a constructor for the struct
    Position(const double &x, const double &y, const double &abs,
             const double &rel)
        : positionX(x), positionY(y), absoluteRotation(abs),
          relativeRotation(rel) {};
};

struct Vector {
    double length = 0;
    double rotation = 0;
};
}; // namespace Polar
