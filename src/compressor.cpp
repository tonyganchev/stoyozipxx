#include <algorithm>
#include <iterator>
#include <exception>

#include "compressor.hpp"
#include "oligorithm.hpp"

using namespace std;
using namespace szxx;

const size_t compressor::jam_cap = 255;
const size_t compressor::lookahead_buffer_cap = 255;

compressor::compressor(const char* in_file,
                       const char* out_file)
		: is{ in_file, ios::binary }
		, os{ out_file, ios::binary }
		, jam{ jam_cap, lookahead_buffer_cap }
		, original_size{ 0u }
		, compression{ 0u }
		, longest_chunk{ 0u } {
	is >> noskipws;
}

compressor::~compressor() {
	os.flush();
}

void compressor::run() {
	if (!is.good()) {
		throw runtime_error("Could not open input file.");
	}

	copy_n(istream_iterator<char>(is),
	       jam.capacity(),
	       back_inserter(jam));
	while (!jam.lookahead_empty()) {
		auto p = find_longest_match();

		auto offset = (unsigned char) p.first;
		auto length = (unsigned char) p.second;
		os.write((const char *) &offset, 1);
		os.write((const char *) &length, 1);
		copy_n(jam.lookahead() + length, 1, ostream_iterator<char>(os));
		jam.forward(length + 1, is);
		original_size += length + 1;
		compression++;
	}
	cout << "Compressed " << original_size << " bytes "
	     << "downto " << compression << " tuples "
	     << "and " << (compression * 3) << " bytes. "
	     << "Compression ratio is " << (original_size * 1.0 / compression / 3) << ". "
		 << "Longest chunk is " << longest_chunk << " bytes."
		 << endl;
	assert(jam.lookahead_empty());
}

pair<int, int> compressor::find_longest_match() {
	auto m = find_longest_prefix(jam.lookahead(), jam.end() - 1,
	                             jam.begin(), jam.lookahead());
	if (m.first == m.second) {
		return make_pair(0, 0);
	} else {
		return make_pair(jam.lookahead() - m.first, m.second - m.first);
	}
}
