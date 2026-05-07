#pragma once

#include <iostream>

using namespace std;
#if (defined(_MSVC_LANG) && _MSVC_LANG < 201703L) ||                           \
    (!defined(_MSVC_LANG) && __cplusplus < 201703L)
string promptFile(const bool &writing);
#endif
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) ||                          \
    (!defined(_MSVC_LANG) && __cplusplus >= 201703L)
#include <filesystem>

// namespace fs = std::filesystem;
namespace fs = filesystem;

void Clear();
void listFiles(const fs::path &path);
bool enterFileName(const fs::path &basePath, fs::path &path,
                   const bool &writing);
bool navigate(fs::path &basePath);
string promptFile(const bool &writing);
#endif
