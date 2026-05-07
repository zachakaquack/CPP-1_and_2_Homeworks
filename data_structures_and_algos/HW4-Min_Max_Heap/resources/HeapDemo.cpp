// HeapDemo.cpp
// Mar. 17, 2026
// <name censored, from professor>

// From StackOverflow.com
#include "MaxHeap.h"
#include <iostream>

int main() {
    MaxHeap<int> h(10);
    h.insert(3);
    h.insert(10);
    h.insert(2);
    h.insert(8);
    h.insert(5);
    h.insert(4);

    std::cout << "Heap elements: ";
    h.printHeap();

    std::cout << "Maximum element: " << h.getMax() << std::endl;

    std::cout << "Extracting max element: " << h.extractMax() << std::endl;
    std::cout << "Heap after extraction: ";
    h.printHeap();

    std::cout << "Maximum element after extraction: " << h.getMax()
              << std::endl;

    return 0;
}
