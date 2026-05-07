#include "set.h"
#include <iostream>

int main() {
    using namespace dsa;

    set<std::string> names;

    std::cout << "Adding names with a duplicate - natural order" << '\n';

    names.add("Shane");
    names.add("Daniel");
    names.add("Shane");
    names.add("Tamara");
    names.add("Lela");

    std::cout << "Print after adding " << '\n';
    std::cout << "For each loop " << '\n';
    for (const auto &name : names) {
        std::cout << name << '\n';
    }

    std::cout << '\n'
              << std::boolalpha << "Am I empty : " << names.empty() << '\n';
    std::cout << "How many elements : " << names.size() << '\n';

    std::cout << std::boolalpha
              << "Do I contain Daniel : " << names.contains("Daniel") << '\n';

    std::cout << std::boolalpha
              << "Do I contain Annabelle : " << names.contains("Annabelle")
              << '\n';

    std::cout << "Remove Shane and try to find " << '\n';
    names.remove("Shane");
    std::cout << std::boolalpha
              << "Do I contain Shane : " << names.contains("Shane") << '\n';

    std::cout << "For each loop " << '\n';
    for (const auto &name : names) {
        std::cout << name << '\n';
    }

    std::cout << '\n' << "Diagnostic print " << '\n';
    std::cout << names << '\n';

    std::cout << "Iterators " << '\n';
    auto iterator = names.begin();
    while (iterator != names.end()) {
        std::cout << *iterator << '\n';
        ++iterator;
    }

    set<double> numbers(false);
    std::cout << '\n' << '\n';
    std::cout << "Adding numbers with a duplicate - reverse order" << '\n';

    numbers.add(2.71828);
    numbers.add(9.0);
    numbers.add(9.0);
    numbers.add(10.6);
    numbers.add(3.1415926);

    std::cout << "Print after adding " << '\n';
    std::cout << "For each loop " << '\n';
    for (const auto &number : numbers) {
        std::cout << number << '\n';
    }

    std::cout << '\n'
              << std::boolalpha << "Am I empty : " << numbers.empty() << '\n';

    std::cout << "How many elements : " << numbers.size() << '\n';

    std::cout << std::boolalpha
              << "Do I contain PI : " << numbers.contains(3.1415926) << '\n';

    std::cout << std::boolalpha << "Do I contain 12 : " << numbers.contains(12)
              << '\n';

    std::cout << "Remove 9.0 and try to find " << '\n';
    numbers.remove(9.0);

    std::cout << std::boolalpha
              << "Do I contain 9.0 : " << numbers.contains(9.0) << '\n';

    std::cout << "For each loop " << '\n';
    for (const auto &number : numbers) {
        std::cout << number << '\n';
    }

    std::cout << '\n' << "Diagnostic print " << '\n';
    std::cout << numbers << '\n';

    std::cout << "Iterators " << '\n';
    auto number_iterator = numbers.begin();
    while (number_iterator != numbers.end()) {
        std::cout << *number_iterator << '\n';
        ++number_iterator;
    }
    return 0;
}
