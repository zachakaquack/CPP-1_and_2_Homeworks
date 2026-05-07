// PROG 1024
// Fall 2025
// HW3

#pragma once
#include <iomanip>
#include <string>

namespace Colors {

struct Color {
    int red;
    int green;
    int blue;

    // `@returns int`: returns the "color number" of the color
    int toColorNumber();
    // `@returns std::string`: returns like "b2b2b2" without #
    std::string toHex();

    // https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading
    friend std::ostream &operator<<(std::ostream &os, Color &color) {
        os << std::setw(15) << "Color Hex: " << std::setw(7) << color.toHex()
           << '\n'
           << std::setw(15) << "Color Number: " << std::setw(7)
           << color.toColorNumber();
        return os;
    }
};

/*
 * `@returns int` that is interpolated from 0-25 to either 0-255 or 255-0 based
 * off of `inverse`
 * `@params`
 *
 * `int input`: value to interpolate
 *
 * `bool inverse`: whether to interpolate over 0-255, or 255-0
 */
int linearInterpolate(int &input, bool const &inverse);

/*
 * `@returns Color` struct based off of value
 * `@params int value`: linearlly interpolated value
 */
Color getColorFromRange(int &value);

}; // namespace Colors
