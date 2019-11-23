#include "compressor.hpp"

#include <algorithm>
#include <iterator>

using namespace std;

const size_t compressor::jam_cap = 255;
const size_t compressor::lookahead_buffer_cap = 255;

compressor::compressor(const char* in_file,
					   const char* out_file)
		: is{ in_file, ios::binary }
		, os{ out_file, ios::binary }
		, jam(jam_cap, 0)
		, lookahead_buf{ new char[lookahead_buffer_cap] }
		, original_size(0u)
		, compression(0u) {
}

compressor::~compressor() {
	delete[] lookahead_buf;
}

void compressor::run() {
	is.read(lookahead_buf, lookahead_buffer_cap);
	lookahead_buf_len = is.gcount();
	while (lookahead_buf_len > 0) {
		auto p = find_longest_match();

		auto pb = (char)p.first;
		auto lb = (char)p.second;
		os.write(&pb, 1);
		os.write(&lb, 1);
		os.write(lookahead_buf + lb, 1);
		slide(lb + 1);
		original_size += lb + 1;
		compression++;
	}
	cout << "Compressed " << original_size
		<< " bytes downto " << compression
		<< " tuples and " << (compression * 3)
		<< " bytes." << endl;
}

bool compressor::is_sequence_match(
	size_t jam_start_index,
	size_t lookahead_buffer_end_index) {

	for (auto i = 0; i < lookahead_buffer_end_index; i++) {
		if (jam[i + jam_start_index] != lookahead_buf[i]) {
			return false;
		}
	}
	return true;
}

int compressor::test_sequence(size_t length) {

	if (length > jam.size()) {
		return -1;
	}

	for (auto i = 0; i <= jam.size() - (int)length; i++) {
		if (is_sequence_match(i, length)) {
			return (int) jam.size() - i;
		}
	}
	return -1;
}

pair<size_t, size_t> compressor::find_longest_match() {
	auto p = make_pair(0u, 0u);

	if (!jam.empty() && lookahead_buf_len > 0) {
		for (auto len = 1u; len < lookahead_buf_len; len++) {
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

void compressor::slide(streamsize n) {

	auto i = n;

	if (jam.size() < jam_cap) {
		auto r = jam_cap - jam.size();
		i = n > r ? n - r : 0;
	}

	if (i > jam.size()) {
		i = jam.size();
	}

	jam.erase(jam.begin(), jam.begin() + i);
	copy(lookahead_buf, lookahead_buf + n, back_inserter(jam));

	copy(lookahead_buf + n, lookahead_buf + lookahead_buf_len, lookahead_buf);
	is.read(lookahead_buf + lookahead_buf_len - n, n);
	lookahead_buf_len -= n - is.gcount();
}
