//
//  main.cpp
//  CMD-Video-Player
//
//  Created by Robert He on 2024/9/1.
//

#include <cstring>
#include <iostream>
#include <map>

#include "basic-functions.hpp"
#include "video-player.hpp"

void get_command(std::string input = "$DEFAULT") {
	if (input == "$DEFAULT") {
		std::cout << std::endl
				  << "Your command >> ";
		std::getline(std::cin, input);
	}

	cmdOptions cmdOpts = parseArguments(parseCommandLine(input));

	/*
	printVector(parseCommandLine(input));
	printMap(cmdOpts.options);
	printVector(cmdOpts.arguments);
	 */

	if (cmdOpts.arguments.size() > 1) {
		std::cout << "Arguments Error!" << std::endl;
		show_help(true);
		get_command();
		return;
	}
	if (cmdOpts.options.find("-h") != cmdOpts.options.end()) {
		show_help(true);
		get_command();
		return;
	}
	if (cmdOpts.options.find("-v") != cmdOpts.options.end()) {
		play_video(cmdOpts.options["-v"]);

		clear_screen();
		show_interface();
	}

	if (std::find(cmdOpts.arguments.begin(), cmdOpts.arguments.end(), "exit") !=
		cmdOpts.arguments.end()) {
		return;
	}

	get_command();
}

void start_ui() {
	show_interface();
	show_help();
	get_command();
}

int main(int argc, const char *argv[]) {
	clear_screen();
	switch (argc) {
		case 1:
			start_ui();
			break;
		case 2:
			play_video(argv[1]);
			break;
		default:
			start_ui();
	}
	return 0;
}
