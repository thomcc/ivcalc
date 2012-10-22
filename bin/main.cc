#include "common.hh"
#include "interval.hh"
#include "parser/parser.hh"
#include "print_visitor.hh"
#include "eval.hh"

using namespace numbers;

std::string get_line(bool continued){
	std::string line;
	while (line.size() == 0) {
		std::cout << (continued ? "| " : "> ");
		std::getline(std::cin, line);
	}
	return line;
}

int
repl(int vrb) {
	std::cout << std::endl;
	std::cout << "calc" << std::endl << std::endl;
	std::cout << "Press Ctrl+C to exit" << std::endl;
	std::cout << std::endl;
	Evaluator e;
	for (;;) {
		std::string src;
		ExprSPtr expr;
		for (;;) {
			std::string line = get_line(src.length() != 0);
			if (src.length()) src += "\n";
			src += line;
			ErrorHandler eh;
			Parser parser(src, eh);
			try {
				expr = parser.parse_expression();
			} catch (std::string s) {
				if (eh.errors() == 0) {
					std::cerr << "uncaught error: " << s << std::endl;
					return 3;
				} 
			}
			if (eh.need_lines()) continue;
			if (eh.errors() == 0) break;
			return 2;
		}
		if (expr.get()) {
			if (vrb) {
				if (vrb > 2) std::cout << "PRINT" << std::endl;
				PrintVisitor::output(expr);
				std::cout << std::endl;
			}
			if (vrb > 2) std::cout << "EVAL" << std::endl;
			interval res = e.eval(expr);
			std::cout << res << std::endl;
		} else {
			std::cerr << "Error: NULL expr!" << std::endl;
			return 4;
		}
	}
}

int
main(int argc, char *argv[]) {
	int v = 0;
	for (int i = 1; (i+1 < argc) && argv[1][0] == '-'; ++i) {
		switch (argv[i][1]) {
		case 'v': ++v; break;
		default:
			fprintf(stderr, "Unrecognized command: %s\n", argv[i]);
			break;
		}
	}
	return repl(v);
}



