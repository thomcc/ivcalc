#include "common.hh"
#include "interval.hh"
#include "parser/parser.hh"
#include "visitors/printer.hh"
#include "visitors/simplifier.hh"
#include "visitors/eval.hh"
#include "visitors/derivator.hh"
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
	while (!cin.eof()) {
		string l;
		cout << (continued ? "| " : "> ");
		getline(cin, l);
		line += l;
		if ((line.size() > 0) && is_balanced(line.c_str()))
			break;
		else continued = true;
	}
	return line;
}

int repl(int vrb) {

	cout << endl;
	cout << "interval calc" << endl;
	cout << "Press Ctrl+C to exit" << endl;
	cout << endl;

	Evaluator e;
	Simplifier s;
	Printer print(cout, true);
	map<string, PartialCalc> known_partials;
	for (;;) {
		string src;
		ExprSPtr expr;
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
				if (eh.errors() == 0) {
					cerr << "uncaught error: " << s << endl;
					return 3;
				} else {
					cerr << "Hrm." << endl;
				}
			}
			if (eh.need_lines()) continue;
			if (eh.errors() == 0) break;
			else continue;
			if (cin.eof()) return 0;
			return 2;
		}
		if (expr.get()) {
			if (FuncExpr const *fe = expr->as_func_expr()) {
				PartialCalc pc(*fe, e);
				cout << "Derived " << pc.partials().size() << " partials." << endl;
				for (auto const &fexpr : pc.partials()) {
					cout << "\t";
					print.print(*fexpr);
					cout << endl;
				}
				e.eval(*expr);
				known_partials[fe->name()] = pc;
			}
			bool did_error = false;
			interval res;
			if (!expr->as_empty_expr() && !expr->as_func_expr()) {
				try {
					res = e.eval(*expr);
					cout << "=> ";
					print.print_interval(res);
					cout << endl;
				} catch (exception &e) {
					cout << "Error: " << e.what() << endl;
					did_error = true;
				}
			}
			if (!did_error) {
				if (CallExpr const *ce = expr->as_call_expr()) {
					auto it = known_partials.find(ce->name());
					if (it != known_partials.end()) {
						PartialCalc &pcalc = it->second;
						vector<ExprSPtr> const &part = pcalc.partials();
						vector<ExprSPtr> const &args = ce->args();
						cout << "Calculating partials..." << endl;
						vector<interval> ivs = pcalc.calculate(args);
						cout << "\t";
						print.print(*expr);
						cout << " => ";
						print.print_interval(res);
						cout << endl;
						for (size_t i = 0; i < ivs.size(); ++i) {
							cout << "\t";
							FuncExpr const *fexpr = part.at(i)->as_func_expr();
							print.print(*Expr::make<CallExpr>(fexpr->name(), args));
							cout << " => ";
							print.print_interval(ivs.at(i));
							cout << endl;
						}
					}
				}
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



