#include "cookiec.hpp"
#include "token.hpp"

namespace lexer {
	using Tokens = std::vector<std::vector<token::token>>;
	using SrcType = std::vector<std::string>;

	SrcType read_file(std::string &);
	void find_commented_lines(std::vector<bool> &, SrcType &&);
	Tokens lexical_analyze(SrcType &src);
	Tokens::value_type lexical_analyze_line(SrcType &src);

	// trap to handle each word. refer to https://github.com/ganler/runzhong_machine_compiler.
	namespace trap {
		static std::array<token::trap_t, 128> trap_table;
		static auto init_trap_table = []() {
			// key word
			for (int i = 'a'; i <= 'z'; i ++) trap_table[i] = trap_table[i+'A'-'a'] = token::KWORD;
			// single character operator
			for (int i = '('; i <= '-'; i ++) trap_table[i] = token::DIRECT;
			trap_table['{'] = trap_table['}'] = trap_table[';'] = token::DIRECT;
			// double characters operator
			trap_table['&'] = trap_table['|'] = trap_table['!'] = token::OP;
			// '/' and '//'
			trap_table['/'] = token::SBAR;
			// equals: =, ==, >=, <=
			trap_table['='] = trap_table['>'] = trap_table['<'] = token::EQS;
			// numbers
			for (int i = '0'; i <= '9'; i ++) trap_table[i] = token::NUM;
			// others
			trap_table['\r'] = trap_table['\t'] = trap_table[' '] = token::PASS;
			return nullptr;
		} ();
		void kword_handler(std::string_view, std::size_t &, Tokens::value_type &);
		decltype(kword_handler) direct_handler, op_handler, sbar_handler, eqs_handler, num_handler;
		constexpr std::array<decltype(kword_handler) *, 6>dispacher = {kword_handler, direct_handler, op_handler, sbar_handler, eqs_handler, num_handler};
	}
}
