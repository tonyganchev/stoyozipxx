#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "compressor.hpp"

using namespace std;

static const size_t window_cap = 1;
static const size_t lookahead_buffer_cap = 1;

void print_usage() {
	cout << "Usage:" << endl;
	cout << "\tstoyozipxx <command> <in-file> <out-file>" << endl;
}

void bail_out() {
	cerr << "Invalid command" << endl;
	print_usage();
	exit(-1);
}

int compress(const char* in_file,
			 const char* out_file) {
	compressor c { in_file, out_file };
	c.run();
	return 0;
}

int decompress(const char* in_file,
			   const char* out_file) {
	return 0;
}

int main(int argc,
		 char** argv) {
	if (argc != 4) {
		bail_out();
	}
	const auto& command = argv[1];
	switch (*command) {
	case 'c':
		return compress(argv[2], argv[3]);
	case 'd':
		return decompress(argv[2], argv[3]);
	default:
		bail_out();
	}
}
