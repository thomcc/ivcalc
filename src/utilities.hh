#ifndef __UTILITIES_HH__
#define __UTILITIES_HH__

#include "common.hh"
#include <sstream>

namespace numbers {

// format a std::string like sprintf

std::string 
strprintf(char const *fmt, ...);

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

// unconvert anything stringlike.
// unsafe if you aren't sure about the input type.
// float pi_ish = unstringize<float>("3.14");
template <typename R, typename S>
R unstringize(S const &str) {
	std::stringstream ss;
	ss << str;
	R value;
	ss >> value;
	return value;
}

// read a file into a string in a single shot.
std::string
slurp(std::string const &filename);

// read a file into an existing string in a single shot.
// returns a bool indicating success or failure.
bool
slurp_into(std::string const &filename, std::string &dest);






}





#endif