// PROG 1024
// Fall 2025
// HW3

#include "validation.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

int validateint(const std::string &question, int const &valmin,
                int const &valmax) {
    while (true) {
        std::cout << question;

        std::string value;
        std::getline(std::cin, value);

        // convert answer to all lowercase
        // https://www.geeksforgeeks.org/cpp/how-to-convert-std-string-to-lower-case-in-cpp/#using-stdtransform
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        // check if want to quit
        if (value == "quit" || value == "q") {
            return -1;
        }

        try {
            // call int initializer on string (stoi = string to int)
            int i;
            i = std::stoi(value);
            // if out of custom bounds, go straight to catch
            if (valmin != -1 && valmax != -1) {
                if (i < valmin || i > valmax) {
                    throw std::out_of_range("");
                }
            }
            return i;
        } catch (std::invalid_argument const &ex) {
            std::cout << "invalid answer! try again." << '\n';
        } catch (std::out_of_range const &ex) {
            std::cout << "out of range! try again." << '\n';
        }
    }
}
