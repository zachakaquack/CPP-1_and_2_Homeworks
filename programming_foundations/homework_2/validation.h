#pragma once

#include "gear_libary.h"
#include <string>

namespace validation {
GearBoxes::Engine validateEngine();
GearBoxes::GearBoxStage validateGearBoxStage();
int validateInt(const std::string &question, int const &valMin = -1,
                int const &valMax = -1);
double validateDouble(std::string const &question);
}; // namespace validation
