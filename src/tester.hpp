#pragma once

#include <string>

namespace szxx {

class tester {
public:
	tester(const char* infn);
	int run();
private:
	const char* infn;
	std::string tmpfn;
	std::string outfn;
};

}
