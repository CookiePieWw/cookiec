#ifndef _TOY_LEXER
#include "cookiec.hpp"
#include <limits>

namespace token {
	constexpr std::size_t unknown = std::numeric_limits<std::size_t>::max();

	enum trap_t {
		KWORD,
		DIRECT,
		OP,
		SBAR,
		EQS,
		NUM,
		BRAKETS,
		PASS,
		ERROR,
	};

	enum token_t {
		NUMBER,
		VAR,
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
		token (std::size_t a, std::size_t b, std::size_t c, token_t d) : line(a), pos(b), len(c), tk(d) {};
	};
}

#endif
