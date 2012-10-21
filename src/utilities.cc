#include "utilities.hh"

#include <cstdarg>
#include <fstream>
#include <string>
#include <cstdlib>



namespace numbers {

std::string
strprintf(char const *fmt, ...) {
	
	int size = 128;
	
	std::string res;

	va_list argptr;

	for (;;) {

		res.resize(size);

		va_start(argptr, fmt);
		int n = vsnprintf((char*)res.c_str(), size, fmt, argptr);
		va_end(argptr);

		if ((n >= 0) && (n < size)) {
			res.resize(n);
			return res;
		}

		if (n > -1) size = n + 1;
		else size *= 2;

	}
}

void
slurp_into(std::string const &filename, std::string &dest) {
	std::ifstream file(filename.c_str(), std::ios_base::ate);
	file.exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);
	dest.resize(file.tellg());
	file.seekg(0, std::ios_base::beg);
	file.read(&dest[0], dest.size());
	file.close();
}


std::string
slurp(std::string const &filename) {
	std::string ret;
	slurp_into(filename, ret);
	return ret;
}




	
}


