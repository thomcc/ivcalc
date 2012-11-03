#include "common.hh"
#include "interval.hh"
#include "parser/parser.hh"
#include "printer.hh"
#include "simplifier.hh"
#include "eval.hh"
#include "derivator.hh"
#include <cstdio>
#include <cstdlib>
#define LINE_LEN 4096

using namespace calc;
using namespace std;

static bool
is_balanced(const char *s) {
	int p, b, bk;
	for (p = b = bk = 0; *s; s++) {
		switch (*s) {
		case '(': ++p; break;
		case ')': --p; break;
		case '[': ++bk; break;
		case ']': --bk; break;
		case '{': ++b; break;
		case '}': --b; break;
		}
	}
	return (p == 0) && (b == 0) && (bk == 0);
}


std::string
get_line(bool continued){
	std::string line;
	while (!std::cin.eof()) {
		std::string l;
		std::cout << (continued ? "| " : "> ");
		std::getline(std::cin, l);
		line += l;
		if ((line.size() > 0) && is_balanced(line.c_str()))
			break;
		else continued = true;
	}
	return line;
}

int
repl(int vrb) {

	std::cout << std::endl;
	std::cout << "calc" << std::endl;
	std::cout << "Press Ctrl+C to exit" << std::endl;
	std::cout << std::endl;

	Evaluator e;
	Simplifier s;
	Printer print(std::cout, true);
	for (;;) {
		std::string src;
		ExprSPtr expr;

		for (;;) {
			bool cont = src.size() != 0;
			std::string line = get_line(cont);
			if (cont) src += "\n";
			src += line;
			ErrorHandler eh;
			Parser parser(src, eh);
			try {
				expr = parser.parse_expression();
			} catch (std::string s) {
				if (eh.errors() == 0) {
					std::cerr << "uncaught error: " << s << std::endl;
					return 3;
				} else {
					std::cerr << "Hrm." << std::endl;
				}
			}
			if (eh.need_lines()) continue;
			if (eh.errors() == 0) break;
			if (std::cin.eof()) return 0;
			return 2;
		}
		if (expr.get()) {

			if (FuncExpr const *fe = expr->as_func_expr()) {
				std::vector<std::pair<std::string, ExprSPtr>> partials = Derivator::partials(*fe);
				std::stringstream ss;
				ss << "(";
				for (size_t i = 0; i < fe->params().size(); ++i)
					if (i) ss << ", " << fe->params().at(i);
					else ss << fe->params().at(i);
				ss << ")";
				std::string paramlist = ss.str();
				std::cout << "Derived " << partials.size() << " partials." << std::endl;
				for (auto const &ep : partials) {
					Simplifier s;
					ExprSPtr sp = s.simplify(*ep.second);
					std::cout << "\t∂" << fe->name() << "/∂" << ep.first << paramlist << " = ";
					print.print(*sp);
					std::cout << std::endl;
				}
			}

			if (!expr->as_empty_expr() && !expr->as_func_expr()) {
				try {
					interval res = e.eval(*expr);
					std::cout << "=> ";
					print.print_interval(res);
					std::cout << std::endl;
				} catch (std::exception &e) {
					std::cout << "Error: " << e.what() << std::endl;
				}
			}

		} else {
			std::cerr << "Error: NULL expr!" << std::endl;
			return 4;
		}
	}
}

int
main(int argc, char *argv[]) {
	int v = 4;
	for (int i = 1; (i < argc) && argv[i][0] == '-'; ++i) {
		switch (argv[i][1]) {
		case 'v': ++v; break;
		default:
			fprintf(stderr, "Unrecognized command: %s\n", argv[i]);
			break;
		}
	}
	return repl(v);
}



