#include "common.hh"
#include "interval.hh"
#include "parser/parser.hh"
#include "printer.hh"
#include "simplifier.hh"
#include "eval.hh"
#include "derivator.hh"
#include <cstdio>
#include <cstdlib>
#include <random>
#include <chrono>
#include <functional>

using namespace std;
using namespace calc;



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

uniform_real_distribution<real> unif_real(-1000, 1000);
random_device rd;
mt19937 e(rd());
function<real()> next_real = bind(unif_real, e);

void fill_randomly(vector<ExprSPtr> &v, size_t n) {
	for (size_t i = 0; i < n; ++i)
		v[i] = Expr::make<LitExpr>(interval(next_real()));
}

class Timer {
	typedef chrono::high_resolution_clock clock;
	typedef chrono::nanoseconds time_unit;
	typedef clock::time_point time_point;
	time_unit _runtime;
	time_point _last;
public:
	Timer() : _runtime(0) {}
	void start() { _last = clock::now(); }
	void stop() {
		time_point stop = clock::now();
		_runtime += stop - _last;
		_last = clock::now();
	}
	unsigned long long nanosecs() const { return _runtime.count(); }
	time_unit runtime() const { return _runtime; }
	template <typename T> unsigned long long get_time() const {
		return chrono::duration_cast<T>(_runtime).count();
	}
};

unsigned partial_iterations = 1000;

void benchmark(PartialCalc &pc) {
	using namespace chrono;
	size_t nargs = pc.params().size();
	vector<ExprSPtr> args(nargs);
	Timer timer;
	for (size_t i = 0; i < partial_iterations; ++i) {
		fill_randomly(args, nargs);
		timer.start();
		pc.calculate(args);
		timer.stop();
	}
	std::cout << "ran " << partial_iterations << " iterations in " << timer.get_time<milliseconds>() << "ms" << endl;
}

int repl(int vrb) {

	cout << endl;
	cout << "Benchmarking expressions." << endl;
	cout << "Press Ctrl+C to exit" << endl;
	cout << "Enter an expression or `!i <number of iterations>` " << endl;
	cout << "to change the number of benchmarking iterations (default: " << partial_iterations << "):" << endl;
	cout << endl;

	Evaluator e;
	Simplifier s;
	Printer print(cout, true);
	for (;;) {
		string src;
		ExprSPtr expr;
		PartialCalc pcalc;
		for (;;) {
			bool cont = src.size() != 0;
			string line = get_line(cont);
			if (cont) src += "\n";
			src += line;
			ErrorHandler eh;
			trim(src);
			if (src[0] == '!') {
				switch(src[1]) {
				case 'i': {
					int i;
					istringstream is(src.substr(2));
					is >> i;
					if (i > 0) {
						partial_iterations = i;
						cout << "Iteration count changed to " << partial_iterations << endl;
					} else cout << "Error: failed reading iteration number" << endl;
					break;
				}
				default:
					cout << src[1] << ": Unknown command" << endl;
					break;
				}
				src = "";
				continue;
			}
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
				Timer t;
				t.start();
				PartialCalc pc(*fe, e);
				t.stop();
				cout << "Derived " << pc.partial_count() << " partials in " << t.get_time<chrono::milliseconds>() <<"ms." << endl;
				for (auto const &fexpr : pc.partials()) {
					cout << "\t";
					print.print(*fexpr);
					cout << endl;
				}
				e.eval(*expr);
				pcalc = pc;
			} else {
				cout << "error, enter `<func_name>(<arg0>, <arg1>, ...) = <expr>` or `!i <new iteration count>`" << endl;
				continue;
			}
			cout << endl << "Benchmarking " << partial_iterations << " iterations of computing points on " << pcalc.expr_count() << " expressions..." << endl;
			benchmark(pcalc);
			cout << "Enter another expression, change the number of iterations, or press Ctrl+C to quit" << endl;
		} else {
			cerr << "Error: NULL expr!" << endl;
			return 4;
		}
	}
}

int main(int argc, char *argv[]) {
	return repl(4);
}



