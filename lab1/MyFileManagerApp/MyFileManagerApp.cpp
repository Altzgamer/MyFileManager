#ifdef _WIN32
    #include <direct.h>
    #define getcwd _getcwd
#else
    #include <unistd.h>
#endif

#include <cstdlib>
#include <iostream>
#include <string>
#include "../MyFileManagerLib/MyFileManager.h" 
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <limits>

using std::cout;
using std::cin;
using std::endl;
using std::string;

string InputPathString(const string& prompt) {
    string input;
    cout << prompt;
    if (cin.peek() == '\n') cin.ignore();
    getline(cin, input);

    if (input.empty()) throw std::invalid_argument("Input cannot be empty.");

    return input;
}

namespace MainMenu {
    enum class MenuCommand {
        none,
        join,
        absolute,
        relative,
        relativize,
        exit,
        change_strings_type
    };


    MenuCommand ParseCommand() {
        std::unordered_map<string, MenuCommand> commandMap = {
            {"join", MenuCommand::join},
            {"absolute", MenuCommand::absolute},
            {"relative", MenuCommand::relative},
            {"relativize", MenuCommand::relativize},
            {"exit", MenuCommand::exit},
            {"g", MenuCommand::change_strings_type},
            {"a", MenuCommand::join},
            {"b", MenuCommand::absolute},
            {"c", MenuCommand::relative},
            {"d", MenuCommand::relativize},
            {"f", MenuCommand::exit}
        };

        string inpStr = InputPathString("Enter command: ");
        cout.flush();
        std::transform(inpStr.begin(), inpStr.end(), inpStr.begin(), [](char c) { return std::tolower(c); });

        auto it = commandMap.find(inpStr);
        if (it != commandMap.end()) return it->second;
        else return MenuCommand::none;
    }

    void PrintMenu() {
        cout << " _______________________" << endl;
        cout << "|   What to execute?    |" << endl;
        cout << "|Enter command or letter|" << endl;
        cout << "|      a) join          |" << endl;
        cout << "|      b) absolute      |" << endl;
        cout << "|      c) relative      |" << endl;
        cout << "|      d) relativize    |" << endl;
        cout << "|      f) exit          |" << endl;
        cout << "|                       |" << endl;
        cout << "| g) change strings type|" << endl;
        cout << "|_______________________|" << endl << endl;

        char* cwd = getcwd(NULL, 0);
        cout << "Cur. working directory: " << cwd << endl;
        free(cwd);
    }
}

using namespace MainMenu;

void handleJoin(bool useCStrings) {
    string base_path, relative_path, result_path;

    try {
        base_path = InputPathString("Enter base path: ");
        relative_path = InputPathString("Enter relative path: ");
    }
    catch (const std::invalid_argument& error) {
        cout << "Error: " << error.what() << endl;
        return;
    }

    if (useCStrings) {
        try {
            char* result_path_c = join(base_path.c_str(), relative_path.c_str());
            cout << result_path_c << endl;
            delete[] result_path_c;
        }
        catch (const std::invalid_argument& error) {
            cout << "Error: " << error.what() << std::endl;
        }
        catch (...) {
            cout << "Unexpected error" << endl;
        }
    }
    else {
        try {
            result_path = join(base_path, relative_path);
            cout << result_path << endl;
        }
        catch (const std::invalid_argument& error) {
            cout << "Error: " << error.what() << std::endl;
        }
        catch (...) {
            cout << "Unexpected error" << endl;
        }
    }
}

void handleAbsolute(bool useCStrings) {
    string base_path, result_path;

    try {
        base_path = InputPathString("Enter relative to CWD path to make absolute: ");
    }
    catch (const std::invalid_argument& error) {
        cout << "Error: " << error.what() << endl;
        return;
    }

    if (useCStrings) {
        try {
            char* result_path_c = absolute(base_path.c_str());
            cout << result_path_c << endl;
            delete[] result_path_c;
        }
        catch (const std::invalid_argument& error) {
            cout << "Error: " << error.what() << std::endl;
        }
        catch (...) {
            cout << "Unexpected error" << endl;
        }
    }
    else {
        try {
            result_path = absolute(base_path);
            cout << result_path << endl;
        }
        catch (const std::invalid_argument& error) {
            cout << "Error: " << error.what() << std::endl;
        }
        catch (...) {
            cout << "Unexpected error" << endl;
        }
    }
}

void handleRelative(bool useCStrings) {
    string base_path, result_path;

    try {
        base_path = InputPathString("Enter absolute path to make relative: ");
    }
    catch (const std::invalid_argument& error) {
        cout << "Error: " << error.what() << endl;
        return;
    }

    if (useCStrings) {
        try {
            char* result_path_c = relative(base_path.c_str());
            cout << result_path_c << endl;
            delete[] result_path_c;
        }
        catch (const std::invalid_argument& error) {
            cout << "Error: " << error.what() << std::endl;
        }
        catch (...) {
            cout << "Unexpected error" << endl;
        }
    }
    else {
        try {
            result_path = relative(base_path);
            cout << result_path << endl;
        }
        catch (const std::invalid_argument& error) {
            cout << "Error: " << error.what() << std::endl;
        }
        catch (...) {
            cout << "Unexpected error" << endl;
        }
    }
}

void handleRelativize(bool useCStrings) {
    string base_path, relative_path, result_path;

    try {
        base_path = InputPathString("Enter 1st abs path: ");
        relative_path = InputPathString("Enter 2nd abs path: ");
    }
    catch (const std::invalid_argument& error) {
        cout << "Error: " << error.what() << endl;
        return;
    }

    if (useCStrings) {
        try {
            char* result_path_c = relativize(base_path.c_str(), relative_path.c_str());
            cout << result_path_c << endl;
            delete[] result_path_c;
        }
        catch (const std::invalid_argument& error) {
            cout << "Error: " << error.what() << std::endl;
        }
        catch (...) {
            cout << "Unexpected error" << endl;
        }
    }
    else {
        try {
            result_path = relativize(base_path, relative_path);
            cout << result_path << endl;
        }
        catch (const std::invalid_argument& error) {
            cout << "Error: " << error.what() << std::endl;
        }
        catch (...) {
            cout << "Unexpected error" << endl;
        }
    }
}

void handleExit(bool& isPrintingMenu) {
    isPrintingMenu = false;
}

void handleChangeStringsType(bool useCStrings) {
    useCStrings = !useCStrings;
}

int main() {
    MenuCommand CurCommand;
    bool IsPrintingMenu = true;
    bool UseCStrings = true;

    std::unordered_map<MenuCommand, std::function<void(bool&)>> commandMap = {
        {MenuCommand::join, handleJoin},
        {MenuCommand::absolute, handleAbsolute},
        {MenuCommand::relative, handleRelative},
        {MenuCommand::relativize, handleRelativize},
        {MenuCommand::exit, handleExit},
        {MenuCommand::change_strings_type, handleChangeStringsType}
    };

    while (IsPrintingMenu) {
        PrintMenu();

        cout << "CStrings used: ";
        cout << (UseCStrings ? "True" : "False") << endl;

        CurCommand = ParseCommand();
        if (CurCommand == MenuCommand::exit) commandMap[CurCommand](IsPrintingMenu); 
        else if (commandMap.find(CurCommand) != commandMap.end()) {
            commandMap[CurCommand](UseCStrings);
        }
        else cout << "Wrong input." << endl;
    }

    return 0;
}

