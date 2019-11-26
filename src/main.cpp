#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include <boost/program_options.hpp>

#include "compressor.hpp"
#include "decompressor.hpp"
#include "tester.hpp"

using namespace szxx;
using namespace std;
using namespace boost::program_options;

int main(int argc,
		 char** argv) {

	//ios_base::sync_with_stdio(false);
	try {
		options_description desc{ "Options" };
		desc.add_options()
			("help,h", "Show usage information")
			("compress,c", "Compress a file.")
			("extract,x", "Extract a compressed file.")
			("test,t", "Compress a file, then extract it and compare the "
			           "resulting file with the original.")
			("input,i", value<string>(), "Source file.")
			("output,o", value<string>(), "Target file.");

		variables_map vm;
		store(parse_command_line(argc, argv, desc), vm);

		if (vm.count("help")) {
			cout << desc << endl;
			return 0;
		}

		const auto compress_count = vm.count("compress");
		const auto decompress_count = vm.count("extract");
		const auto test_count = vm.count("test");
		const auto all_commands_count =
				compress_count + decompress_count + test_count;
		if (all_commands_count != 1) {
			cerr << "A single command is expected while ";
			cerr << all_commands_count << " were specified." << endl;
			cout << desc << endl;
			return -1;
		}

		if (!vm.count("input")) {
			cerr << "Input file not specified." << endl;
			cout << desc << endl; 
			return -1;
		}
		auto input_file = vm["input"].as<string>().c_str();
		if (!filesystem::exists(filesystem::path(input_file))) {
			cerr << input_file << " does not exist." << endl;
			return -1;
		}

		if (test_count) {
			return tester { input_file }.run();
		}

		if (!vm.count("output")) {
			cerr << "Input file not specified." << endl;
			cout << desc << endl; 
			return -1;
		}
		auto output_file = vm["output"].as<string>().c_str();

		if (compress_count) {
			compressor { input_file, output_file }.run();
			return 0;
		}
		
		if (decompress_count) {
			decompressor { input_file, output_file }.run();
			return 0;
		}
		
		// Should not be happenning ...
		assert(false);
	} catch (const error& ex) {
		cerr << ex.what() << endl;
		return -1;
	}
}
