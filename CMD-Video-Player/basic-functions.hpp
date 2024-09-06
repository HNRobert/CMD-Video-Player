//
//  basic-functions.hpp
//  CMD-Video-Player
//
//  Created by Robert He on 2024/9/1.
//

#ifndef basic_functions_hpp
#define basic_functions_hpp

#include <stdio.h>
#include <iostream>
#include <map>

struct cmdOptions {
    std::map<std::string, std::string> options;
    std::vector<std::string> arguments;
};

template <typename T>
void printVector(const std::vector<T>& vec) {
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

template <typename K, typename V>
void printMap(const std::map<K, V>& m) {
    for (const auto& pair : m) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}

void get_terminal_size(int &width, int &height);
std::string get_system_type();
void show_interface();
void show_help(bool full_version=false);
void clear_screen();
std::vector<std::string> parseCommandLine(const std::string& str);
cmdOptions parseArguments(const std::vector<std::string>& args);

#endif /* basic_functions_hpp */
