#include "common.hh"
#include "interval.hh"
#include "parser/parser.hh"
#include "visitors/printer.hh"
#include "visitors/simplifier.hh"
#include "visitors/eval.hh"
#include "visitors/derivator.hh"
#include "visitors/compiler.hh"
#include "frontend/flagparser.hh"
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Debug.h>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <chrono>
#include <functional>
#include <csignal>
#include <fstream>

// this is a huge mess. its really not worth even looking at.
using namespace std;
using namespace calc;
using namespace flag;

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

void fill_randomly(vector<interval> &v, size_t n) {
	for (size_t i = 0; i < n; ++i)
		v[i] = interval(next_real());
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

template <typename PartialC>
void run_benchmark(PartialC &pc) {
	using namespace chrono;
	size_t nargs = pc.params().size();
	vector<interval> args(nargs);
	Timer timer;
	for (size_t i = 0; i < partial_iterations; ++i) {
		fill_randomly(args, nargs);
		timer.start();
		pc.calculate(args);
		timer.stop();
	}
	std::cout << "ran " << partial_iterations << " iterations in " << timer.get_time<milliseconds>() << "ms" << endl;
}

void parse_cmd(string const &src, bool &verbose, bool &benchmark, bool &codegen, bool &partials, bool &jit, bool &opt) {
	switch(src[1]) {
	case 'b':
		benchmark = !benchmark;
		cout << "Benchmarking? " << boolalpha << benchmark << endl;
		break;
	case 'c':
		codegen = !codegen;
		cout << "Printing LLVM IR? " << boolalpha << codegen << endl;
		break;
	case 'p':
		partials = !partials;
		if (partials) codegen = true;
		cout << "Printing LLVM IR for expressions and partials? " << boolalpha << partials << endl;
		break;
	case 'v':
		verbose = !verbose;
		cout << "Verbose? " << boolalpha << verbose << endl;
		break;
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
	case 'j':
		jit = !jit;
		cout << "Use JIT? " << boolalpha << jit << endl;
		break;
	case 'o':
		opt = !opt;
		cout << "optimizations on? " << boolalpha << opt << endl;
		break;
	case 'q':
		exit(0);
		break;
	case 'h':
		cout << "Recognized commands: " << endl;
		cout << "  !h        -- print this message" << endl;
		cout << "  !b        -- toggle benchmarking" << endl;
		cout << "  !j        -- use JIT instead of interpreter" << endl;
		cout << "  !c        -- toggle printing llvm ir" << endl;
		cout << "  !o        -- toggle whether or not the JIT compiler performs optimizations" << endl;
		cout << "  !p        -- toggle printing of code for partials (forces !c when true)" << endl;
		cout << "  !v        -- toggle verbose mode" << endl;
		cout << "  !i <uint> -- set benchmark iterations to <uint>" << endl;
		cout << "  !q        -- quit." << endl;
		cout << endl;
		break;
	default:
		cout << "Unknown command: " << src << endl;
		break;
	}
}

int handle_expr(ExprPtr const &expr,
                bool verbose,
                bool benchmark,
                bool codegen,
                bool partials,
                bool jit,
                Evaluator &e,
                Compiler *c,
                Printer &print) {
	if (expr.get()) {
		if (verbose) {
			cout << "Parsed: ";
			print.print(*expr);
			cout << endl;
		}
		bool did_codegen = false;

		interval i = e.eval(*expr);

		if (!i.is_empty()) {
			cout << "eval: ";
			print.print_interval(i);
			cout << endl;
		}

		interval i2 = c->execute(expr);

		if (!i2.is_empty()) {
			cout << "jit: ";
			print.print_interval(i2);
			cout << endl;
		}

		if (benchmark) {
			if (FuncExpr *fe = expr->as_func_expr()) {
				Timer t;
				PartialComp pcomp;
				t.start();
				if (jit) {
					puts("compiling... (somewhat slow)");
					pcomp = PartialComp(c, *fe);
				}
				PartialCalc pcalc(*fe, e);
				t.stop();


				cout << (jit ? "Derived and compiled " : "Derived ") << pcalc.partial_count() << " partials in " << t.get_time<chrono::milliseconds>() <<"ms." << endl;
				if (partials && jit) {
					for (size_t i = 0; i < pcomp.expr_count(); ++i) {
						cout << "\t";
						print.print(*pcomp.partials().at(i));
						cout << endl;
//						cout.flush();
//						pcomp.funcs().at(i)->dump();
//						cout.flush();
//						llvm::dbgs().flush();
					}
					pcomp.fn()->dump();

					did_codegen = true;
				} else {
					for (auto const &fexpr : pcalc.partials()) {
						cout << "\t";
						print.print(*fexpr);
						cout << endl;
					}
				}

				cout << endl << "Benchmarking " << (jit ? "(jit) " : "(eval) ") << partial_iterations << " iterations of computing points on " << pcalc.expr_count() << " expressions..." << endl;
				if (jit) run_benchmark(pcomp);
				else run_benchmark(pcalc);
			}
		}

		if (!did_codegen) {
			llvm::Function *f = nullptr;
			if (!partials && codegen) {
				try {
					f = c->compile_expr(expr);
				} catch (exception e) {
					cerr << "Error during compilation: " << e.what() << endl;
					return 1;
				}
				if (!f) cerr << "Compiler returned nullptr when compiling function" << endl;
				f->dump();
			} else if (partials) {
				if (FuncExpr *fe = expr->as_func_expr()) {
					PartialComp pc(c, *fe);
					for (size_t i = 0; i < pc.expr_count(); ++i) {
						print.print(*pc.partials().at(i));
						cout.flush();
//						pc.funcs().at(i)->dump();
//						llvm::dbgs().flush();
					}
					pc.fn()->dump();
				}
			}
		}
		return 0;
	} else {
		cerr << "Error: NULL expr!" << endl;
		return 4;
	}
}



int repl(bool verbose, bool benchmark, bool codegen, bool emit_partials, bool jit) {

	cout << endl;
	cout << "Interval arithmetic evaluator/compiler." << endl;
	cout << "Press Ctrl+C to exit" << endl;
	cout << "enter `!h` for options" << endl;
	cout << endl;

	Evaluator e;
	Simplifier s;
	Compiler c;//*c = Compiler::get();
	Printer print(cout, true);

	for (;;) {
		string src;
		ExprPtr expr(nullptr);
		for (;;) {
			bool cont = src.size() != 0;
			string line = get_line(cont);
			if (cont) src += "\n";
			src += line;
			ErrorHandler eh;
			ltrim(src);
			if (src[0] == '!') {
				bool opt = c.is_optimizing();//c->is_optimizing();
				parse_cmd(src, verbose, benchmark, codegen, emit_partials, jit, opt);
				c.set_optimizing();
				//c->set_optimizing(opt);
				src = "";
				continue;
			}
			Parser parser(src, eh);
			try {
				expr = parser.parse_expression();
			} catch (exception e) {
				cerr << "Error: " << e.what() << endl;
				return 3;
			}
			if (eh.need_lines()) continue;
			if (eh.errors() == 0) break;
			else { src = ""; continue; }
			if (cin.eof()) return 0;
			return 2;
		}
		int ret = handle_expr(expr, verbose, benchmark, codegen, emit_partials, jit, e, &c, print);
		if (ret) return ret;
	}
}


int handle_expr(string const &expr_src, bool vb, bool bm, bool cg, bool part, bool jit) {
	Compiler c;// = Compiler::get();
	Evaluator e;
	Printer print(cout, true);
	ErrorHandler eh(false, false);
	Parser parser(expr_src, eh);
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

	return handle_expr(expr, vb, bm, cg, part, jit, e, &c, print);
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
	run_benchmark(pcalc);
	return 0;
}

struct Args {

	Args(int argc, char **argv);
	void usage(int exit_with=1);

	vector<string> args;

	string program_name;
	string e_expr;

	bool from_str;

	bool verbose;
	bool benchmark;
	bool compile;
	bool compile_partials;
	bool use_jit;

	unsigned bench_iters;


};

int main(int argc, char *argv[]) {
	FlagSet fset(argc, argv);

	std::string empty = "";

	bool verbose = false;
	bool codegen = false;
	bool simplify = true;
	bool emit_partials = false;
	bool interpret = false;
	bool dobench = false;
	int bench = 1000;
	bool repl = false;
	std::string file = empty;
	std::string expr = empty;
	bool stdin = false;

	fset.Bool(verbose, "verbose", "print extra info (doesn't do much currently)");
	fset.Bool(codegen, "codegen", "emit generated llvm-ir?");
	fset.Bool(emit_partials, "partials", "emit llvm-ir for partial derivatives?");
	fset.Bool(interpret, "interpret", "Interpret expressions instead of using the JIT");
	fset.Bool(simplify, "simplify", "Simplify expressions.");
	fset.Bool(dobench, "bench", "run benchmark on partials for expressions?");
	fset.Int(bench, "iter", "number of benchmark iterations. Implies -b");
	fset.Bool(repl, "repl", "use repl? Incompatible with -file, -expr, -stdin");
	fset.String(file, "file", "read from file. Incompatible with -repl, -expr, -stdin");
	fset.String(expr, "expr", "evaluate expr. Incompatible with -repl, -file, -stdin");
	fset.Bool(stdin, "stdin", "read from stdin? Incompatible with -repl, -file, -expr");
 	Error err = fset.Parse();
 	if (err.bad()) exit(2);

 	if (bench != 1000) dobench = true;
 	if (bench <= 0) {
 		fset.Complain(stringize() << "-iter must be >= 0. got: " << bench);
 		exit(2);
 	}

 	if (!repl && !stdin && file == empty && expr == empty)
 		repl = true;

 	int i = 0;
 	if (repl) ++i;
 	if (stdin) ++i;
 	if (file != empty) ++i;
 	if (expr != empty) ++i;
 	if (i != 1) {
		fset.Complain("-repl, -file, -expr, -stdin are mutually exclusive");
		exit(2);
	}

	std::string expr_str = "";
	bool use_str = false;
	if (expr != empty) { expr_str = expr; use_str = true; }
	else if (stdin) {
		std::string line;
		while (getline(cin, line)) expr_str += " "+line;
		use_str = true;
	} else if (file != empty) {
		ifstream infile(file);
		if (!infile.is_open()) {
			fset.Complain("Couldn't open file: "+file);
			exit(3);
		}
		try {
			expr_str = (static_cast<stringstream const&>(stringstream() << infile.rdbuf()).str());
		} catch (...) {
			fset.Complain("Couldn't read from file: "+file);
			exit(3);
		}
		use_str = true;
	}
	if (dobench) partial_iterations = bench;

	if (use_str)
		return handle_expr(expr_str, verbose, dobench, codegen, emit_partials, !interpret);
	else //repl(args.verbose, args.benchmark, args.compile, args.compile_partials, args.use_jit
		return ::repl(verbose, dobench, codegen, emit_partials, !interpret);

}



