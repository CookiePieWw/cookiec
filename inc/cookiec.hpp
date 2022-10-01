#ifndef _TOY_COMPILER
#define _TOY_COMPILER

#define VERSION 0.1
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <array>
#define COLOR(msg, color) "\e[0;" #color "m" msg "\e[0m"
#define RED(msg) COLOR(msg, 31)
#define BLUE(msg) COLOR(msg, 34)

#ifdef DEBUG
#define ERROR(...) error(__FILE__, __LINE__, __func__, ...)
#define INFO(...) info(__FILE__, __LINE__, __func__, ...)
#else
#define ERROR(...)
#define INFO(...)
#endif

extern std::string filename;

inline void error(const char *fname, int lineno, const char *fxname, const char *msg) {
	// std::cerr << std::format("[%s, %d, %s] ERROR: ", fname, lineno, fxname) << msg << std::endl;
	std::cerr << '[' << fname << ", " << lineno << ", " << fxname << "] " RED("error") ": " << msg << std::endl; // WE NEED STD::FORMAT!!
}

inline void info(const char *fname, int lineno, const char *fxname, const char *msg) {
	std::clog << '[' << fname << ", " << lineno << ", " << fxname << "] " BLUE("info") ": " << msg << std::endl; // WE NEED STD::FORMAT!!
}

inline void compile_error(const std::size_t lineno, const std::string &line, const std::size_t pos, const char *msg) {
	std::cout << filename << ':' << lineno << ':' << pos << ": " << msg << std::endl;
	std::cout << std::setiosflags(std::ios::right)
						<< std::setw(5) << lineno
						<< std::setiosflags(std::ios::left)
						<< " |" << line << std::endl
						<< std::setiosflags(std::ios::right)
						<< std::setw(7) << '|'
						<< std::setw(pos+1) << '^' << std::endl;
}

#endif
