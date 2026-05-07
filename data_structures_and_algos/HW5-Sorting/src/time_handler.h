#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
class TimeHandler {
    using map = std::unordered_map<std::string, std::vector<double>>;

  public:
    map dict() { return this->m_dict; }

    // gets from the map
    std::vector<double> getAllTimesFor(const std::string &name) {
        return this->dict()[name];
    };
    double getTimeFor(const std::string &name, const size_t &index) {
        return this->dict()[name][index];
    };

    // inserts into the map
    void insertTime(const std::string &name, const double &seconds) {
        this->m_dict[name].push_back(seconds);
    };

  private:
    // "algorigthm_sort_name": vector<double>,
    // vector is the time taken for each algo, following the list in globals.h
    map m_dict;
};
