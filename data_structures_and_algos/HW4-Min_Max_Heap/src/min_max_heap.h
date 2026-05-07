#include <algorithm>
#include <cmath>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <vector>
#pragma once

namespace Color {
    const std::string Reset = "\033[0m";
    const std::string Index = "\033[94m";
}

namespace dsa {

// min max heap implementation - custom objects using this must implement at
// least the < and > operator, and also stream overload if you want to print
template <typename T> class min_max_heap {
  public:
    min_max_heap() {};
    // inserts an item into the heap
    void insert(const T &x);

    // we only need this size function because for some reason prof may wants
    // the print looking like (index+1):item FOR SOME REASON COME ON BRO
    int size() const { return this->m_array.size(); };

    // same for this one i cant believe you @may
    T get(const int &index) const { return this->m_array[index]; }

    // returns the "lowest" element within the heap
    T find_min() const;

    // returns the "highest" element within the heap
    T find_max() const;

    // removes the "lowest" element from the heap
    void delete_min();

    // removes the "largest" element from the heap
    void delete_max();

    friend std::ostream &operator<<(std::ostream &out, min_max_heap<T> heap) {
        for (size_t i = 0; i < heap.size(); i += 1) {
            const T item = heap.get(i);
            std::cout << Color::Index << (i + 1) << Color::Reset << ":" << item << ' ';
        }
        return out;
    }

  private:
    std::vector<T> m_array{};

    // figure out what level of depth the given index is and return whether or
    // not it is odd, which correlates to whether or not the current level is a
    // min or a max level. odd = min level, even = max level
    bool f_isOddLevel(const int &index) const;

    // recursively swaps values in the array to "complete" the heap and keep the
    // heap property validated
    void f_percolate(const int &index, const bool max);

    // finds the corresponding parent index based off of the index
    int f_findParentIndex(const int &index) const;
    // finds the corresponding parent index based off of the index
    int f_findGrandParentIndex(const int &index) const;

    int f_findChild(const int &index, const bool findMax) const;
    int f_findGrandChild(const int &index, const bool findMax) const;

    void f_trickleDown(const int &index, const bool findMax);
};

// implementation details only below here

// inserts an item into the heap by fucking around with the percolates until the
// min max heap property is valid again
template <typename T> void min_max_heap<T>::insert(const T &x) {
    const int index = this->m_array.size();
    this->m_array.push_back(x);

    const bool isOddLevel = this->f_isOddLevel(index);
    this->f_percolate(index, !isOddLevel);

}

// literally just returns the first item if possible because its a min_max heap,
// not a max_min heap
template <typename T> T min_max_heap<T>::find_min() const {
    if (this->m_array.size() < 1) {
        throw std::out_of_range(
            "CANNOT FIND MIN! THERE ARE NOT ENOUGH ITEMS IN THE VECTOR");
    }

    return this->m_array[0];
}
// just returns the greater of the two children of the root node, because these
// two nodes are the two highest nodes within the entire tree, due to the min
// max heap properties
template <typename T> T min_max_heap<T>::find_max() const {
    if (this->m_array.size() == 0) {
        throw std::out_of_range(
            "CANNOT FIND MAX! THERE ARE NOT ENOUGH ITEMS IN THE VECTOR");
    } else if (this->m_array.size() == 1) {
        return this->m_array[0];
    } else if (this->m_array.size() == 2) {
        return this->m_array[1];
    }

    return std::max(this->m_array[1], this->m_array[2]);
}

// removes the smallest item of the list by making arr[0] into arr[-1],
// and "trickles down" from that root
template <typename T> void min_max_heap<T>::delete_min() {
    if (this->m_array.size() == 1) {
        this->m_array.pop_back();
        return;
    }

    const size_t lastIndex = this->m_array.size() - 1;
    this->m_array[0] = this->m_array[lastIndex];
    this->m_array.pop_back();

    this->f_trickleDown(0, false);
}

// unknown how this is gonna work ngl i havent thought it through
template <typename T> void min_max_heap<T>::delete_max() {
    if (this->m_array.size() <= 2) {
        this->m_array.pop_back();
        return;
    }

    int biggerItemIndex = 1;
    if (this->m_array.size() > 2) {
        if (this->m_array[biggerItemIndex] <
            this->m_array[biggerItemIndex + 1]) {
            biggerItemIndex++;
        }
    }

    const size_t lastIndex = this->m_array.size() - 1;
    this->m_array[biggerItemIndex] = this->m_array[lastIndex];
    this->m_array.pop_back();

    this->f_trickleDown(biggerItemIndex, true);
}

// figure out what level of depth the given index is and return whether or
// not it is odd, which correlates to whether or not the current level is a
// min or a max level. odd = min level, even = max level
template <typename T>
bool min_max_heap<T>::f_isOddLevel(const int &index) const {
    // edge case where index is zero, so its min
    if (index == 0) {
        return true;
    }

    int depth = 1;
    while (true) {
        // shut up grant
        const int twoPowerDepth = std::pow(2, depth) - 1;
        const int twoPowerDepthPlusOne = std::pow(2, depth + 1) - 1;

        if (twoPowerDepth <= index && index < twoPowerDepthPlusOne) {
            depth++;
            break;
        } else {
            depth++;
        }
    }
    // we now have our depth, we big chilling
    return (depth % 2 == 1);
}

// recursively swaps values in the array to "complete" the heap and keep the
// heap property validated
template <typename T> void min_max_heap<T>::f_percolate(const int &index, const bool maxMode) {
    const int parentIndex = this->f_findParentIndex(index);
    const int grandParentIndex = this->f_findGrandParentIndex(index);

    if ((this->m_array[index] > this->m_array[parentIndex] && !maxMode) || 
        (this->m_array[index] < this->m_array[parentIndex] && maxMode)) {
        std::swap(this->m_array[parentIndex], this->m_array[index]);
        this->f_percolate(parentIndex, !maxMode);
    }

    if (parentIndex == grandParentIndex) {
        return;
    }

    if ((this->m_array[index] < this->m_array[grandParentIndex] && !maxMode) || 
        (this->m_array[index] > this->m_array[grandParentIndex] && maxMode)) {
        std::swap(this->m_array[grandParentIndex], this->m_array[index]);
        this->f_percolate(grandParentIndex, maxMode);
    }
}

// finds the corresponding parent index based off of the index
template <typename T>
int min_max_heap<T>::f_findParentIndex(const int &index) const {
    if (index == 0) {
        return 0;
    }

    const bool isLeftChild = (index % 2 == 1);
    if (isLeftChild) {
        return (index - 1) / 2;
    }
    return (index - 2) / 2;
}

// finds the corresponding parent index based off of the index
template <typename T>
int min_max_heap<T>::f_findGrandParentIndex(const int &index) const {
    const int parentIndex = this->f_findParentIndex(index);
    return this->f_findParentIndex(parentIndex);
}

template <typename T> void min_max_heap<T>::f_trickleDown(const int &index, const bool findMax) {
    // this returning the index provided indicates that there are no grandchildren; so we must check children
    const int grandChildIndex = this->f_findGrandChild(index, findMax);

    // if the grand child swaps, then we need to trickle down again
    if (grandChildIndex > index) {
        std::swap(this->m_array[grandChildIndex], this->m_array[index]);
        this->f_trickleDown(grandChildIndex, findMax);
    }

    // check child; swap the smallest/largest of the children to where we are
    const int childIndex = this->f_findChild(index, findMax);
    
    // if we swap with a child, that means there are no grandchildren
    if (childIndex > index) {
        std::swap(this->m_array[childIndex], this->m_array[index]);
    }
};

template <typename T>
int min_max_heap<T>::f_findChild(const int &index, const bool findMax) const {
    const int size = 2;
    const int childrenIndexes[size] = {
        2 * index + 1, // left  child
        2 * index + 2, // right child
    };

    int foundIndex = index;
    // find the index of the smallest child
    for (size_t i = 0; i < size; i += 1) {
        const int childIndex = childrenIndexes[i];
        // case where we are out of children to look through
        if (childIndex > this->m_array.size() - 1) {
            break;
        }

        if ((this->m_array[childIndex] >= this->m_array[foundIndex] && !findMax) ||
            (this->m_array[childIndex] <= this->m_array[foundIndex] && findMax)) {
            continue;
        }

        foundIndex = childIndex;
    }
    return foundIndex;
}

template <typename T>
int min_max_heap<T>::f_findGrandChild(const int &index, const bool findMax) const {
    const int size = 4;
    const int grandChildrenIndexes[size] = {
        2 * (2 * index + 1) + 1, // left left
        2 * (2 * index + 1) + 2, // left right
        2 * (2 * index + 2) + 1, // right left
        2 * (2 * index + 2) + 2, // right right
    };

    int foundIndex = index;
    // find the index of the smallest grandchild
    for (size_t i = 0; i < size; i += 1) {
        const int grandChildIndex = grandChildrenIndexes[i];
        // case where we are out of grandchildren to look through
        if (grandChildIndex > this->m_array.size() - 1) {
            break;
        }

        if ((this->m_array[grandChildIndex] > this->m_array[foundIndex] && !findMax) ||
            (this->m_array[grandChildIndex] < this->m_array[foundIndex] && findMax)) {
            continue;
        }

        foundIndex = grandChildIndex;
    }

    return foundIndex;
}

} // namespace dsa
