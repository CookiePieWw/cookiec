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

	// Tokens lexical_analyze(SrcType &&src)
}
