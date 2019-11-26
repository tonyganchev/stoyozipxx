#include "decompressor.hpp"

#include <boost/circular_buffer.hpp>

using namespace std;
using namespace szxx;

decompressor::decompressor(const char* in_file,
                           const char* out_file)
		: is{ in_file, ios::binary }
		, os{ out_file, ios::binary } {
}

decompressor::~decompressor() {
	os.flush();
}

void decompressor::run() {
	boost::circular_buffer<char> back_buf { 512 };

	auto n = 0u;
	while (true) {
		char ctuple[3];
		is.read(ctuple, sizeof(ctuple));
		if (is.eof()) {
			auto char_count = is.gcount();
			if (char_count != 0 && char_count != size(ctuple)) {
				throw runtime_error("Invalid format of input file.");
			}
			break;
		}

		const auto& offset = ctuple[0];
		const auto& length = ctuple[1];
		const auto& symbol = ctuple[2];

		auto match_begin = back_buf.end() - (unsigned char) offset;
		auto match_end = match_begin + (unsigned char) length;
		copy(match_begin, match_end, ostream_iterator<char>(os));
		copy(match_begin, match_end, back_inserter(back_buf));
		os.write((char *) &symbol, 1);
		back_buf.push_back(symbol);
		n++;
	}
	cout << "Decompressed " << n << " tuples." << endl;
}
