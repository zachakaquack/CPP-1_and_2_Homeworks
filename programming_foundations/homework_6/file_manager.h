#pragma once


#include <string>
using namespace std;
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) ||                           \
    (!defined(_MSVC_LANG) && __cplusplus >= 201703L)
#include <filesystem>
namespace fs = filesystem;


class FileManager {

  public:
    enum Layout {
        Vertical = 0,
        Horizontal = 1,
    };

    FileManager();

    void clearScreen();
    fs::path getFilePath(const bool &writing = false);
    fs::path getFilePath(const fs::path &startingPath,
                         const bool &writing = false);

    void listFiles();
    void listFiles(const Layout &layout);
    void listFiles(const fs::path &path);
    void listFiles(const Layout &layout, const fs::path &path);

    void write(const string &contents);
    void write(const string &contents, const fs::path &path);

    string read();
    string read(const fs::path &path);

    void askSaveFile(const string &contents);

    fs::path cwd() { return this->m_cwd; }
    fs::path setCWD(const fs::path &x);
    Layout layout() { return this->m_layout; }
    Layout setLayout(const Layout &x);
    bool useSlashesAfterDir() { return this->m_useSlashesAfterDir; }
    bool setUseSlashesAfterDir(const bool &x);
    bool listDirectoriesFirst() { return this->m_listDirectoriesFirst; }
    bool setListDirectoriesFirst(const bool &x);
    int itemsPerRow() { return this->m_itemsPerRow; }
    int setItemsPerRow(const int &x);
    string errorMessage() { return this->m_errorMessage; }
    string setErrorMessage(const string &x);
    string todoMessage() { return this->m_todoMessage; }
    string setTodoMessage(const string &x);

  private:
    fs::path m_cwd = fs::current_path();
    Layout m_layout = Layout::Horizontal;
    bool m_useSlashesAfterDir = true;
    bool m_listDirectoriesFirst = true;
    size_t m_itemsPerRow = 2;
    string m_errorMessage = "";
    string m_todoMessage = "";

    void prompt();
    void printErrorMessage();
    void printTodoMessage();
};
#endif