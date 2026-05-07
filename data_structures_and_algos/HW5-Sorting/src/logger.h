#pragma once

#include <chrono>
#include <ctime>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>

namespace Loggers {
const std::string Reset = "\033[0m";
const std::string Log = "\e[4;37m";       // white + underline
const std::string Warn = "\e[4;33m";      // yellow + underline
const std::string Error = "\033[4;1;31m"; // red + bold + underline
} // namespace Loggers

class Logger {
  public:
    Logger(const std::string &name = "default_name") : m_name(name) {
        time_t tt;
        struct tm *ti;
        time(&tt);
        ti = localtime(&tt);
        std::ofstream outfile;
        outfile.open(this->filename, std::ios_base::app);
        outfile << "\nNEW ATTEMPT @ " << asctime(ti) << "\n";
        outfile.close();
    }

    void log(const std::string msg);
    void warn(const std::string msg);
    void error(const std::string msg);

    void setName(const std::string name) { this->m_name = name; }

  private:
    std::string m_name;
    const std::string filename = "log.txt";

    enum LogLevel {
        Log = 0,
        Warn,
        Error,
    };

    void f_appendToFile(const LogLevel &l, const std::string msg);
};

// implementation details only below here

inline void Logger::log(const std::string msg) {
    // std::cout << Loggers::Log << "[" << this->m_name << "] " << msg
    //           << Loggers::Reset << '\n';
    this->f_appendToFile(Logger::LogLevel::Log,
                         '[' + this->m_name + "]: " + msg);
}

inline void Logger::warn(const std::string msg) {
    std::cout << Loggers::Warn << "[" << this->m_name << "] " << msg
              << Loggers::Reset << '\n';
    this->f_appendToFile(Logger::LogLevel::Warn,
                         '[' + this->m_name + "]: " + msg);
}

inline void Logger::error(const std::string msg) {
    std::cout << Loggers::Error << "[" << this->m_name << "] " << msg
              << Loggers::Reset << '\n';
    this->f_appendToFile(Logger::LogLevel::Error,
                         '[' + this->m_name + "]: " + msg);
}

inline void Logger::f_appendToFile(const LogLevel &l, const std::string msg) {
    std::ofstream outfile;
    outfile.open(this->filename, std::ios_base::app);
    std::string guh;
    switch (l) {
    case Logger::LogLevel::Log:
        guh = "[LOG]: ";
        break;
    case Logger::LogLevel::Warn:
        guh = "[WARN]: ";
        break;
    case Logger::LogLevel::Error:
        guh = "[ERROR]: ";
        break;
    }

    outfile << guh << msg << '\n';
    outfile.close();
}
