#pragma once

#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

#include <boost/circular_buffer.hpp>

#include "buffer.hpp"

namespace szxx {

class compressor {
public:
	compressor(const char* in_file,
	           const char* out_file);
	~compressor();

	void run();

private:
	static const size_t jam_cap;
	static const size_t lookahead_buffer_cap;

	std::ifstream is;
	std::ofstream os;
	buffer jam;
	size_t original_size;
	size_t compression;

	std::pair<int, int> find_longest_match();
};

}
