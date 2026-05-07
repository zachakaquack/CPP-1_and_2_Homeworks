#include "avl_tree.h"
#include <iostream>

int main() {

    using namespace dsa;
    avl_tree<int> values;

    values.insert(5);
    values.insert(7);
    values.insert(3);
    values.insert(2);
    values.insert(19);

    // does tree contain 19
    std::cout << std::boolalpha << "Testing contains: " << values.contains(19)
              << '\n';

    std::cout << "Testing a for loop" << '\n';
    for (auto iterator = values.cbegin(); iterator != values.cend();
         ++iterator) {
        std::cout << *iterator << " ";
    }
    std::cout << '\n';

    values.remove(7);
    std::cout << "Testing a standard for loop after a remove" << '\n';
    for (auto iterator = values.cbegin(); iterator != values.cend();
         ++iterator) {
        std::cout << *iterator << " ";
    }
    std::cout << '\n';

    values.insert(7);
    values.insert(-1);
    std::cout << "Testing re-insertion of a removed value" << '\n';
    for (auto iterator = values.cbegin(); iterator != values.cend();
         ++iterator) {
        std::cout << *iterator << " ";
    }
    std::cout << '\n';

    std::cout << "Now in reverse" << '\n';
    std::cout << "Testing a reverse for loop" << '\n';
    for (auto iterator = values.crbegin(); iterator != values.crend();
         ++iterator) {
        std::cout << *iterator << " ";
    }
    std::cout << '\n';
    values.remove(7);
    std::cout << "Testing a standard reverse for loop after a remove" << '\n';
    for (auto iterator = values.crbegin(); iterator != values.crend();
         ++iterator) {
        std::cout << *iterator << " ";
    }
    std::cout << '\n';

    values.insert(7);
    values.insert(-1); // should ignore this value
    std::cout << "Testing re-insertion of a removed value in reverse" << '\n';
    for (auto iterator = values.crbegin(); iterator != values.crend();
         ++iterator) {
        std::cout << *iterator << " ";
    }
    std::cout << '\n';

    std::cout << "Testing ++ and --" << '\n';
    auto iterator = values.crbegin();
    std::cout << "Current: " << *iterator << '\n';
    iterator++;
    std::cout << "Current: " << *iterator << '\n';
    iterator--;
    std::cout << "Current: " << *iterator << '\n';
    return 0;
}
