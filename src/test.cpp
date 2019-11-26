#include <iostream>
#include <sstream>

#include "buffer.hpp"

using namespace std;

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

	return 0;
}
