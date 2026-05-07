// PROG 1024
// Fall 2025
// HW3

#include "color.h"
#include "validation.h"
#include <iostream>

int main() {

    // intro
    std::cout << "HW3" << '\n';
    std::cout << "Solution by <zachakaquack>" << '\n';

    // main loop
    while (true) {
        int value = validateInt("Enter a temperature (0-100) >: ", 0, 100);

        // value == -1 means quit (from validation)
        if (value == -1) {
            std::cout << "Quitting..." << '\n';
            break;
        }

        Colors::Color color = Colors::getColorFromRange(value);
        std::cout << color << '\n' << '\n';
    }

    std::cout << "HW3 Complete" << '\n';

    return 0;
}
