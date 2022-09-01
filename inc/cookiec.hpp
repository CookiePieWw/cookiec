#ifndef _TOY_COMPILER
#define _TOY_COMPILER

#include <iostream>
#include <string>
#include <string_view>
#define ERROR(...) error(__FILE__, __LINE__, __func__, ...)
#define INFO(...) info(__FILE__, __LINE__, __func__, ...)

inline void error(const char *fname, int lineno, const char *fxname, const char *msg) {
	// std::cerr << std::format("[%s, %d, %s] ERROR: ", fname, lineno, fxname) << msg << std::endl;
	std::cerr << '[' << fname << ", " << lineno << ", " << fxname << "] ERROR: " << msg << std::endl; // WE NEED STD::FORMAT!!
}

inline void info(const char *fname, int lineno, const char *fxname, const char *msg) {
	std::clog << '[' << fname << ", " << lineno << ", " << fxname << "] INFO: " << msg << std::endl; // WE NEED STD::FORMAT!!
}

#endif
