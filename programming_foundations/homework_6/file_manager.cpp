#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) ||                          \
    (!defined(_MSVC_LANG) && __cplusplus >= 201703L)

#include "file_manager.h"
#include "global.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

FileManager::FileManager() {};

FileManager::Layout FileManager::setLayout(const FileManager::Layout &x) {
    this->m_layout = x;
    return x;
}

bool FileManager::setUseSlashesAfterDir(const bool &x) {
    this->m_useSlashesAfterDir = x;
    return x;
}

int FileManager::setItemsPerRow(const int &x) {
    this->m_itemsPerRow = x;
    return x;
}

bool FileManager::setListDirectoriesFirst(const bool &x) {
    this->m_listDirectoriesFirst = x;
    return x;
}

fs::path FileManager::setCWD(const fs::path &x) {
    this->m_cwd = x;
    return x;
}

string FileManager::setErrorMessage(const string &x) {
    this->m_errorMessage = x;
    return x;
}

string FileManager::setTodoMessage(const string &x) {
    this->m_todoMessage = x;
    return x;
}

void FileManager::listFiles() { this->listFiles(this->m_layout, this->m_cwd); }
void FileManager::listFiles(const FileManager::Layout &layout) {
    this->listFiles(layout, this->m_cwd);
}

void FileManager::listFiles(const fs::path &path) {
    this->listFiles(this->layout(), path);
}

fs::path FileManager::getFilePath(const bool &writing) {
    return this->getFilePath(this->cwd(), writing);
}

void FileManager::printErrorMessage() {
    cout << Global::Colors::Error << this->errorMessage()
         << Global::Colors::Reset << '\n';
}

void FileManager::printTodoMessage() {
    cout << Global::Colors::Directory << this->todoMessage()
         << Global::Colors::Reset << '\n';
}

void FileManager::write(const string &contents) {
    this->setTodoMessage("Write File");
    const bool writing = true;
    fs::path path = getFilePath(writing);
    this->write(contents, path);
}

void FileManager::write(const string &contents, const fs::path &path) {
    ofstream file(path);
    file << contents;
    file.close();
    cout << "File written to: " << path << '\n';
}

string FileManager::read() {
    fs::path path = this->getFilePath();
    return this->read(path);
}
string FileManager::read(const fs::path &path) {
    this->setTodoMessage("Open File");
    // streams
    ifstream fileIn(path, ios::in);
    ostringstream os;

    // reading
    os << fileIn.rdbuf();
    fileIn.close();

    return os.str();
}

void FileManager::askSaveFile(const string &contents) {
    while (true) {
        cout << "Do you want to save this to a file? y/n\n>";
        string input = "";
        getline(cin, input);
        char c = tolower(input[0]);
        if (c == 'y') {
            this->write(contents);
            break;
        } else if (c == 'n' || c == 'q') {
            break;
        } else {
            cout << "Unknown option: " << input << '\n';
        }
    }
}

fs::path FileManager::getFilePath(const fs::path &startingPath,
                                  const bool &writing) {
    while (true) {
        this->clearScreen();
        this->printTodoMessage();
        cout << this->cwd().string() << '/' << ':' << '\n';
        this->listFiles();
        this->printErrorMessage();
        // reset teh error message after performing it
        this->setErrorMessage("");
        cout << "[Q]uit, [..] to go back, [cd <directory>] to enter directory, "
                "or type the "
                "filename"
             << '\n'
             << "> ";
        string input = "";
        getline(cin, input);

        if (tolower(input[0]) == 'q') {
            return "";
        }

        if (input.empty()) {
            continue;
        }

        // back macro
        if (input.substr(0, 2) == "..") {
            int backCount = count(input.begin(), input.end(), '.') - 1;
            for (int i = 0; i < backCount; i += 1) {
                this->setCWD(cwd().parent_path());
            }
            continue;
        }

        // cd into a directory
        if (input.substr(0, 3) == "cd ") {
            const string dirName = input.substr(3);
            const fs::path potential = this->cwd() / dirName;
            if (!fs::exists(potential)) {
                this->setErrorMessage("Directory " + dirName + " not found!");
                continue;
            }

            // directory exists :)
            if (fs::is_directory(potential)) {
                this->setCWD(potential);
            } else {
                this->setErrorMessage(dirName + " is not a directory!");
            }
            continue;
        }

        // inputting a file now
        const fs::path potentialFile = this->cwd() / input;

        // file / dir not found
        if (!fs::exists(potentialFile) && !writing) {
            this->setErrorMessage("File " + input + " not found!");
            continue;
        }

        // file exists, just is a directory
        if (fs::is_directory(potentialFile)) {
            // yes it's called potentialFile, but it is now a directory

            // input is the original input; if the input is just '.' (the
            // current dir) then ignore it
            if (input != ".") {
                this->setCWD(potentialFile);
            }
            continue;
        }

        // file exists :)
        return potentialFile.string();
    }
}

void FileManager::listFiles(const FileManager::Layout &layout,
                            const fs::path &path) {
    vector<fs::path> files{};
    vector<fs::path> dirs{};
    size_t longestFileName = 0;

    for (const auto &entry : fs::directory_iterator(path)) {

        if (fs::is_directory(entry)) {
            dirs.push_back(entry);
        } else {
            files.push_back(entry);
        }

        const size_t fileNameSize = entry.path().filename().string().size();
        if (fileNameSize > longestFileName) {
            longestFileName = fileNameSize;
        }
    }

    if (path.has_parent_path()) {
        dirs.push_back("..");
    }

    vector<fs::path> entries;
    entries.reserve(files.size() + dirs.size());

    // if dirs first, append all of files to dirs and use that
    if (this->listDirectoriesFirst()) {
        sort(dirs.begin(), dirs.end());
        sort(files.begin(), files.end());
        entries.insert(entries.end(), dirs.begin(), dirs.end());
        entries.insert(entries.end(), files.begin(), files.end());
    } else {
        // if not dirs first, then just combine them and then sort
        entries.insert(entries.end(), dirs.begin(), dirs.end());
        entries.insert(entries.end(), files.begin(), files.end());
        sort(entries.begin(), entries.end());
    }

    const string FILE_GAP = "  ";

    if (layout == FileManager::Layout::Horizontal) {
        string row;
        for (const fs::path &file : entries) {
            if (fs::is_directory(file)) {
                row += Global::Colors::Directory + file.filename().string();
                if (this->useSlashesAfterDir()) {
                    row += '/';
                }
                row += Global::Colors::Reset;
            } else if (file.extension() == ".txt") {
                row += Global::Colors::TextFile + file.filename().string() +
                       Global::Colors::Reset;
            } else {
                row += file.filename().string();
            }
            row += FILE_GAP;
        }
        cout << row << '\n';
        return;
    }

    for (size_t i = 0; i < entries.size(); i += this->itemsPerRow()) {
        vector<fs::path> rowItems;

        // not enough files left to complete a row
        if (i > (entries.size() - this->itemsPerRow())) {
            for (size_t j = i; j < entries.size(); j += 1) {
                rowItems.push_back(entries[j]);
            }

        } else {
            // there is enough to complete the row, just add the row
            for (int j = 0; j < this->itemsPerRow(); j += 1) {
                rowItems.push_back(entries[i + j]);
            }
        }

        // print the row
        ostringstream stream;
        for (size_t j = 0; j < rowItems.size(); j += 1) {
            string fileName = rowItems[j].filename().string();

            if (fs::is_directory(rowItems[j]) && this->useSlashesAfterDir()) {
                fileName += '/';
            }

            stream << left << setw(longestFileName + FILE_GAP.size())
                   << fileName + FILE_GAP;
        }
        cout << stream.str() << '\n';
    }
}

void FileManager::clearScreen() {
#if defined _WIN32
    system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    cout << u8"\033[2J\033[1;1H"; // Using ANSI Escape Sequences
#elif defined(__APPLE__)
    system("clear");
#endif
}
#endif
