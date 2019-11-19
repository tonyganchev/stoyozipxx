#include "compressor.hpp"

#include <algorithm>
#include <iterator>

using namespace std;

const size_t compressor::window_cap = 10;
const size_t compressor::lookahead_buffer_cap = 10;

compressor::compressor(const char* in_file,
					   const char* out_file)
	: is{ in_file, ios::binary }
	, os{ out_file, ios::binary }
	, window(window_cap, 0)
	, lookahead_buf{ new char[lookahead_buffer_cap] } {
}

compressor::~compressor() {
	delete[] lookahead_buf;
}

void compressor::run() {
	is.read(lookahead_buf, lookahead_buffer_cap);
	lookahead_buf_len = is.gcount();
	while (lookahead_buf_len > 0 && !is.eof()) {
		auto p = find_longest_match();

		auto pb = (char)p.first;
		auto lb = (char)p.second;
		os.write(&pb, 1);
		os.write(&lb, 1);
		if (p.second == 0) {
			os.write(lookahead_buf, 1);
		}
		slide(1);
	}
}

bool compressor::is_sequence_match(
	size_t window_start_index,
	size_t lookahead_buffer_end_index) {

	for (auto i = 0; i < lookahead_buffer_end_index; i++) {
		if (window[i + window_start_index] != lookahead_buf[i]) {
			return false;
		}
	}
	return true;
}

int compressor::test_sequence(size_t length) {

	if (length > window.size()) {
		return -1;
	}

	for (auto i = 0; i <= window.size() - (int)length; i++) {
		if (is_sequence_match(i, length)) {
			return (int) window.size() - i;
		}
	}
	return -1;
}

pair<size_t, size_t> compressor::find_longest_match() {
	auto p = make_pair(0u, 0u);

	if (!window.empty() && lookahead_buf_len > 0) {
		for (auto i = 3u; i < lookahead_buf_len; i++) {
			auto match_index = test_sequence(i);
			if (match_index == -1) {
				break;
			}
			p.first = (size_t)match_index;
			p.second = i;
		}
	}

	return p;
}

void compressor::slide(streamsize n) {

	auto i = n;

	if (window.size() < window_cap) {
		auto r = window_cap - window.size();
		i = n > r ? n - r : 0;
	}

	if (i > window.size()) {
		i = window.size();
	}

	window.erase(window.begin(), window.begin() + i);
	//window.resize(i + n);
	copy(lookahead_buf, lookahead_buf + n, back_inserter(window));

	copy(lookahead_buf + n, lookahead_buf + lookahead_buf_len, lookahead_buf);
	is.read(lookahead_buf + lookahead_buf_len - n, n);
}
