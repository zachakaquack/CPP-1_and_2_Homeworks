// PROG 1024
// Fall 2025
// HW3

#pragma once

#include <string>

/*
 * `@returns int`: -1 (if want to quit) | range(`valMin`, `valMax`)
 *
 * `@params`
 *
 * `std::string question`: question to ask at the start or if invalid
 *
 * `int valMin`: min value to accept
 * `int valMax`: max value to accept
 */
int validateInt(const std::string &question, int const &valMin = -1,
                int const &valMax = -1);
