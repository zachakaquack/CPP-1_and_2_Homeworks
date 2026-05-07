#pragma once

namespace GearBoxes {
struct GearBoxStage {
    // values that get initialized
    int inputGearTeeth;
    int outputGearTeeth;
    double gearRatio;
    double efficiencyPercent;

    // values that get calculated
    double inputRPM;
    double outputRPM;
    double inputTorque;
    double outputTorque;
};

struct Engine {
    int outputRPM;
    int outputTorqueNM;
};

struct GearBox {
    Engine engine;
    GearBoxStage stages[10];
    int stageCount = 0;
    double finalOutputSpeedRPM;
    double finalOutputTorqueNM;
};

void addStageToGearBox(GearBox &gb, GearBoxStage &gbs);
GearBox createGearBox(Engine const &engine, GearBoxStage const (&stages)[10],
                      int const &stageCount, double const &finalOutputSpeedRPM,
                      double const &finalOutputTorqueNM);
void calculateFinalGearBox(GearBox &gb);
void printGearBox(GearBox const &gb);
void printGearBoxStageInitial(GearBoxStage &gbs, int number);
}; // namespace GearBoxes
