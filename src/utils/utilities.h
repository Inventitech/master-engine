#ifndef UTILITIES_H
#define UTILITIES_H
#include <boost/program_options.hpp>

namespace po = boost::program_options;

bool parseCommandLineArgs(int argc, char** argv, bool& showFps);

#endif
