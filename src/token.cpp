#include <cookiec.hpp>
#include <token.hpp>

namespace token {
	std::string tk_map(token_t tk) {
		std::array<std::string, ELSE - NUMBER + 1> token_map = {
			"NUMBER", "VAR", "SPLIT", "BOUNDS", "ADD", "MINUS", "DIV", "MULT", "ASIGN", "EQ", "NEQ", "GEQ",
			"LEQ", "GE", "LE", "AND", "OR", "L_PARENTHESES", "R_PARENTHESES", "L_BRACE", "R_BRACE", "WHILE",
			"IF", "INT", "VOID", "RETURN", "ELSE"
		};
		return token_map[static_cast<int>(tk)];
	}
}
