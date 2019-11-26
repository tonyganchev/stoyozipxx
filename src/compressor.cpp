#include "compressor.hpp"

#include <algorithm>
#include <iterator>
#include <exception>

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
		//cout << (int) p.first << ' ' << (int) p.second << endl;

		// if (original_size % 10 == 0) {
			// cout << "Ori: " << original_size << endl;
			// cout << jam << endl;
			// cout << "Mat: " << (int) p.first << "+" << (int) p.second << endl;
		// }

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
	auto p = make_pair(0, 0);

	for (auto length = 1u; length <= min(jam.lookback_size() + 1, jam.lookahead_size()) - 1; length++) {
		bool length_match = false;
		for (auto it = jam.begin(); it != jam.lookahead() - length; ++it) {
			bool match = true;
			for (auto jt = it; jt != it + length; ++jt) {
				if (*jt != *(jam.lookahead() + (jt - it))) {
					match = false;
					break;
				}
			}
			if (match) {
				p = make_pair(jam.lookahead() - it, length);
				length_match = true;
				break;
			}
		}
		if (!length_match) {
			break;
		}
	}
	longest_chunk = max((int) longest_chunk, p.second);
	return p;
}
