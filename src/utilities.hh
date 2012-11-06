#ifndef __UTILITIES_HH__
#define __UTILITIES_HH__

#include "common.hh"
#include <sstream>
#include <functional>
#include <cctype>
#include <locale>

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


}





#endif
