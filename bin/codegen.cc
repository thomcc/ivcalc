#include "common.hh"
#include "interval.hh"
#include "parser/parser.hh"
#include "visitors/printer.hh"
#include "visitors/simplifier.hh"
#include "visitors/eval.hh"
#include "visitors/derivator.hh"
#include "visitors/compiler.hh"
#include <cstdio>
#include <cstdlib>
#define LINE_LEN 4096

using namespace calc;
using namespace std;

static bool is_balanced(const char *s) {
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

string get_line(bool continued){
	string line;
	while (cin) {
		string l;
		cout << (continued ? "| " : "> ");
		if (getline(cin, l)) {
			line += l;
			if ((line.size() > 0) && is_balanced(line.c_str()))
				break;
			else continued = true;
		} else break;
	}
	return line;
}

int repl(int vrb) {

	cout << endl;
	cout << "interval codegen tester" << endl;
	cout << "Press Ctrl+C to exit" << endl;
	cout << endl;

	Simplifier s;
	Compiler c;
	Printer print(cout, true);
	for (;;) {
		string src;
		ExprPtr expr(nullptr);
		for (;;) {
			bool cont = src.size() != 0;
			string line = get_line(cont);
			if (cont) src += "\n";
			src += line;
			trim(src);

			ErrorHandler eh;
			Parser parser(src, eh);
			try {
				expr = parser.parse_expression();
			} catch (string s) {
				cerr << "uncaught error: " << s << endl;
				return 3;
			} catch (exception e) {
				cerr << "uncaught error: " << e.what() << endl;
				return 3;
			}
			if (eh.need_lines()) continue;
			if (eh.errors() == 0) break;
			else {src = ""; continue;}
			if (cin.eof()) return 0;
			return 2;
		}
		if (expr.get()) {
			if (FuncExpr const *fe = expr->as_func_expr()) {
				llvm::Function *f = nullptr;
				try {
					f = c.compile_func(*fe);
				} catch (exception e) {
					cerr << "Uncaught error: " << e.what() << endl;
					return 3;
				}
				if (!f) {
					cerr << "error: compiler returned nullptr when compiling function. " << endl;
					return 1;
				}
				cout << "read function: " << endl;
				f->dump();
			}
		} else {
			cerr << "Error: NULL expr!" << endl;
			return 4;
		}
	}
}

int main(int argc, char *argv[]) {
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



