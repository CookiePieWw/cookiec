#ifndef _TOKEN
#define _TOKEN
#include <limits>

namespace token {
	enum token_t {
		NUMBER,
		VAR,
		SPLIT,         // ,
		BOUNDS,        // ;
		// operators
		ADD,           // +
		MINUS,         // -
		DIV,           // /
		MULT,          // *
		ASIGN,         // =
		EQ,            // ==
		NEQ,           // !=
		GEQ,           // >=
		LEQ,           // <=
		GE,            // >
    LE,            // <
    AND,           // &&
    OR,            // ||
		// brakets
		L_PARENTHESES, // (
		R_PARENTHESES, // )
		L_BRACE,       // {
		R_BRACE,       // }
		// keywords
		WHILE,
		IF,
		INT,
		VOID,
		RETURN,
		ELSE,
	};

	struct token {
		std::size_t line;
		std::size_t pos;
		std::size_t len;
		token_t tk;
		token (std::size_t a, std::size_t b, std::size_t c) : line(a), pos(b), len(c) {};
		token (std::size_t a, std::size_t b, std::size_t c, token_t d) : line(a), pos(b), len(c), tk(d) {};
	};

	std::string tk_map(token_t);
}

#endif
