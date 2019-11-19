#pragma once

#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

class compressor {
public:
	compressor(const char* in_file,
			   const char* out_file);
	~compressor();

	void run();

private:
	static const size_t window_cap;
	static const size_t lookahead_buffer_cap;

	std::ifstream is;
	std::ofstream os;
	std::vector<char> window;
	char* lookahead_buf;
	size_t lookahead_buf_len;

	void slide(std::streamsize n);
	std::pair<size_t, size_t> find_longest_match();
	int test_sequence(size_t length);
	bool is_sequence_match(size_t window_start_index,
						   size_t lookahead_buffer_end_index);
};
