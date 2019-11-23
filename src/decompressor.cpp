#include "decompressor.hpp"

#include <string>

using namespace std;

decompressor::decompressor(const char* in_file,
                           const char* out_file)
        : is{ in_file, ios::binary }
        , os{ out_file, ios::binary } {
}

decompressor::~decompressor() { }

void decompressor::run() {
    string back_buf = "";
    auto n = 0u;
    while (true) {
        char ctuple[3];
        is.read(ctuple, 3);
        if (is.eof()) {
            break;
        }
        
        const unsigned char& offset = ctuple[0];
        const unsigned char& length = ctuple[1];
        const unsigned char& symbol = ctuple[2];

        // cout << back_buf << " " << back_buf.length() << " " << (unsigned) offset << " " << (unsigned) length << endl;
        auto match = back_buf.substr(back_buf.length() - (unsigned) offset, (unsigned) length);
        os << match;
        back_buf += match;
        if (back_buf.length() > 700) {
            back_buf = back_buf.substr(256);
        }
        os.write((char *) &symbol, 1);
        back_buf += symbol;

        n++;
    }
    cout << "Decompressed " << n << " tuples." << endl;
}
