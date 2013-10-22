#ifndef UTILITIES_CPP
#define UTILITIES_CPP
#include "utilities.h"

/**
 * Parses the given command line args with Boot libraries program_options
 * class. Returns true, if the program should exit and false otherwise.
 */
bool parseCommandLineArgs(int argc, char** argv, bool& showFps) {
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()("help", "produce this help message")("showFPS",
			"show frames per second in console output");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
		return true;
	}

	if (vm.count("showFPS")) {
		std::cout << "Displaying frames per second.\n";
		showFps = true;
	}
	return false;
}

#endif
