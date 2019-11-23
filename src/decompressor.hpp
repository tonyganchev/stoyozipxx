#pragma once

#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

class decompressor {
public:
    decompressor(const char* in_file,
                 const char* out_file);
    ~decompressor();

    void run();

private:
	std::ifstream is;
	std::ofstream os;

};
