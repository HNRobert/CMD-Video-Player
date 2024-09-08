//
//  basic-functions.cpp
//  CMD-Video-Player
//
//  Created by Robert He on 2024/9/1.
//

#include "basic-functions.hpp"
#include <cstring>
#include <iostream>
#include <map>

#ifdef _WIN32
#include <windows.h>

void get_terminal_size(int &width, int &height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.dwSize.X;
        height = csbi.dwSize.Y;
    } else {
        // 获取失败时的处理
        width = 80;  // 默认宽度
        height = 25; // 默认高度
    }
}

#else
#include <sys/ioctl.h>
#include <unistd.h>

void get_terminal_size(int &width, int &height) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        width = ws.ws_col;
        height = ws.ws_row;
    } else {
        // 获取失败时的处理
        width = 80;  // 默认宽度
        height = 24; // 默认高度
    }
}

#endif

const std::string SYS_TYPE = get_system_type();

std::string get_system_type() {
#ifdef _WIN32
    return "Windows\n";
#elif __linux__
    return "Linux\n";
#elif __APPLE__
    return "macOS\n";
#endif
}

void show_interface() {
    std::cout << R"(
  ____ __  __ ____   __     ___     _            
 / ___|  \/  |  _ \  \ \   / (_) __| | ___  ___  
| |   | |\/| | | | |  \ \ / /| |/ _` |/ _ \/ _ \ 
| |___| |  | | |_| |   \ V / | | (_| |  __/ (_) |
 \____|_|  |_|____/     \_/  |_|\__,_|\___|\___/ 
                                                 
 ____  _                       
|  _ \| | __ _ _   _  ___ _ __ 
| |_) | |/ _` | | | |/ _ \ '__|
|  __/| | (_| | |_| |  __/ |        - by HNRobert
|_|   |_|\__,_|\__, |\___|_|   
               |___/            
)";
}

void show_help(bool full_version) {
    if (full_version)
        std::cout << R"(
Usage:
#################################################
-------------------------------------------------
help -- Get usage
-------------------------------------------------
play -- Start palyback

play -v /path/to/a/video [-stct | -dyct] [-c s/l]
     [-chars "@%#*+=-:. "]

"play -v PATH_TO_YOUR_VIDEO" to play a video
    Examples: (1st for Windows, 2nd for macOS)
        >> play -v C:\video.mp4
        >> play -v "/Users/robert/a video.mov"
-------------------------------------------------
set -- Change settings permanently
-------------------------------------------------
exit - Quit the programme
-------------------------------------------------
#################################################

Project URL: 

)";
    else std::cout << R"(
-------- Type "help" and return for help --------
)";
}

void clear_screen() {
    if (SYS_TYPE == "Windows")
        system("cls");
    else
        system("clear");
}

std::pair<int, const char **> parseCommandLine(const std::string &str) {
    std::vector<std::string> result;
    std::string currentArg;
    bool inQuotes = false; // if we're between "s or 's
    char quoteChar = '\0'; // to track which quote character is used (' or ")

    for (size_t i = 0; i < str.length(); ++i) {
        char ch = str[i];

        if (ch == '"' || ch == '\'') {
            // Toggle inQuotes state if matching quote is found
            if (!inQuotes) {
                inQuotes = true;
                quoteChar = ch; // Set the current quote character
            } else if (quoteChar == ch) {
                inQuotes = false;
            }
        } else if (std::isspace(ch) && !inQuotes) {
            // Finish the argument if not inside quotes
            if (!currentArg.empty()) {
                result.push_back(currentArg);
                currentArg.clear();
            }
        } else if (ch == '\\' && i + 1 < str.length()) {
            // Handle escaped characters
            char nextChar = str[i + 1];
            if (nextChar == '"' || nextChar == '\'' || nextChar == '\\') {
                currentArg += nextChar; // Add escaped character
                ++i;                    // Skip the next character
            } else {
                currentArg += ch;
            }
        } else {
            // Add other characters directly
            currentArg += ch;
        }
    }

    // Add the final argument if it's not empty
    if (!currentArg.empty()) {
        result.push_back(currentArg);
    }

    // Convert std::vector<std::string> to const char* argv[]
    int argc = static_cast<int>(result.size());
    const char **argv = new const char *[argc + 1]; // +1 for the NULL terminator

    for (int i = 0; i < argc; ++i) {
        // Allocate memory for each string and copy its content
        argv[i] = new char[result[i].size() + 1];               // +1 for null terminator
        strcpy(const_cast<char *>(argv[i]), result[i].c_str()); // Copy string content
    }
    argv[argc] = nullptr; // NULL terminator for argv

    return std::make_pair(argc, argv);
}

cmdOptions parseArguments(const std::pair<int, const char **> &args, const char *self_name) {
    cmdOptions cmdOptions;
    int argc = args.first;
    const char **argv = args.second;
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == self_name)
            continue;
        if (arg[0] == '-') { // option starts with '-'
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                // Check if there's a value following or not
                cmdOptions.options[arg] = argv[++i];
            } else {
                cmdOptions.options[arg] = ""; // option without value
            }
        } else {
            cmdOptions.arguments.push_back(arg); // normal arg
        }
    }

    return cmdOptions;
}

void print_error(std::string error_name, std::string error_detail) {
    std::cerr << error_name;
    if (error_detail.length())
        std::cout << ": " << error_detail;
    std::cout << std::endl
              << "Press any key to continue..." << std::endl;
    getchar();
}
