// 1024-gear_box.cpp
// PROG 1024
// <zachakaquack>

#include "gear_libary.h"
#include "validation.h"
#include <iostream>

int main() {

    using namespace GearBoxes;

    std::cout << "HW2 - Gear Box" << '\n';
    std::cout << "Solution by <zachakaquack>" << '\n';

    // stage count
    int stageCount =
        validation::validateInt("How Many Gearbox Stages? (1-10): ", 1, 10);

    // init gear box & its engine
    GearBox gb;
    gb.engine = validation::validateEngine();

    // get info on stages
    for (int i = 0; i < stageCount; i += 1) {
        std::cout << '\n'
                  << "For stage " << i + 1 << " of the gear box:" << '\n';

        // get values for gear box stage
        GearBoxStage gbs = validation::validateGearBoxStage();
        std::cout << '\n';

        // add the stage to the gear box itself, then print
        addStageToGearBox(gb, gbs);
        printGearBoxStageInitial(gbs, i);
    }

    std::cout << '\n';
    std::cout << "-----" << '\n';
    std::cout << '\n';

    // calc & print
    calculateFinalGearBox(gb);
    printGearBox(gb);

    std::cout << "HW2 Complete" << '\n';

    return 0;
}
