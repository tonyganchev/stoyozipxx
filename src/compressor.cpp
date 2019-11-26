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
		, jam{ jam_cap }
		, lookahead_buf{ lookahead_buffer_cap }
		, original_size(0u)
		, compression(0u) {
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
	       lookahead_buf.capacity(),
	       back_inserter(lookahead_buf));
	while (!lookahead_buf.empty()) {
		auto p = find_longest_match();

		auto offset = (unsigned char) p.first;
		auto length = (unsigned char) p.second;
		os.write((const char *) &offset, 1);
		os.write((const char *) &length, 1);
		copy_n(lookahead_buf.begin() + length, 1, ostream_iterator<char>(os));
		slide(length + 1);
		original_size += length + 1;
		compression++;
	}
	cout << "Compressed " << original_size
	     << " bytes downto " << compression
	     << " tuples and " << (compression * 3)
	     << " bytes." << endl;
}

bool compressor::is_sequence_match(size_t jam_start_index,
                                   size_t lookahead_buffer_end_index) {

	for (auto i = 0; i < lookahead_buffer_end_index; i++) {
		if (jam[jam_start_index + i] != lookahead_buf[i]) {
			return false;
		}
	}
	return true;
}

int compressor::test_sequence(size_t length) {

	if (length > jam.size()) {
		return -1;
	}

	for (auto i = 0u; i <= jam.size() - length; i++) {
		if (is_sequence_match(i, length)) {
			return (int) jam.size() - i;
		}
	}
	return -1;
}

pair<char, char> compressor::find_longest_match() {
	auto p = make_pair('\0', '\0');

	if (!jam.empty() && !lookahead_buf.empty()) {
		for (auto len = 1u; len < lookahead_buf.size(); len++) {
			auto match_index = test_sequence(len);
			if (match_index == -1) {
				break;
			}
			p.first = (size_t)match_index;
			p.second = len;
		}
	}

	return p;
}

template<class ReadIt, class WriteIt>
void copy_upto_n(ReadIt begin, ReadIt end, size_t n, WriteIt dest) {
	for (auto it = begin; it != end && n > 0; n--) {
		*dest++ = *it;
		if (n > 1) {
			++it;
		}
	}
}

void compressor::slide(streamsize n) {
	copy_n(lookahead_buf.begin(), n, back_inserter(jam));
	lookahead_buf.erase_begin(n);
	copy_upto_n(istream_iterator<char>(is),
	            istream_iterator<char>(),
	            n,
	            back_inserter(lookahead_buf));
}
