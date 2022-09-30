#include <cookiec.hpp>
#include <lexer.hpp>
#include <getopt.h>

static void help();
static void version();

static std::string filename = "srcfile";

static const struct option longopts[] =
{
	{"version", no_argument      , nullptr, 'v'},
	{"help"   , no_argument      , nullptr, 'h'},
	{"lexical", no_argument      , nullptr, 'l'},
	{"file"   , required_argument, nullptr, 'f'},
};

static const char *optstring = "vhlf:";

int main(int argc, char **argv)
{
	int c;
	bool l;
	while ((c = getopt_long(argc, argv, optstring, longopts, nullptr)) != -1) {
		switch (c) {
			case 'v':
				version();
				return 0;
			case 'h':
				help();
				return 0;
			case 'l':
				l = true;
				break;
			case 'f':
				filename = optarg;
				break;
		}
	}

	if (l) {
		auto src = lexer::read_file(filename);
		//lexer::lexical_analyze(src);
		std::cout << "src filename: " << filename << std::endl;
		// lexial_analyze(filename);
	}
	return 0;
}

void version() {
	std::cout << "cookiec " << VERSION << "\n"
			  << "Copyright (C) 2022 CookiePie\n";
}

void help() {
	std::cout << "Usage: cookiec [-l, --lexcial] [-f, --file \e[4minfile\e[0m]\n"
				 "   or: cookiec -v, --version\n"
				 "   or: cookiec -h, --help\n"
				 "\n"
				 "Compiles a C-like source file to LLVM IR.\n"
				 "\n"
				 "-l, --lexcial\tonly use lexer to analyze source file\n"
				 "-f, --file \e[4minfile\e[0m\n\t\tspecify source file to be compiled(default \e[4mtest/srcfile\e[0m)\n"
				 "-v, --version\tdisplay version information and exit\n"
				 "-h, --help\tdisplay this help and exit\n";
}

