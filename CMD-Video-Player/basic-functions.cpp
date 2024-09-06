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
    std::cout << R"(
"-h" for help
)";
    if (!full_version)
        return;
    std::cout << R"(
"play -v [PATH_TO_YOUR_VIDEO]" to play a video
Correct Examples: (1st for Windows, 2nd for macOS)
>>play -v C:\video.mp4
>>play -v "/Users/robert/Downloads/video with spaces.mov"

"exit" to quit the programme

Project URL: 
)";
}

void clear_screen() {
    if (SYS_TYPE == "Windows")
        system("cls");
    else
        system("clear");
}
#include <cctype>
#include <string>
#include <vector>

std::vector<std::string> parseCommandLine(const std::string &str) {
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

    return result;
}

cmdOptions parseArguments(const std::vector<std::string> &args) {
    cmdOptions cmdOptions;

    for (size_t i = 0; i < args.size(); ++i) {
        std::string arg = args[i];

        if (arg[0] == '-') { // option starts with '-'
            if (i + 1 < args.size() && args[i + 1][0] != '-') {
                // check if there's a value following or not
                cmdOptions.options[arg] = args[i + 1];
                ++i; // jump over handled option
            } else {
                cmdOptions.options[arg] = ""; // option without value
            }
        } else {
            cmdOptions.arguments.push_back(arg); // norm arg
        }
    }

    return cmdOptions;
}
