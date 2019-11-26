#pragma once

#include <iostream>
#include <boost/circular_buffer.hpp>

namespace szxx {

class buffer {
private:
	using bcb = boost::circular_buffer<char>;
public:
	using value_type = char;
	using iterator = bcb::iterator;
	using const_iterator = bcb::const_iterator;

	iterator begin() { return b.begin(); }
	const_iterator begin() const { return b.begin(); }
	const_iterator cbegin() const { return b.begin(); }

	iterator end() { return b.end(); }
	const_iterator end() const { return b.end(); }
	const_iterator cend() const { return b.end(); }

	iterator lookahead() { return b.begin() + la_idx; }
	const_iterator lookahead() const { return b.begin() + la_idx; }
	const_iterator clookahead() const { return b.begin() + la_idx; }

	size_t capacity() const { return b.capacity(); }
	size_t size() const { return b.size(); }

	size_t lookback_size() const { return la_idx; }
	size_t lookahead_size() const { return b.size() - la_idx; }

	bool empty() const { return b.size() == 0u; }
	bool lookahead_empty() const { return lookahead_size() == 0u; }

	void push_back(const value_type c) {
		b.push_back(c);
	}

	void forward(size_t n, std::istream& is) {
		auto free_bytes = b.capacity() - b.size();

		size_t pos_to_advance;
		size_t bytes_to_add;
		size_t lookback_to_drop;
		if (la_idx < lb_cap) {
			pos_to_advance = std::min(lb_cap - la_idx, n);
			bytes_to_add = n - pos_to_advance;
			lookback_to_drop = n - pos_to_advance;
		} else {
			pos_to_advance = 0u;
			bytes_to_add = n;
			lookback_to_drop = n; 
		}
		b.erase_begin(lookback_to_drop);

		la_idx += pos_to_advance;
		auto rn = 1u;
		for (; rn <= bytes_to_add; rn++) {
			char c;
			is.read(&c, 1);
			if (!is.good()) {
				break;
			}
			b.push_back(c);
			if (is.eof()) {
				break;
			}
		}

		assert(la_idx <= lb_cap);
		assert(lookback_size() <= la_idx);
		assert(la_idx <= b.size());
	}

	buffer(size_t lookback_cap, size_t lookahead_cap)
			: lb_cap { lookback_cap }
			, la_cap { lookahead_cap }
			, b { lookback_cap + lookahead_cap }
			, la_idx { 0u } {
	}

	friend std::ostream& operator <<(std::ostream& s, const szxx::buffer& b) {
		s << "Size: " << b.size() << " Capacity: " << b.capacity() << std::endl << '[';
		copy(b.begin(), b.end(), std::ostream_iterator<char>(s));
		for (auto i = b.size(); i < b.capacity(); i++) {
			s << '*';
		}
		s << ']' << std::endl << ' ';
		for (auto i = 0u; i < b.lookback_size(); i++) {
			s << ' ';
		}
		s << '^';
		return s;
	}

private:
	boost::circular_buffer<char> b;
	size_t la_idx;
	size_t lb_cap;
	size_t la_cap;
};

}
