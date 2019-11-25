#include <algorithm>
#include <cstdlib>
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
		notify(vm);

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
			cerr << "A single command is expected while "
					<< all_commands_count << " were specified." << endl;
			cout << desc << endl;
			return -1;
		}

		if (!vm.count("input")) {
			cerr << "Input file not specified." << endl;
			cout << desc << endl; 
			return -1;
		}
		const char* input_file = vm["input"].as<string>().c_str();

		if (test_count) {
			tester t(input_file);
			return t.run();
		}

		if (!vm.count("output")) {
			cerr << "Input file not specified." << endl;
			cout << desc << endl; 
			return -1;
		}
		const char* output_file = vm["output"].as<string>().c_str();

		if (compress_count) {
			compressor c { input_file, output_file };
			c.run();
			return 0;
		}
		
		if (decompress_count) {
			decompressor d { input_file, output_file };
			d.run();
			return 0;
		}
		
		// Should not be happenning ...
		assert(false);
	} catch (const error& ex) {
		cerr << ex.what() << endl;
		return -1;
	}
}
