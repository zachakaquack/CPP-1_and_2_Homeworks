#include "chain.h"
#include "parser.h"
#include <iostream>
#include <string>

void Chain::print() {

    for (size_t i = 0; i < this->size(); i += 1) {
        const shared_ptr<Polar::Position> anchor =
            this->get(i)->data().anchor();
        const Polar::Position ending = this->get(i)->data().end();
        std::cout << "Chain #" << (i + 1) << " begins at (" << anchor->positionX
                  << ", " << anchor->positionY << "), with absolute rotation "
                  << anchor->absoluteRotation << "° and relative rotation "
                  << anchor->relativeRotation << "°, and ends at ("
                  << ending.positionX << ", " << ending.positionY
                  << "), @ absolute rotation " << ending.absoluteRotation
                  << "°, relative rotation " << ending.relativeRotation << "°"
                  << '\n';
    }

    const Polar::Position last = this->end();
    std::cout << "Final chain ends at (" << last.positionX << ", "
              << last.positionY << "), @ absolute angle "
              << last.absoluteRotation << "°" << '\n';
}

void Chain::calculateEndChain() {
    // starting position
    const vector<string> split = splitString(this->lines()[0], ' ');
    this->m_starting =
        make_shared<Polar::Position>(stod(split[0]), stod(split[1]), 0.0, 0.0);

    // chain count
    this->m_chainCount = stod(this->lines()[1]);

    // offset to begin the actual position/rotation pairs
    const unsigned short OFFSET = 2;

    // create the list, and do all their calculations
    for (int i = OFFSET; i < this->chainCount() + OFFSET; i += 1) {
        const vector<string> split = splitString(this->lines()[i], ' ');
        double length = stod(split[0]);
        double rotation = stod(split[1]);

        if (length < 1 || length > 100) {
            cout << "Skipping line #" << (i + 1) << ": length not within the range (0, 100)."
                 << '\n';
            continue;
        }

        if (rotation < -90 || rotation > 90) {
            cout << "Skipping line #" << (i + 1)
                 << ": rotation is not within the range (-90, 90)" << '\n';
            continue;
        }

        shared_ptr<Polar::Position> anchor = this->starting();
        if (this->root() != nullptr) {
            anchor = make_shared<Polar::Position>(this->last()->data().end());
        }

        ChainLink link{anchor, length, rotation};
        shared_ptr<Node> node = make_shared<Node>(link);
        this->append(node);
    }

    // end
    this->m_end = this->last()->data().end();
}
