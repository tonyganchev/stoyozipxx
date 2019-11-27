#pragma once

#include <algorithm>
#include <iostream>
#include <utility>

template<typename A, typename B>
std::ostream& operator <<(std::ostream& s, const std::pair<A, B>& p) {
	s << "[" << p.first << ", " << p.second << "]";
	return s;
}

namespace szxx {

template <typename It>
std::pair<It, It> find_longest_prefix(It needle_begin,
                                      It needle_end,
                                      It haystack_begin,
                                      It haystack_end) {
	auto p = std::make_pair(haystack_end, haystack_end);
	for (auto hbit = haystack_begin; hbit != haystack_end; ++hbit) {
		bool matched = true;
		auto nit = needle_begin;
		for (auto heit = hbit;
				heit != haystack_end && nit != needle_end;
				++heit, ++nit) {
			if (*heit != *nit) {
				matched = false;
				assert(heit >= hbit);
				if (p.second - p.first < heit - hbit) {
					p = std::make_pair(hbit, heit);
				}
				break;
			}
		}
		if (matched) {
			p = std::make_pair(hbit, hbit + (nit - needle_begin));
			break;
		}
	}
	return p;
}

}
