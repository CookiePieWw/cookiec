#include <cookiec.hpp>
#include <lexer.hpp>
#include <stack>
#include <fstream>

namespace lexer {
	SrcType read_file(const std::string &filename) {
		SrcType src;
		std::ifstream srcfile(filename);
		if (srcfile.is_open()) {
			char buf[100];
			while (srcfile.getline(buf, 100))
				src.emplace_back(buf);
		} else {
			std::cout << "cookiec" << ": " RED("error: ") << filename << ": No such file or directory" << std::endl;
			exit(0);
		}
		srcfile.close();
		return src;
	}

	void find_commented_lines(std::vector<bool> &com, SrcType &src) {
		// bug: cannot handle comment inside a string constant
		// "/*abcd*/" will be identified as ""
		std::stack<int> s;
		for (std::size_t i = 0; i < src.size(); ++i) { // task: implement a pythonic enumerate in C++
			SrcType::value_type line = src[i];
			if (line.empty()) {
				com[i] = true;
				continue;
			}
			for (std::size_t j = 0; j < line.size() - 1; ++j) {
				if (s.empty() && line[j] == '\\' && line[j+1] == '*') {
					s.emplace(i);
					line.erase(line.begin() + j, line.end());
				}
				if (!s.empty() && line[j] == '*' && line[j+1] == '\\') {
					for (std::size_t k = s.top() + 1; k < i - 1; ++k) {
						com[i] = true;
					}
					line.erase(line.begin(), line.begin() + j + 2);
				}
			}
		}
	}

	namespace trap {
		constexpr std::size_t unknown = std::numeric_limits<std::size_t>::max();

		enum trap_t {
			KWORD,
			DIRECT,
			OP,
			SBAR,
			EQS,
			NUM,
			PASS,
			ERROR,
		};

		static std::array<trap_t, 128> trap_table;

		static auto init_trap_table = []() {
			trap_table.fill(trap_t::ERROR);
			// key word
			for (int i = 'a'; i <= 'z'; i ++) trap_table[i] = trap_table[i+'A'-'a'] = KWORD;
			// single character operator
			for (int i = '('; i <= '-'; i ++) trap_table[i] = DIRECT;
			trap_table['{'] = trap_table['}'] = trap_table[';'] = DIRECT;
			// double characters operator
			trap_table['&'] = trap_table['|'] = trap_table['!'] = OP;
			// '/' and '//'
			trap_table['/'] = SBAR;
			// equals: =, ==, >=, <=
			trap_table['='] = trap_table['>'] = trap_table['<'] = EQS;
			// numbers
			for (int i = '0'; i <= '9'; i ++) trap_table[i] = NUM;
			// others
			trap_table['\r'] = trap_table['\t'] = trap_table[' '] = PASS;
			return nullptr;
		} ();

		void kword_handler(const SrcType::value_type &src_line, std::size_t &pointer, Tokens::value_type &tokens_line) {
			std::size_t begin = pointer, end = pointer + 1;
			while (end != src_line.size() && (isalpha(src_line[end]) || isdigit(src_line[end])))
				end ++;
			std::string word(src_line, begin, end - begin);
			// maybe we can handle it with X-Macro
			if (word == "while")
				tokens_line.emplace_back(unknown, begin, end - begin, token::token_t::WHILE);
			else if (word == "if")
				tokens_line.emplace_back(unknown, begin, end - begin, token::token_t::IF);
			else if (word == "int")
				tokens_line.emplace_back(unknown, begin, end - begin, token::token_t::INT);
			else if (word == "void")
				tokens_line.emplace_back(unknown, begin, end - begin, token::token_t::VOID);
			else if (word == "return")
				tokens_line.emplace_back(unknown, begin, end - begin, token::token_t::RETURN);
			else if (word == "else")
				tokens_line.emplace_back(unknown, begin, end - begin, token::token_t::ELSE);
			else
				tokens_line.emplace_back(unknown, begin, end - begin, token::token_t::VAR);
			pointer = end;
		}

		void direct_handler(const SrcType::value_type &src_line, std::size_t &pointer, Tokens::value_type &tokens_line) {
			constexpr std::array<token::token_t, 6> direct = {
				token::token_t::L_PARENTHESES, token::token_t::R_PARENTHESES, token::token_t::MULT, token::token_t::ADD, token::token_t::SPLIT, token::token_t::MINUS
			};
			tokens_line.emplace_back(unknown, pointer, 1);
			switch (src_line[pointer]) {
				case '{':
					tokens_line.back().tk = token::token_t::L_BRACE;
					break;
				case '}':
					tokens_line.back().tk = token::token_t::R_BRACE;
					break;
				case ';':
					tokens_line.back().tk = token::token_t::BOUNDS;
					break;
				default:
					tokens_line.back().tk = direct[static_cast<int>(src_line[pointer]) - '('];
					break;
			}
			pointer++;
		}

		void op_handler(const SrcType::value_type &src_line, std::size_t &pointer, Tokens::value_type &tokens_line) {
			tokens_line.emplace_back(unknown, pointer, 2);
			switch (src_line[pointer]) {
				case '&':
					tokens_line.back().tk = token::token_t::AND;
					break;
				case '|':
					tokens_line.back().tk = token::token_t::OR;
					break;
				case '!':
					tokens_line.back().tk = token::token_t::NEQ;
			}
			pointer += 2;
		}

		void sbar_handler(const SrcType::value_type &src_line, std::size_t &pointer, Tokens::value_type &tokens_line) {
			if (pointer == src_line.size() - 1 || src_line[pointer + 1] != '/') {
				tokens_line.emplace_back(unknown, pointer, 1, token::token_t::DIV);
				pointer += 1;
			}
			else
				pointer = src_line.size();
		}

		void eqs_handler(const SrcType::value_type &src_line, std::size_t &pointer, Tokens::value_type &tokens_line) {
			if (pointer != src_line.size() - 1 && src_line[pointer + 1] == '=') {
				tokens_line.emplace_back(unknown, pointer, 2);
				switch (src_line[pointer]) {
					case '=':
						tokens_line.back().tk = token::token_t::EQ;
						break;
					case '>':
						tokens_line.back().tk = token::token_t::GEQ;
						break;
					case '<':
						tokens_line.back().tk = token::token_t::LEQ;
						break;
				}
				pointer += 2;
			}
			else{
				tokens_line.emplace_back(unknown, pointer, 1);
				switch (src_line[pointer]) {
					case '=':
						tokens_line.back().tk = token::token_t::ASIGN;
						break;
					case '>':
						tokens_line.back().tk = token::token_t::GE;
						break;
					case '<':
						tokens_line.back().tk = token::token_t::LE;
						break;
				}
				pointer ++;
			}
		}

		void num_handler(const SrcType::value_type &src_line, std::size_t &pointer, Tokens::value_type &tokens_line) {
			std::size_t begin = pointer, end = pointer + 1;
			while (end != src_line.size() && isdigit(src_line[end]))
				end ++;
			tokens_line.emplace_back(unknown, begin, end - begin, token::token_t::NUMBER);
			pointer = end;
		}
	}


	Tokens::value_type lexical_analyze_line(const SrcType::value_type &src_line, const std::size_t lineno) {
		Tokens::value_type tokens_line;
		std::size_t pointer = 0;
		while (pointer < src_line.size()) {
			const trap::trap_t trap_gate = trap::trap_table[src_line[pointer]];
			switch (trap_gate) {
				// caution: handle ERROR and PASS
				case trap::trap_t::ERROR:
					compile_error(lineno, src_line, pointer, "Unknown identifier.");
					break;
				case trap::trap_t::PASS:
					pointer++;
					break;
				default:
					// caution: explicit type casting
					constexpr std::array<decltype(trap::kword_handler) *, 6>dispatcher = {
						trap::kword_handler, 
						trap::direct_handler, 
						trap::op_handler, 
						trap::sbar_handler, 
						trap::eqs_handler, 
						trap::num_handler
					};
					dispatcher[static_cast<int>(trap_gate)](src_line, pointer, tokens_line);
			}
		}
		for (auto &i: tokens_line)
			i.line = lineno;
		return tokens_line;
	}

	Tokens lexical_analyze(SrcType &src) {
		std::vector<bool> com(src.size(), false);
		find_commented_lines(com, src);
		Tokens tokens;
		for (int i = 0; i < src.size(); ++i) {
			if (!com[i])
				tokens.emplace_back(lexical_analyze_line(src[i], i));
		}
		return tokens;
	}	
}
