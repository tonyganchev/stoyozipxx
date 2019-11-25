#include <cassert>

#include "tester.hpp"
#include "compressor.hpp"
#include "decompressor.hpp"

using namespace std;
using namespace szxx;

tester::tester(const char* in_file)
		: infn { in_file }
		, tmpfn { string(in_file) + ".compressed" }
		, outfn { string(in_file) + ".decompressed"}  {
}

int tester::run() {
	compressor { infn, tmpfn.c_str() }.run();
	decompressor { tmpfn.c_str(), outfn.c_str() }.run();

	ifstream origs { infn };
	ifstream fins { outfn.c_str() };

	if (!fins.good()) {
		cerr << "Cannot find the output file " << outfn << endl;
		return -1;
	}

	auto pos = 0ull;
	while (true) {
		pos++;
		char ic;
		origs >> ic;
		char oc;
		fins >> oc;
		if (origs.eof()) {
			if (fins.eof()) {
				cout << "Files match." << endl;
				return 0;
			}
			cerr << "Files differ at position " << pos
			     << ". Decompressed file is longer." << endl;
			return -1;
		}
		if (fins.eof()) {
			cerr << "Files differ at position " << pos
			     << ". Original file is longer." << endl;
			return -1;
		}
		if (ic != oc) {
			cerr << "Files differ at position " << pos << endl;
			return -1;
		}
	}
}
