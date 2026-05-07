#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#if (defined(_MSVC_LANG) && _MSVC_LANG < 201703L) ||                           \
    (!defined(_MSVC_LANG) && __cplusplus < 201703L)

string promptFile(const bool &writing) {

    while (true) {
        const string DEFAULT_FILE_NAME = "route_5.txt";
        cout << "Enter file (default " << DEFAULT_FILE_NAME << "): ";

        string input;
        getline(cin, input);

        // empty check
        if (input.empty()) {
            return DEFAULT_FILE_NAME;
        }
        return input;
    }
}

#endif
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) ||                          \
    (!defined(_MSVC_LANG) && __cplusplus >= 201703L)

#include <filesystem>

// namespace fs = std::filesystem;
namespace fs = filesystem;

void Clear() {
    cout << "\n\n\n";
    /*
#if defined _WIN32
    system("cls");
    // clrscr(); // including header file : conio.h
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    cout << u8"\033[2J\033[1;1H"; // Using ANSI Escape Sequences
#elif defined(__APPLE__)
    system("clear");
#endif
*/
}
bool fileExists(const fs::path &fileName) {
    ifstream f(fileName.c_str());
    return f.good();
}

void listFiles(const fs::path &path) {

    cout << path << ": " << '\n';
    vector<fs::path> files;
    vector<fs::path> dirs{"."};
    for (const auto &entry : fs::directory_iterator(path)) {
        if (entry.is_directory()) {
            string dirName = entry.path().filename().string() + '/';
            dirs.emplace_back(dirName);
            continue;
        }
        files.emplace_back(entry.path().filename());
    }

    if (path.has_parent_path())
        dirs.insert(dirs.begin() + 1, "..");

    sort(files.begin(), files.end());
    sort(dirs.begin(), dirs.end());

    // 2 spaces
    const string FILE_GAP = "  ";
    string str = "";

    for (fs::path &dir : dirs)
        str += dir.string() + FILE_GAP;
    for (fs::path &file : files)
        str += file.string() + FILE_GAP;

    cout << str << "\n\n\n";
}

bool enterFileName(const fs::path &basePath, fs::path &path,
                   const bool &writing) {
    while (true) {
        cout << "Enter file name ([quit], default route_5.txt): ";
        string input = "";
        getline(cin, input);

        if (input == "quit")
            return false;

        if (input.empty()) 
            input = "route_5.txt";

        // if writing, ignore the case that the file doesn't exist
        if (writing) {
            path = basePath / fs::path(input);
            return true;
        }

        if (fileExists(basePath / fs::path(input))) {
            path = basePath / fs::path(input);
            return true;
        } else {
            cout << "File not found! Try again." << '\n';
            continue;
        }
    }
}

bool navigate(fs::path &basePath) {

    listFiles(basePath);
    while (true) {

        cout << "[L]ist dirs & files, [B]ack a dir, [C]hoose dir, [Q]uit "
                "to enter file: ";
        string input = "";
        getline(cin, input);

        char c = input[0];
        c = tolower(c);
        switch (c) {
        case 'q': {
            Clear();
            return false;
        }

        case 'l': {
            Clear();
            listFiles(basePath);
            break;
        }
        case 'b': {
            Clear();
            while (true) {
                if (basePath.has_parent_path()) {
                    basePath = basePath.parent_path();
                    listFiles(basePath);
                    break;
                } else {
                    cout << "No parent path found!" << '\n';
                    continue;
                }
            }
            break;
        }
        case 'c': {
            while (true) {
                cout << "Enter Directory Name [quit]: ";
                string dir = "";
                getline(cin, dir);

                if (dir == "quit") {
                    Clear();
                    return false;
                }

                fs::path dir_path = basePath / dir;
                if (!fs::is_directory(dir_path)) {
                    cout << "Not a directory / directory not found!" << '\n';
                    continue;
                }

                Clear();
                basePath = dir_path;
                listFiles(basePath);
                break;
            }
            continue;
            break;
        }
        }
    }
    return false;
}

string promptFile(const bool &writing) {
    Clear();
    fs::path basePath = fs::current_path();
    // list current files once

    listFiles(basePath);
    while (true) {
        // prompting
        cout << "Enter option: [L]ist files, [N]avigate, [E]nter file "
                "name, [Q]uit: ";

        string input = "";
        getline(cin, input);

        char c = input[0];
        // handle other cases
        switch (c) {
        case 'l': {
            listFiles(basePath);
            break;
        }
        case 'n': {
            Clear();
            while (true) {
                fs::path path = "";
                if (navigate(basePath)) {
                    continue;
                } else {
                    listFiles(basePath);
                    break;
                }
            }
            break;
        }
        case 'e': {
            while (true) {
                fs::path path = "";
                if (enterFileName(basePath, path, writing)) {
                    return path.string();
                } else {
                    break;
                }
            }
            break;
        }
        case 'q': {
            return "";
            break;
        }
        default: {
            cout << "Unknown option: " << c << '\n';
            continue;
        }
        }
    }
}
#endif
