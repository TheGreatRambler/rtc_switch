#include <cxxopts.hpp>

// Main program entrypoint
int main(int argc, char* argv[]) {
	cxxopts::Options commandLineOptions("RTC Switch", "Use the Real Time Corrupter on actual Switch hardware");
	// clang-format off
	commandLineOptions.add_options ()
		// https://github.com/jarro2783/cxxopts
		("a,address", "Specify the Switch address");
	// clang-format on
	cxxopts::ParseResult commandLineResult = commandLineOptions.parse(argc, argv);

	return 0;
}