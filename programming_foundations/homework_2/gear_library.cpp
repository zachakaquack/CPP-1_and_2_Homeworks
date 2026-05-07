#include "gear_libary.h"
#include <iomanip>
#include <iostream>

namespace GearBoxes {

void addStageToGearBox(GearBox &gb, GearBoxStage &gbs) {
    if (gb.stageCount > 10) {
        throw std::overflow_error("Stage count overflow! Max: 10");
    }

    gb.stages[gb.stageCount] = gbs;
    gb.stageCount += 1;
}

GearBox createGearBox(Engine const &engine, GearBoxStage const (&stages)[10],
                      int const &stageCount, double const &finalOutputSpeedRPM,
                      double const &finalOutputTorqueNM) {
    GearBox gb;

    gb.engine = engine;
    // assign stages, arrays cant be an lvalue
    for (int i = 0; i < stageCount; i += 1) {
        gb.stages[i] = stages[i];
    }
    gb.stageCount = stageCount;
    gb.finalOutputSpeedRPM = finalOutputSpeedRPM;
    gb.finalOutputTorqueNM = finalOutputTorqueNM;

    return gb;
}

void calculateFinalGearBox(GearBox &gb) {

    // for the first stage of the gearbox, the output RPM of the
    // engine is the input RPM of the stage; same thing with torque
    double prevOutput = gb.engine.outputRPM;
    double prevTorque = gb.engine.outputTorqueNM;
    // output RPM    = input * gear ratio
    // output torque = input torque * eff / gear ratio

    for (int i = 0; i < gb.stageCount; i += 1) {
        GearBoxStage &gbs = gb.stages[i];

        gbs.inputRPM = prevOutput;
        gbs.inputTorque = prevTorque;

        prevOutput *= gbs.gearRatio;
        prevTorque *= gbs.efficiencyPercent / gbs.gearRatio;

        gbs.outputRPM = prevOutput;
        gbs.outputTorque = prevTorque;
    }

    // finalize
    gb.finalOutputSpeedRPM = prevOutput;
    gb.finalOutputTorqueNM = prevTorque;
}

void printGearBoxStageInitial(GearBoxStage &gbs, int number) {
    int width = 15;
    std::cout << "Performance of Stage #" << number + 1 << ":" << '\n';

    std::cout << std::setw(width) << "Input Teeth: " << gbs.inputGearTeeth
              << '\t';
    std::cout << std::setw(width) << "Output Teeth: " << gbs.outputGearTeeth
              << '\t' << '\n';

    std::cout << std::setw(width) << "Gear Ratio: " << gbs.gearRatio << '\t';
    std::cout << std::setw(width) << "Efficiency: " << gbs.efficiencyPercent
              << "%" << '\t' << '\n'
              << '\n';
}

void printGearBoxStage(GearBoxStage const &gbs, int number) {
    int width = 15;
    std::cout << "Performance of Stage #" << number + 1 << ":" << '\n';

    std::cout << std::setw(width) << "Input Teeth: " << gbs.inputGearTeeth
              << '\t';
    std::cout << std::setw(width) << "Output Teeth: " << gbs.outputGearTeeth
              << '\t' << '\n';

    std::cout << std::setw(width) << "Gear Ratio: " << gbs.gearRatio << '\t';
    std::cout << std::setw(width) << "Efficiency: " << gbs.efficiencyPercent
              << "%" << '\t' << '\n'
              << '\n';

    std::cout << std::setw(width) << "Input RPM: " << gbs.inputRPM << '\t';
    std::cout << std::setw(width) << "Output RPM: " << gbs.outputRPM << '\t'
              << '\n';

    std::cout << std::setw(width) << "Input Torque: " << gbs.inputTorque
              << '\t';
    std::cout << std::setw(width) << "Output Torque: " << gbs.outputTorque
              << '\t' << '\n'
              << '\n';
}

void printGearBox(GearBox const &gb) {
    std::cout << "Gear Box Stage Count: " << gb.stageCount << '\n';

    int width = 15;
    for (int i = 0; i < gb.stageCount; i += 1) {
        printGearBoxStage(gb.stages[i], i);
        std::cout << '\n';
    }

    std::cout << "-----" << '\n' << '\n';
    std::cout << "Final Output Speed: " << gb.finalOutputSpeedRPM << " RPM"
              << '\n';
    std::cout << "Final Output Torque: " << gb.finalOutputTorqueNM << " Nm"
              << '\n';
}
}; // namespace GearBoxes
