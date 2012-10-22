#ifndef __UTILITIES_HH__
#define __UTILITIES_HH__

#include "common.hh"
#include <sstream>

namespace calc {

// format a std::string like sprintf

std::string 
strprintf(char const *fmt, ...)
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
	
	template <typename T>
	stringize&operator <<(T const &v) {
		_ss << v;
		return *this;
	}

	operator std::string() const {
		return _ss.str();
	}
};

// read a file into a string in a single shot.
std::string
slurp(std::string const &filename);

// read a file into an existing string in a single shot.
// returns a bool indicating success or failure.
void
slurp_into(std::string const &filename, std::string &dest);






}





#endif