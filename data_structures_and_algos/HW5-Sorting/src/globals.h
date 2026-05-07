#pragma once

#include "sort.h"
#include "timer.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using SA = Sorter::SortAlgorithm;

// the index to use in the loop, goes from 0 to the number in the name of
// the variable (inclusive)
enum VALUES {
    HUNDRED_THOUSAND = 2,
    FIVEHUNDRED_THOSUAND = 10,
    MILLION = 20,
    TEN_MILLION = 41,
    HUNDRED_MILLION = 50,
    BILLION = 59
};

namespace Colors {
const std::string Reset = "\033[0m";
const std::string Blue = "\033[94m";
const std::string Error = "\033[31m";
const std::string Success = "\033[32m";
const std::string Bold = "\033[1m";
} // namespace Colors

// clang-format off
inline std::unordered_map<SA, std::string> algorithmToName = {
    {SA::INSERTION, "Insertion Sort"},
    {SA::SHELL, "Shell Sort"},
    {SA::QUICK, "Quick Sort"},
    {SA::MERGE, "Merge Sort"},
    {SA::RADIX, "Radix Sort"},
    {SA::HEAPSORT, "Heap Sort"},
    {SA::STANDARD, "Standard Sort"},
    {SA::QUICK_MEDIANFIVE, "Quick Median of 5 Sort"},
    {SA::QUICK_RANDOMELEMENT, "Quick Random Element Sort"},
    {SA::QUICK_MEDIANTHREE, "Quick Median of 3 Sort"},
    {SA::SHELL_CUIRA, "Shell Cuira Sort"},
    {SA::SHELL_KNUTH, "Shell Knuth Sort"}
};

inline const std::vector<int> sizes = {
    // 50k - 1M, jump 50k
    50'000, 100'000, 150'000, 200'000, 250'000,
	300'000, 350'000, 400'000,450'000, 500'000,
	550'000, 600'000, 650'000, 700'000, 750'000,
	800'000,850'000, 900'000, 950'000, 1'000'000,
    // 1M - 2M, jump 100k
    1'100'000, 1'200'000, 1'300'000, 1'400'000, 1'500'000,
	1'600'000,1'700'000, 1'800'000, 1'900'000, 2'000'000,
    // 2M - 5M, jump 500k
    2'500'000, 3'000'000, 3'500'000, 4'000'000, 4'500'000, 5'000'000,
    // 5M - 10M, jump 1M
    6'000'000, 7'000'000, 8'000'000, 9'000'000, 10'000'000,
    // 10M - 100M, jump 10M
    20'000'000, 30'000'000, 40'000'000, 50'000'000,
	60'000'000, 70'000'000,80'000'000, 90'000'000, 100'000'000,
    // 100M - 1B, jump 100M
    200'000'000, 300'000'000, 400'000'000, 500'000'000,
	600'000'000, 700'000'000, 800'000'000, 900'000'000, 1'000'000'000
};
// clang-format on

// prints the line according to the width
inline void printDottedLine(const std::string &left, const std::string &right,
                            const int &width = 66,
                            const bool addSpaces = true) {
    std::stringstream ss;
    const int dotCount = width - left.length() - right.length();

    if (addSpaces) {
        ss << left << ' ' << std::string(dotCount - 3, '.') << ' ' << right;
    } else {
        ss << left << std::string(dotCount, '.') << right;
    }

    std::cout << ss.str() << '\n';
}

inline std::vector<short> createRandomizedVector(const size_t &size) {
    std::vector<short> vector;
    vector.reserve(size);

    // yes, this is apparently the "modern" way of generating a random number
    // within a certain range
    // https://stackoverflow.com/a/7560564

    // obtain a random number from hardware
    std::random_device rd;

    // seed the generator
    std::mt19937 gen(rd());

    // define the range
    std::uniform_int_distribution<> distr(0, 10'000);

    for (size_t i = 0; i < size; i += 1) {
        // generate the number
        vector.push_back(distr(gen));
    }
    return vector;
}

// format with some weird std library thing. found on SO somewhere
inline std::string formatWithCommas(const double &value) {
    std::stringstream ss;
    ss.imbue(std::locale(""));
    ss << std::setprecision(0) << std::fixed << value;
    return ss.str();
}

// print all the times from the time handler
inline void printAllTimes(TimeHandler &handler) {

    for (auto iterator : handler.dict()) {
        const std::string key = iterator.first;
        const std::vector<double> vector = iterator.second;

        std::cout << "\n\n\n" << key << '\n';
        for (size_t i = 0; i < vector.size(); i += 1) {
            std::string seconds;
            if (vector[i] < 0.000001) {
                seconds = "N/A";
            } else {
                seconds = std::to_string(vector[i]) + " seconds";
            }
            const std::string size = formatWithCommas(sizes[i]);
            const int dotCount = 40 - size.length() - seconds.length();
            std::cout << size << ' ' << std::string(dotCount, '.') << ' '
                      << seconds << '\n';
        }
    }
}

// save the data in csv file so we can easily import it into any excel like
// program fuck you grant. fuck you. i spent an hour writing this. instead of
// spending 10 seconds just easily importing it myself. #worth
inline void
writeCSVDataToFileFromHandler(TimeHandler &handler,
                              const std::string &filepath = "results.csv") {

    // first row; just all the names
    std::string names = "Vector Size,";

    // all of the actual data values
    std::vector<std::string> csvLines;

    auto d = handler.dict();

    // fill up the csvLines vector with the right amount of empty strings
    for (auto iterator : d) {
        for (size_t i = 0; i < iterator.second.size(); i += 1) {
            std::string s{};
            csvLines.push_back(s);
        }

        // fill up the first column with the vector sizes
        for (size_t i = 0; i < csvLines.size(); i += 1) {
            csvLines[i] = std::to_string(sizes[i]) + ',';
        }
        break;
    }

    // actually add to the vector of strings
    for (auto iterator : d) {
        std::string str = iterator.first;
        std::vector<double> vector = iterator.second;

        names += str + ',';
        for (size_t i = 0; i < vector.size(); i += 1) {
            csvLines[i] += std::to_string(vector[i]) + ',';
        }
    }

    // format properly now
    std::stringstream ss;
    ss << names << '\n';
    for (size_t i = 0; i < csvLines.size(); i += 1) {
        ss << csvLines[i] << '\n';
    }

    // write
    std::ofstream out(filepath);
    out << ss.str();
    out.close();
}

inline void printCenter(const std::string string) {
    const int width = 50;
    const std::string spaces =
        std::string(int((width - string.length()) / 2), ' ');
    std::cout << (spaces + string + spaces) << '\n';
}

inline void shuffleVector(std::vector<int> &vector) {
    std::random_device rd;
    std::mt19937 gen(rd());
    shuffle(vector.begin(), vector.end(), gen);
}

// just prompt for an int smile
// returns nullptr if user wants to quit
// otherwise returns a pointer to the result
inline int *promptForGB() {
    while (true) {
        std::cout << "How many GB of ram do you want to allocate? (default: "
                     "6gb; enter [q] to quit, [h] for help)\n> ";

        std::string value;
        std::getline(std::cin, value);

        // convert answer to all lowercase
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        // check if want to quit
        if (value == "quit" || value == "q") {
            return nullptr;
        } else if (value == "h") {
            continue;
        } else if (value == "") {
            return new int{6}; // default value
        }
        try {
            return new int{std::stoi(value)};
        } catch (const std::invalid_argument &ex) {
            std::cout << "invalid answer! try again." << '\n';
        } catch (const std::out_of_range &ex) {
            std::cout << "out of range! try again." << '\n';
        }
    }
}

// stolen and slightly tweaked from https://stackoverflow.com/a/64166
inline int parseLineForUsedGB(char *line) {
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char *p = line;
    while (*p < '0' || *p > '9')
        p++;
    line[i - 3] = '\0';
    i = atoi(p);
    return i;
}

inline float getCurrentlyUsedGB() { // Note: this value is in KB!
    FILE *file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL) {
        if (strncmp(line, "VmSize:", 7) == 0) {
            result = parseLineForUsedGB(line);
            break;
        }
    }
    fclose(file);

    const double gigabyte_factor = 1'000'000.0;
    return result / gigabyte_factor;
}

inline void printHelpThingyYupYupYupYupYup() {
    // yes, this is constructing the string each time. fuick you
    std::string str =
        "Each short is 2 bytes, which means a vector of 1 billion "
        "shorts is 2 billion bytes; which then translates to 2gb. ";
    str += "This program watches its memory usage to figure out how "
           "many threads it can run at once.\n";
    str += "Some sorting algorithms have a copy of their vector, thus "
           "doubling the size; so the minimum gb for this program is "
           "4gb (if you want to run it to completion.)\n";
    str += "However, 6gb is recommended for general memory leakage "
           "over time and safety.\n";
    str += "The more memory you give, the faster the program will run. "
           "On my (zach) desktop, the program ran 50k - 1bil in just "
           "under four hours w/ 8gb given.";
    std::cout << str << '\n';
}
