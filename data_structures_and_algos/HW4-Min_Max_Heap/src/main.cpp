#include "min_max_heap.h"
#include <iostream>
#include <vector>

int main() {
    using namespace dsa;

    min_max_heap<int> mmh;
    std::vector<int> numbers{93, 16, 60, 87, 2, 89, 53, 49, 16, 62, 83};

    for (const auto &number : numbers) {
        mmh.insert(number);
    }

    std::cout << mmh << '\n';
    std::cout << "Min: ";

    for (auto counter = 1; counter < 12; counter++) {
        std::cout << mmh.find_min() << " ";
        mmh.delete_min();
    }

    for (const auto &number : numbers) {
        mmh.insert(number);
    }

    std::cout << '\n';
    std::cout << mmh << '\n';

    std::cout << "Max: ";
    for (auto counter = 1; counter < 12; counter++) {
        std::cout << mmh.find_max() << " ";
        mmh.delete_max();
    }
    std::cout << '\n';
    return 0;
}
