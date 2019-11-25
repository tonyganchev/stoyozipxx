#pragma once

#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

namespace szxx {

class decompressor {
public:
    decompressor(const char* in_file,
                 const char* out_file);
    void run();

private:
	std::ifstream is;
	std::ofstream os;

};

}
