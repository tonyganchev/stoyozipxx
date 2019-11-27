#include <iostream>
#include <sstream>

#include "buffer.hpp"
#include "oligorithm.hpp"

using namespace std;

void test_find_longest_match(string needle,
                             string haystack,
                             size_t expected_length,
                             size_t expected_start_idx) {
	auto m = szxx::find_longest_prefix(needle.begin(), needle.end(),
	                                  haystack.begin(), haystack.end());
	if (expected_length != m.second - m.first
			|| (expected_length > 0 && expected_start_idx != m.first - haystack.begin())) {
		cout << "FAIL finding the expected prefix of '" << needle
		     << "' in '" << haystack << "'.\n";
		cout << "Instead matched '";
		copy(m.first, m.second, ostream_iterator<char>(cout));
		cout << "' of length " << m.second - m.first << "." << endl;
	}
}

int main() {
	stringstream is("sum dolor sit amet gatsi gatsi.");
	szxx::buffer buf{ 4u, 4u };
	buf.push_back('L');
	buf.push_back('o');
	buf.push_back('r');
	buf.push_back('e');
	buf.push_back('m');
	buf.push_back(' ');
	buf.push_back('i');
	buf.push_back('p');
	cout << buf << endl;

	while (!buf.lookahead_empty()) {
		cout << "--------------------------" << endl;
		cout << buf << endl;
		buf.forward(1u, is);
		cout << buf << endl;
	}

	test_find_longest_match("abc", "efghijk", 0, 0);
	test_find_longest_match("cde", "efghijk", 0, 0);
	test_find_longest_match("exyz", "efghijk", 1, 0);
	test_find_longest_match("efghijk", "efghijk", 7, 0);
	test_find_longest_match("abc", "efghijk", 0, 0);
	test_find_longest_match("", "efghijk", 0, 0);
	test_find_longest_match("efghijklmnop", "efghijk", 7, 0);
	test_find_longest_match("efghij", "efghijk", 6, 0);
	test_find_longest_match("ghij", "efghijk", 4, 2);
	test_find_longest_match("ghijk", "efghijk", 5, 2);
	test_find_longest_match("ghijkl", "efghijk", 5, 2);
	test_find_longest_match("kl", "efghijk", 1, 6);
	test_find_longest_match("f", "efghijk efghijk efghijk", 1, 1);
	return 0;
}
