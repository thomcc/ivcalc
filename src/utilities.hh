#ifndef __UTILITIES_HH__
#define __UTILITIES_HH__

#include "common.hh"
#include <sstream>
#include <functional>
#include <cctype>
#include <locale>
#include <vector>
#include <new>
#include <cassert>

namespace calc {

// format a std::string like sprintf

std::string strprintf(char const *fmt, ...)
#if __has_attribute(format) || defined(__GNUC__)
__attribute__((format(printf, 1, 2)))
#endif
;

// easily convert anything to a string
// std::string foo = stringize() << 30 << 1.2 << "bah";
class stringize {
private:
	std::ostringstream _ss;
public:
	template <typename T> stringize &operator<<(T const &v) {
		_ss << v;
		return *this;
	}
	operator std::string() const { return _ss.str(); }
};

// read a file into a string in a single shot.
std::string slurp(std::string const &filename);

// read a file into an existing string in a single shot.
// returns a bool indicating success or failure.
void slurp_into(std::string const &filename, std::string &dest);

// remove spaces from start of string
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// remove spaces from end of string
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// remove spaces from both ends of string
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}


// used for visitor implementation, see visitorbase.hh

class NullType {};

template <typename T, typename S>
struct Typelist {
	typedef T Head;
	typedef S Tail;
};

template <typename...> struct Seq;

template <> struct Seq<> {
	typedef NullType Type;
};

template <typename A, typename... Rest> struct Seq<A, Rest...> {
  typedef Typelist<A, typename Seq<Rest...>::Type> Type;
};

//#define DEBUG_POOL

class MemPool {
	const size_t max_alloc;
	size_t cnt, n, max;
	void **buff;
	std::string name;
#ifdef DEBUG_POOL
	unsigned allocs, deallocs;
	size_t peak_cnt;
#endif
public:
	MemPool(size_t ma, std::string label)
		: max_alloc(ma), cnt(0), n(0), max(0), buff(nullptr), name(label)
#ifdef DEBUG_POOL
		, allocs(0), deallocs(0), peak_cnt(0)
#endif
		 {}
	~MemPool() {
		for (size_t k = 0; k < n; ++k) free(buff[k]);
		free(buff);
#ifdef DEBUG_POOL
		stats();
#endif
	}

	void stats() {
		std::cout << "~MemPool(" << max_alloc << " ," << name << ")";
#ifdef DEBUG_POOL
		std::cout << " allocs=" << allocs << ", deallocs=" << deallocs;
		std::cout << ", peak_cnt=" << peak_cnt << ", cnt=" << cnt << ",";
#endif
		std::cout << " n=" << n << ", cnt=" << cnt << ", max=" << max << std::endl;

	}

	void *allocate(size_t size) {
		assert(size <= max_alloc);
		++cnt;
#ifdef DEBUG_POOL
		++allocs;
		if (cnt > peak_cnt) peak_cnt = cnt;
#endif
		if (!n) return calloc(1, max_alloc);
		return buff[--n];
	}
	void deallocate(void *p) {
#ifdef DEBUG_POOL
		++deallocs;
#endif
		--cnt;
		if (n == max) {
			max = max ? max<<1 : 16;
			buff = (void**)realloc(buff, sizeof(void*) * max);
		}
		memset(p, '\0', sizeof *p);
		buff[n++] = p;
	}
};

/*
template <typename T>
class Pooled {
private:
	static MemPool _pool;
public:
	static void *operator new(size_t size) {
		return _pool.allocate(size);
	}
	static void operator delete(void *p) {
		_pool.deallocate(p);
	}
};
*/


}





#endif
