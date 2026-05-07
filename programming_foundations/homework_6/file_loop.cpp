#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) ||                          \
    (!defined(_MSVC_LANG) && __cplusplus >= 201703L)

#include "chain.h"
#include "events.h"
#include "file_manager.h"
#include "parser.h"
#include <filesystem>

using namespace std;
namespace fs = filesystem;

void FileLoop::mainLoop() {
    // prompt for file
    FileManager manager;
    fs::path path = manager.getFilePath();
    if (path.empty()) {
        return;
    }

    // path is good!
    manager.clearScreen();

    string contents = manager.read(path);
    vector<string> lines = splitString(contents, '\n');

    Chain chain{lines};
    chain.print();

    manager.askSaveFile(contents);
}
#endif
