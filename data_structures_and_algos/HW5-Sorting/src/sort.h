#pragma once

#include "logger.h"
#include "time_handler.h"
#include <algorithm>
#include <future>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// SORTS
// - [x] insertion
// - [x] shell (shell sequence)
// - [x] quick
// - [x] merge
// - [x] radix
// - [x] heapsort
// - [x] std::sort
// - [x] modified quick sort w/ median of five (maybe)
// - [x] modified quick sort w/ random element
// - [x] modified quick sort w/ median of three w/ cut-off 4907?? (LOOK AT DOC)
// - [x] shell sort using cuira sequence (gaps: 1, 4, 10, 23, 57, 132, 301, 701)
// - [x] shell sort using knuths increments: h_i = 1/2(3^i - 1)

class Sorter {
  public:
    enum QuickPivotMode { LOWEST = 0, RANDOM, MEDIAN, MEDIAN3, MEDIAN5 };
    enum SortAlgorithm {
        // slowest 2 first because we want to get rid of them fast
        INSERTION = 0,
        SHELL,
        // now sorted by speed*
        SHELL_KNUTH,
        STANDARD,
        HEAPSORT,
        MERGE,
        QUICK,
        QUICK_RANDOMELEMENT,
        QUICK_MEDIANTHREE,
        QUICK_MEDIANFIVE,
        SHELL_CUIRA,
        // This is so the list can be reordered without worrying about other
        // places in the program
        LAST_ENUM,
        RADIX,
    };

    Sorter() {
        srand(time(nullptr));
        this->m_logger.setName("SORTER");
    }; // seed every time we run the program

    // deals with entire sort lifetime, including timer the function, and
    // logging of the times
    void startSort(std::vector<short> vector, const SortAlgorithm &algorithm,
                   const int &size, std::promise<bool> promise,
                   const int &size_index);

    // the actual time logger
    TimeHandler timeHandler() { return this->m_handler; }

  private:
    // manages all of the times; has to be static for threading
    static TimeHandler m_handler;

    static Logger m_logger;

    // calls the correct function for the sort
    void f_handleSort(std::vector<short> &vector,
                      const SortAlgorithm &algorithm, const int &size_index);

    // ALL SORTS BELOW HERE
    void f_InsertionSort(std::vector<short> &vector);
    void f_ShellSort(std::vector<short> &vector);
    void f_QuickSort(std::vector<short> &vector, const int &left,
                     const int &right,
                     QuickPivotMode pivotMode = QuickPivotMode::MEDIAN);
    void f_MergeSort(std::vector<short> &vector, const int &left,
                     const int &right);
    void f_RadixSort(std::vector<short> &vector);
    void f_HeapSort(std::vector<short> &vector);
    void f_StandardSort(std::vector<short> &vector);
    void f_ShellCuiraSort(std::vector<short> &vector, const int &size_index);
    void f_ShellKnuthSort(std::vector<short> &vector);

    // HELPER FUNCTIONS FOR SORTS
    void HELPER_merge(std::vector<short> &vector, const int &left,
                      const int &mid, const int &right);
    void HELPER_toHeap(std::vector<short> &vector);
    void HELPER_shiftDown(std::vector<short> &vector, int i, const int max);
    std::vector<int> HELPER_generateCiuraSequence(const int &size);
    int HELPER_quickSortPartition(std::vector<short> &vector, const int &left,
                                  const int &right,
                                  const QuickPivotMode &pivotMode);
    int HELPER_quickBigger(std::vector<short> &vector, const int &a,
                           const int &b);
};
