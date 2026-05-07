#include "sort.h"
#include "globals.h"
#include "logger.h"
#include "time_handler.h"
#include "timer.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

TimeHandler Sorter::m_handler;
Logger Sorter::m_logger;

std::random_device rd;
std::mt19937 gen(rd());

// ugly ass function. but its all for the good formatting :)
void Sorter::startSort(std::vector<short> vector,
                       const SortAlgorithm &algorithm, const int &size,
                       std::promise<bool> promise, const int &size_index = -1) {
    const std::string name = algorithmToName[algorithm];
    printDottedLine("\033[94m\033[0m STARTING", name);

    this->m_logger.log("Starting: " + name + " (size: " + std::to_string(size) +
                       ")");

    // predefined skips
    const bool skip =
        (size > 500'000 && algorithm == SortAlgorithm::INSERTION) ||
        (size > 10'000'000 && algorithm == SortAlgorithm::SHELL);
    if (skip) {
        promise.set_value(true);
        printDottedLine("\033[31m\033[0m FINISHED " + name, " SKIPPED");
        this->m_handler.insertTime(name, 0);
        this->m_logger.log("Finished: " + name + " (N/A secs)");
        return;
    }

    // handling of the sort
    const Timer t;
    this->f_handleSort(vector, algorithm, size_index);
    const double secondsElapsed = t.elapsed();

    // finish all the printings
    std::string sorted = std::is_sorted(vector.begin(), vector.end())
                             ? "\033[32m\033[0m"
                             : "\033[31m\033[0m";

    promise.set_value(true);
    printDottedLine(sorted + " FINISHED " + name,
                    std::to_string(secondsElapsed) + " secs");

    this->m_logger.log("Finished: " + name + " (" +
                       std::to_string(secondsElapsed) + " secs)");

    this->m_handler.insertTime(name, secondsElapsed);
    return;
};

// finds the correct sort to do
void Sorter::f_handleSort(std::vector<short> &vector,
                          const SortAlgorithm &algorithm,
                          const int &size_index) {
    using SA = Sorter::SortAlgorithm;
    switch (algorithm) {
    case SA::INSERTION:
        return this->f_InsertionSort(vector);
    case SA::SHELL:
        return this->f_ShellSort(vector);
    case SA::QUICK:
        return this->f_QuickSort(vector, 0, vector.size() - 1,
                                 QuickPivotMode::MEDIAN);
    case SA::QUICK_RANDOMELEMENT:
        return this->f_QuickSort(vector, 0, vector.size() - 1,
                                 QuickPivotMode::RANDOM);
    case SA::QUICK_MEDIANTHREE:
        return this->f_QuickSort(vector, 0, vector.size() - 1,
                                 QuickPivotMode::MEDIAN3);
    case SA::QUICK_MEDIANFIVE:
        return this->f_QuickSort(vector, 0, vector.size() - 1,
                                 QuickPivotMode::MEDIAN5);
    case SA::MERGE:
        return this->f_MergeSort(vector, 0, vector.size() - 1);
    case SA::RADIX:
        return this->f_RadixSort(vector);
    case SA::HEAPSORT:
        return this->f_HeapSort(vector);
    case SA::STANDARD:
        return this->f_StandardSort(vector);
    case SA::SHELL_CUIRA:
        return this->f_ShellCuiraSort(vector, size_index);
    case SA::SHELL_KNUTH:
        return this->f_ShellKnuthSort(vector);
    default:
        std::cout << "UNKNOWN SORTING ALGORITHM" << '\n';
        throw std::out_of_range(
            "UNKNOWN SORTING ALGORITHM!!! PANIC!! HOW DID YOU DO THAT?");
    }
}

// ALL SORTS BELOW HERE
void Sorter::f_InsertionSort(std::vector<short> &vector) {
    for (size_t i = 1; i < vector.size(); i += 1) {
        const int item = vector[i];
        int j = i - 1;

        while (j >= 0 && vector[j] > item) {
            vector[j + 1] = vector[j];
            j -= 1;
        }
        vector[j + 1] = item;
    }
}
void Sorter::f_ShellSort(std::vector<short> &vector) {
    for (int gap = vector.size() / 2; gap > 0; gap /= 2) {

        // Perform a "gapped" insertion sort for this gap size
        for (size_t i = gap; i < vector.size(); i++) {

            // Current element to be placed correctly
            int temp = vector[i];
            int j = i;

            // Shift elements that are greater than temp to make space
            while (j >= gap && vector[j - gap] > temp) {
                vector[j] = vector[j - gap];
                j -= gap;
            }

            // Place temp in its correct location
            vector[j] = temp;
        }
    }
}
//  Using Hoare's Partition Algorithm
void Sorter::f_QuickSort(std::vector<short> &vector, const int &left,
                         const int &right, QuickPivotMode pivotMode) {

    if (left >= right) {
        return;
    }

    int pIndex =
        this->HELPER_quickSortPartition(vector, left, right, pivotMode);

    // non multi threading (crashes lol (I'm going to cry))
    // std::thread lowHalf(&Sorter::f_QuickSort, this, ref(vector), left,
    // pIndex,
    //                     pivotMode);
    //
    // std::thread highHalf(&Sorter::f_QuickSort, this, ref(vector), pIndex + 1,
    //                      right, pivotMode);
    //
    // lowHalf.join();
    // highHalf.join();

    // non multi threaded
    this->f_QuickSort(vector, left, pIndex, pivotMode);
    this->f_QuickSort(vector, pIndex + 1, right, pivotMode);
}
int Sorter::HELPER_quickSortPartition(std::vector<short> &vector,
                                      const int &left, const int &right,
                                      const QuickPivotMode &pivotMode) {
    int i = left - 1;
    int j = right + 1;

    int truePivot;
    using QM = Sorter::QuickPivotMode;
    switch (pivotMode) {
    case QM::LOWEST:
        truePivot = left;
        break;
    case QM::MEDIAN3: {
        int median = (right - left) / 2 + left;

        int max = this->HELPER_quickBigger(
            vector, left, this->HELPER_quickBigger(vector, median, right));

        if (max == left) {
            truePivot = this->HELPER_quickBigger(vector, median, right);
            break;
        }
        if (max == right) {
            truePivot = this->HELPER_quickBigger(vector, median, left);
            break;
        }

        truePivot = this->HELPER_quickBigger(vector, left, right);
        break;
    }
    case QM::MEDIAN5: {
        // Unsure if this is correct but at this point
        // FUCK YOU ZACH
        int median = (right - left) / 2 + left;
        int medianR = (right - median) / 2 + median;
        int medianL = (median - left) / 2 + left;

        std::vector<short> medianArray = {vector[left], vector[medianL],
                                          vector[median], vector[medianR],
                                          vector[right]};
        sort(medianArray.begin(), medianArray.end());

        if (medianArray[2] == vector[left]) {
            truePivot = left;
            break;
        }
        if (medianArray[2] == vector[medianL]) {
            truePivot = medianL;
            break;
        }
        if (medianArray[2] == vector[median]) {
            truePivot = median;
            break;
        }
        if (medianArray[2] == vector[medianR]) {
            truePivot = medianR;
            break;
        }
        if (medianArray[2] == vector[right]) {
            truePivot = right;
            break;
        }
        break;
    }
    case QM::RANDOM: {
        std::uniform_int_distribution<> distr(left, right);
        truePivot = distr(gen);
        // Non uniform but perhaps faster
        // truePivot = rand() % (right - left + 1) + left;
        break;
    }
    case QM::MEDIAN:
    default:
        truePivot = (right - left) / 2 + left;
        break;
    }
    // Works but isnt super pretty, makes random easier though
    std::swap(vector[truePivot], vector[left]);
    const int pivotIndex = left;
    while (true) {
        // Increment the left index until you hit something >= pivot
        do {
            i += 1;
        } while (vector[i] < vector[pivotIndex]);
        // Same but reverse for the right
        do {
            j -= 1;
        } while (vector[j] > vector[pivotIndex]);

        if (i >= j) {
            return j;
        }

        std::swap(vector[i], vector[j]);
    }
}
void Sorter::f_MergeSort(std::vector<short> &vector, const int &left,
                         const int &right) {

    if (left < right) {
        // Calculate the midpoint
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        this->f_MergeSort(vector, left, mid);
        this->f_MergeSort(vector, mid + 1, right);

        // Merge the sorted halves
        this->HELPER_merge(vector, left, mid, right);
    }
}

void Sorter::f_RadixSort(std::vector<short> &vector) {
    int maxNum = 0;
    for (size_t i = 0; i < vector.size(); i++) {
        if (vector[i] > maxNum) {
            maxNum = vector[i];
        }
    }

    std::vector<short> tempVector = vector;

    this->m_logger.warn("CURRENTLY USED GB AT START OF RADIX SORT: " +
                        std::to_string(getCurrentlyUsedGB()) + "gb");
    for (int digitPlace = 1; maxNum / digitPlace > 0; digitPlace *= 10) {
        size_t i;
        int countArray[10] = {0};

        for (i = 0; i < vector.size(); i++) {
            countArray[(vector[i] / digitPlace) % 10]++;
        }

        for (i = 1; i < 10; i++) {
            countArray[i] += countArray[i - 1];
        }

        // must be int otherwise if the vector is of size zero, a size_t
        // (long *unsigned* int) is set to -1; which then overflows to like 8
        // quintillion, and the program shits the bed.
        for (int i = vector.size() - 1; i >= 0; i--) {
            tempVector[countArray[(vector[i] / digitPlace) % 10] - 1] =
                vector[i];
            countArray[(vector[i] / digitPlace) % 10]--;
        }

        for (i = 0; i < vector.size(); i++) {
            vector[i] = tempVector[i];
        }
    }
}
void Sorter::f_HeapSort(std::vector<short> &vector) {
    this->HELPER_toHeap(vector);
    int end = vector.size() - 1;
    while (end > 0) {
        std::swap(vector[0], vector[end]);
        this->HELPER_shiftDown(vector, 0, end);
        --end;
    }
}
void Sorter::f_StandardSort(std::vector<short> &vector) {
    std::sort(vector.begin(), vector.end());
}
void Sorter::f_ShellCuiraSort(std::vector<short> &vector,
                              const int &size_index) {
    std::vector<int> gaps = this->HELPER_generateCiuraSequence(size_index);
    this->m_logger.log("size of gaps vector: " + std::to_string(gaps.size()));

    for (const int &gap : gaps) {
        for (size_t i = gap; i < vector.size(); i += 1) {
            auto temp = vector[i];
            int j = i;

            while (j >= gap && vector[j - gap] > temp) {
                vector[j] = vector[j - gap];
                j -= gap;
            }
            vector[j] = temp;
        }
    }
}
void Sorter::f_ShellKnuthSort(std::vector<short> &vector) {
    const int n = vector.size();
    int h = 1;

    // follow the knuth formula of h = 3h + 1
    while (h < n / 3) {
        h = 3 * h + 1;
    }

    while (h >= 1) {
        for (int i = h; i < n; i += 1) {
            int temp = vector[i];
            int j = i;

            while (j >= h && vector[j - h] > temp) {
                vector[j] = vector[j - h];
                j -= h;
            }
            vector[j] = temp;
        }

        h = (h - 1) / 3;
    }
}

// HELPER FUNCTIONS FOR SORTS
void Sorter::HELPER_merge(std::vector<short> &vector, const int &left,
                          const int &mid, const int &right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary vectors
    std::vector<int> leftVec(n1);
    std::vector<int> rightVec(n2);

    // Copy data to temporary vectors
    for (i = 0; i < n1; i++)
        leftVec[i] = vector[left + i];
    for (j = 0; j < n2; j++)
        rightVec[j] = vector[mid + 1 + j];

    // Merge the temporary vectors back into vec[left..right]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (leftVec[i] <= rightVec[j]) {
            vector[k] = leftVec[i];
            i++;
        } else {
            vector[k] = rightVec[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftVec[], if any
    while (i < n1) {
        vector[k++] = leftVec[i++];
    }

    // Copy the remaining elements of rightVec[], if any
    while (j < n2) {
        vector[k++] = rightVec[j++];
    }
}

void Sorter::HELPER_toHeap(std::vector<short> &vector) {
    int i = (vector.size() / 2) - 1;
    while (i >= 0) {
        this->HELPER_shiftDown(vector, i, vector.size());
        --i;
    }
}
void Sorter::HELPER_shiftDown(std::vector<short> &vector, int i,
                              const int max) {
    int i_big, c1, c2;
    while (i < max) {
        i_big = i;
        c1 = (2 * i) + 1;
        c2 = c1 + 1;
        if (c1 < max && vector[c1] > vector[i_big])
            i_big = c1;
        if (c2 < max && vector[c2] > vector[i_big])
            i_big = c2;
        if (i_big == i)
            return;
        std::swap(vector[i], vector[i_big]);
        i = i_big;
    }
}

std::vector<int> Sorter::HELPER_generateCiuraSequence(const int &size) {
    // by far best ones, found by brute force by Ciura
    std::vector<int> gaps = {1, 4, 10, 23, 57, 132, 301, 701};

    for (int i = gaps.size(); i < size; i += 1) {
        gaps.push_back(std::floor(gaps[i - 1] * 2.25));
    }

    std::reverse(gaps.begin(), gaps.end());
    return gaps;
}

int Sorter::HELPER_quickBigger(std::vector<short> &vector, const int &a,
                               const int &b) {
    if (vector[a] > vector[b])
        return a;
    return b;
}
