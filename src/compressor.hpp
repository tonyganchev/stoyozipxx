#pragma once

#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

#include <boost/circular_buffer.hpp>

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
	boost::circular_buffer<char> jam;
	boost::circular_buffer<char> lookahead_buf;
	size_t original_size;
	size_t compression;

	void slide(std::streamsize n);
	std::pair<char, char> find_longest_match();
	int test_sequence(size_t length);
	bool is_sequence_match(size_t window_start_index,
	                       size_t lookahead_buffer_end_index);
};

}
