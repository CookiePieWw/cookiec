#ifndef _LEXER
#define _LEXER
#include "cookiec.hpp"
#include "token.hpp"

namespace lexer {
	using Tokens = std::vector<std::vector<token::token>>;
	using SrcType = std::vector<std::string>;

	SrcType read_file(const std::string &);
	Tokens lexical_analyze(SrcType &src);
}
#endif
