// PROG 1024
// Fall 2025
// HW3

#include "color.h"
#include <algorithm>
#include <iostream>
#include <utility>

int Colors::Color::toColorNumber() {
    return (65536 * this->red) + (256 * this->green) + (this->blue);
};

std::string Colors::Color::toHex() {
    // https://cplusplus.com/forum/general/274256/
    std::ostringstream oss;
    oss << std::hex << this->toColorNumber();
    return oss.str();
}

int Colors::linearInterpolate(int &input, bool const &inverse) {

    // y = y_0 + ((y_1 - y_0) * (x - x_0)) / (x_1 - x_0)
    // where y = output range,
    //       x = input range

    // mod the value to stay within the (0, 25) range
    input %= 25;

    // x = (0, 25), y = (0, 255) || (255, 0)
    int x_0 = 0;
    int y_0 = 0;
    int x_1 = 25;
    int y_1 = 255;

    if (inverse) {
        std::swap(y_0, y_1);
    }

    return (y_0 + ((y_1 - y_0) * (input - x_0)) / (x_1 - x_0));
}

Colors::Color Colors::getColorFromRange(int &value) {

    // inverse indicates the range goes from 255-0, rather than 0-255
    bool inverse;

    // clamp between 0, 100
    value = std::clamp(value, 0, 100);

    // -inf - 25
    if (value <= 25) {
        inverse = false;
        int interp = linearInterpolate(value, inverse);
        return Color{0, interp, 255};

        // 25 - 50
    } else if (value > 25 && value <= 50) {
        inverse = true;
        int interp = linearInterpolate(value, inverse);
        return Color{0, 255, interp};

        // 50 - 75
    } else if (value > 50 && value <= 75) {
        inverse = false;
        int interp = linearInterpolate(value, inverse);
        return Color{interp, 255, 0};

        // 75 - inf
    } else {
        inverse = true;
        int interp = linearInterpolate(value, inverse);
        return Color{255, interp, 0};
    }
}
