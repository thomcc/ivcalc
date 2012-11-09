#include "common.hh"
#include "interval.hh"
#include "parser/parser.hh"
#include "visitors/printer.hh"
#include "visitors/simplifier.hh"
#include "visitors/eval.hh"
#include "visitors/derivator.hh"
#include <cstdio>
#include <cstdlib>
#include <random>
#include <chrono>
#include <functional>
#include <csignal>
#include <fstream>
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

uniform_real_distribution<real> unif_real(-1000, 1000);
random_device rd;
mt19937 e(rd());
function<real()> next_real = bind(unif_real, e);

void fill_randomly(vector<ExprPtr> &v, size_t n) {
	for (size_t i = 0; i < n; ++i)
		v[i] = Expr::make_lit(interval(next_real()));
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
	vector<ExprPtr> args(nargs);
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
		ExprPtr expr(nullptr);
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
				case 's':
					Expr::stats();
					break;
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
			else { src = ""; continue; }
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
int do_bench(string expr_src) {
	ErrorHandler eh(false, false);
	Parser parser(expr_src, eh);
	Printer print(cout, true);
	Evaluator e;
	PartialCalc pcalc;
	ExprPtr expr;
	try {
		expr = parser.parse_expression();
	} catch (string s) {
		cerr << "Caught Error: " << s << endl;
		return 1;
	} catch (exception e) {
		cerr << "Caught Error: " << e.what() << endl;
		return 1;
	}
	if (eh.errors() != 0) return eh.errors();
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
		cerr << "error, expected function expression: `<func_name>(<arg0>, <arg1>, ...) = <expr>`" << endl;
		return 1;
	}
	cout << endl << "Benchmarking " << partial_iterations << " iterations of computing points on " << pcalc.expr_count() << " expressions..." << endl;
	benchmark(pcalc);
	return 0;
}

void usage() {
	cerr << "usage: -r: enter interactive prompt (default). if used explicitly, incompatible with -[ef]" << endl;
	cerr << "usage: -i <unsigned integer>: set partial iteration count" << endl;
	cerr << "usage: -f <filename>: benchmark the expression in <filename>. incompatible with -[re]" << endl;
	cerr << "usage: -e \'func(arg, ...) = <expr>\': benchmark partial evaluation of func. incompatible with -[rf]" << endl;
	exit(1);
}

int bench_file(string fn) {
	ifstream file(fn);
	if (!file.is_open()) {
		cerr << "Couldn't open file: " << fn << endl;
		return 1;
	}
	string dest;
	try {
		dest = (static_cast<stringstream const&>(stringstream() << file.rdbuf()).str());
	} catch (std::exception e) {
		cerr << "Error reading file '" << fn << "': " << e.what() << endl;
		return 1;
	}
	return do_bench(dest);
}

int main(int argc, char *argv[]) {
	string bench_str;
	string file_str;
	bool should_bench = false;
	bool should_repl = false;
	bool should_read_file = false;
	for (int i = 1; (i < argc) && argv[i][0] == '-'; ++i) {
		switch (argv[i][1]) {
		case 'e':
			bench_str = argv[++i];
			should_bench = true;
			break;
		case 'i': {
			istringstream is{string(argv[++i])};
			if (!(is >> partial_iterations)) {
				cerr << "Invalid number (must be unsigned integer): " << argv[i-1];
				return 1;
			}
			break;
		}
		case 'r':
			should_repl = true;
			break;
		case 'f':
			file_str = argv[++i];
			should_read_file = true;
			break;
		default:
			cerr << "bench: illegal option -- " << argv[i] << endl;
			usage();
			break;
		}
	}
	if ((should_bench && should_repl) || (should_bench && should_read_file) || (should_repl && should_read_file)) {
		cerr << "bench: conflicting options. got more than one of -[ref]" << endl;
		usage();
	} else if (should_bench) return do_bench(bench_str);
	else if (should_repl) return repl(4);
	else if (should_read_file) return bench_file(file_str);
	return 2;
}



