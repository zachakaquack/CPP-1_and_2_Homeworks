#include "validation.h"
#include "gear_libary.h"

#include <iostream>

namespace validation {
GearBoxes::Engine validateEngine() {
    using namespace GearBoxes;
    Engine eng;

    eng.outputRPM = validateDouble("What is the engine's output RPM? (revs): ");
    eng.outputTorqueNM =
        validateDouble("What is the engine's output torque? (Nm): ");

    return eng;
}

GearBoxes::GearBoxStage validateGearBoxStage() {
    using namespace GearBoxes;

    GearBoxStage gbs;

    gbs.inputGearTeeth = validateInt("Input Teeth #: ");
    gbs.outputGearTeeth = validateInt("Output Teeth #: ");
    gbs.efficiencyPercent =
        validateInt("Efficiency % (0-100): ", 1, 100) / 100.0;
    gbs.gearRatio = gbs.inputGearTeeth * 1.0 / gbs.outputGearTeeth;

    return gbs;
}

int validateInt(const std::string &question, int const &valMin,
                int const &valMax) {
    while (true) {
        std::cout << question;

        std::string value;
        std::getline(std::cin, value);
        try {
            // call int initializer on string (stoi = string to int)
            int i;
            i = std::stoi(value);
            if (valMin != -1 && valMax != -1) {
                if (i < valMin || i > valMax) {
                    throw std::invalid_argument("");
                }
            }
            return i;
        } catch (std::invalid_argument const &ex) {
            std::cout << "Invalid answer! Try again." << '\n';
        } catch (std::out_of_range const &ex) {
            std::cout << "Out of range! Try again." << '\n';
        }
    }
}
double validateDouble(const std::string &question) {
    while (true) {
        std::cout << question;

        std::string value;
        std::getline(std::cin, value);
        try {
            // call double initializer on string (stoi = string to int)
            double d;
            d = std::stod(value);
            return d;
        } catch (std::invalid_argument const &ex) {
            std::cout << "Invalid answer! Try again." << '\n';
        } catch (std::out_of_range const &ex) {
            std::cout << "Out of range! Try again." << '\n';
        }
    }
}
} // namespace validation
