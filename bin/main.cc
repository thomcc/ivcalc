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
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Support/Debug.h>
#include <llvm/Assembly/AssemblyAnnotationWriter.h>
#include <llvm/Support/CommandLine.h>
//#include <llvm/Support/Streams.h>
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
using namespace llvm;
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

template <typename TimeUnit=chrono::milliseconds, typename PartialC>
unsigned long long run_benchmark(PartialC &pc) {
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
	std::cout << "ran " << partial_iterations << " iterations in " << timer.get_time<chrono::milliseconds>() << "ms" << endl;
	return timer.get_time<TimeUnit>();
}

void parse_cmd(string const &src, int &verbose, bool &benchmark, bool &codegen, bool &partials, bool &jit, bool &opt) {
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
	case 'v': {
		int v = 0;
		istringstream is(src.substr(2));
		if (is >> v) {
			verbose = std::min(std::max(v, 0), 3);
		} else if (verbose) verbose = 0;
		else verbose++;
		verbose = std::min(std::max(verbose, 0), 3);
		cout << "Verbiosity: " << verbose << endl;
		break;
	}
	case 'i': {
		int i = 0;
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
		cout << "  !v [0-3]  -- change verbose mode" << endl;
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
                int verbose,
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



int repl(int verbose, bool opt, bool benchmark, bool codegen, bool emit_partials, bool jit) {

	cout << endl;
	cout << "Interval arithmetic evaluator/compiler." << endl;
	cout << "Press Ctrl+C to exit" << endl;
	cout << "enter `!h` for options" << endl;
	cout << endl;

	Evaluator e;
	Simplifier s;
	Compiler c;//*c = Compiler::get();
	Printer print(cout, true);
	c.set_verbose(verbose);
	c.set_optimizing(opt);
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
				int v = c.verbiosity();
				parse_cmd(src, v, benchmark, codegen, emit_partials, jit, opt);
				c.set_verbose(v);
				c.set_optimizing(opt);
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
		int ret = handle_expr(expr, c.verbiosity(), benchmark, codegen, emit_partials, jit, e, &c, print);
		if (ret) return ret;
	}
}


int handle_expr(string const &expr_src, int vb, bool opt, bool bm, bool cg, bool part, bool jit) {
	Compiler c;// = Compiler::get();
	c.set_verbose(vb);
	c.set_optimizing(opt);
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
	} catch (exception const &e) {
		cerr << "Caught Error: " << e.what() << endl;
		return 1;
	}
	if (eh.errors() != 0) return eh.errors();

	return handle_expr(expr, vb, bm, cg, part, jit, e, &c, print);
}

string millistring(unsigned long long ctime) {
	stringstream ss;
	int milli = ctime;
	if (ctime > 1000) {
		int s = ctime / 1000;
		milli = ctime % 1000;
		if (s > 60) {
			int m = s / 60;
			s = s % 60;
			if (m > 60) {
				int h = m / 60;
				m = m % 60;
				ss << h << "hr ";
			}
			ss << m << "min ";
		}
		ss << s << "s ";
	}
	ss << milli << "ms";
	return ss.str();
}

string microstring(unsigned long long ctime) {
	stringstream ss;
	int micro = ctime;
	if (micro > 1000) {
		ss << millistring(micro / 1000) << " ";
		micro = micro % 1000;
	}
	ss << micro << "us";
	return ss.str();
}
string nanostring(unsigned long long ctime) {
	stringstream ss;
	int nano = ctime;
	if (nano > 1000) {
		ss << microstring(nano / 1000) << " ";
		nano = nano % 1000;
	}
	ss << nano << "ns";
	return ss.str();
}

int benchcompare(string const &expr_src, int vb, bool opt, bool cg) {
	Compiler c;
	c.set_verbose(vb);
	c.set_optimizing(opt);
	Evaluator e;
	Printer print(cout, true);
	ErrorHandler eh(false, false);
	Parser parser(expr_src, eh);
	ExprPtr expr;
	try {
		expr = parser.parse_expression();
		if (vb > 1) { cout << "PARSE: "; print.print(*expr); cout << endl; }
	} catch (exception const &e) {
		cerr << "Caught: " << e.what() << endl;
	}
	if (eh.errors() != 0) return eh.errors();
	if (!expr.get()) fputs("Got NULL expr!\n", stderr);
	if (FuncExpr *fe = expr->as_func_expr()) {
		Timer compiler_timer, eval_timer;
		string prefix = cg ? "; " : "";
		cerr << prefix << "Compiling... " << flush;
		compiler_timer.start();
		PartialComp pcomp(&c, *fe); // todo: why does this want a pointer?
		compiler_timer.stop();
		cerr << compiler_timer.get_time<chrono::milliseconds>() << "ms" << endl;

		cerr << prefix << "Preparing evaluator... " << flush;
		eval_timer.start();
		PartialCalc pcalc(*fe, e); // todo: fix argument order differences
		eval_timer.stop();
		cerr << eval_timer.get_time<chrono::milliseconds>() << "ms" << endl;



		unsigned long long compiled_runtime = run_benchmark<chrono::microseconds>(pcomp);
		unsigned long long evalled_runtime = run_benchmark<chrono::microseconds>(pcalc);
		unsigned long long compile_time = compiler_timer.get_time<chrono::microseconds>();
		unsigned long long eval_prep_time = eval_timer.get_time<chrono::microseconds>();
		cerr << endl << prefix << "Benchmark results" << endl;
		cerr << prefix << "iterations:             " << partial_iterations << endl;
		cerr << prefix << "number of partials:     " << pcalc.partial_count() << endl;
		cerr << prefix << "compile time:           " << microstring(compile_time) << endl;
		cerr << prefix << "eval prep time:         " << microstring(eval_prep_time) << endl;
		cerr << prefix << "  fraction:             " << (double(compile_time) / double(eval_prep_time)) << endl;
		cerr << prefix << "compiled code runtime:  " << microstring(compiled_runtime) << endl;
		cerr << prefix << "evaluated code runtime: " << microstring(evalled_runtime) << endl;
		cerr << prefix << "  fraction:             " << (double(compiled_runtime) / double(evalled_runtime)) << endl;
		cerr << prefix << "compile total:          " << microstring(compiled_runtime + compile_time) << endl;
		cerr << prefix << "evalled total:          " << microstring(evalled_runtime + eval_prep_time) << endl;
		cerr << prefix << "  fraction:             " << (double(compiled_runtime + compile_time) / double(evalled_runtime + eval_prep_time)) << endl;
		cerr << endl;
		AssemblyAnnotationWriter writer;
		raw_os_ostream rawstrm(cout);
		if (cg) c.module()->print(rawstrm, &writer);
		exit(0);
	}
	cerr << "Error, expected function expression" << endl;
	exit(1);
}

int main(int argc, char *argv[]) {
	FlagSet fset(argc, argv);
	// if I had known llvm::cl existed, and was configurable,
	// I wouldn't have used FlagSet, but since that's not the case,
	// I'm not sure how to implement these options that way...
	std::string empty = "";

	int verbose = 0;
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
	bool v = false;
	bool no_optimize = false;
	bool bench_compare = false;

	fset.Int(verbose, "verbose", "print extra info.");
	fset.Bool(v, "v", "same as -verbose=1");
	fset.Bool(no_optimize, "no-opt", "control optimizations for the JIT compiler");
	fset.Bool(codegen, "codegen", "emit generated llvm-ir?");
	fset.Bool(emit_partials, "partials", "emit llvm-ir for partial derivatives?");
	fset.Bool(interpret, "interpret", "Interpret expressions instead of using the JIT");
	fset.Bool(simplify, "simplify", "Simplify expressions.");
	fset.Bool(dobench, "bench", "run benchmark on partials for expressions?");
	fset.Bool(bench_compare, "bench-both", "bench both JIT and interpreter. Not available with -repl");
	fset.Int(bench, "iter", "number of benchmark iterations. Implies -b");
	fset.Bool(repl, "repl", "use repl? Incompatible with -file, -expr, -stdin");
	fset.String(file, "file", "read from file. Incompatible with -repl, -expr, -stdin");
	fset.String(expr, "expr", "evaluate expr. Incompatible with -repl, -file, -stdin");
	fset.Bool(stdin, "stdin", "read from stdin? Incompatible with -repl, -file, -expr");
 	Error err = fset.Parse();
 	if (err.bad()) exit(2);
 	if (v) verbose = 1;
 	if (bench != 1000) dobench = true;
 	if (bench <= 0) {
 		fset.Complain(stringize() << "-iter must be >= 0. got: " << bench);
 		exit(2);
 	}

 	if (!repl && !stdin && file == empty && expr == empty)
 		repl = true;
 	if (repl && bench_compare) {
 		fset.Complain("-bench-compare is not available from -repl mode.");
 		exit(2);
 	}
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
//	FakeArgs fa = fset.Remaining();
//	cl::ParseCommandLineOptions(fa.argc, fa.argv);

	if (dobench) partial_iterations = bench;
	try {
		if (bench_compare)
			return benchcompare(expr_str, verbose, no_optimize, codegen);
		if (use_str)
			return handle_expr(expr_str, verbose, !no_optimize, dobench, codegen, emit_partials, !interpret);
		else //repl(args.verbose, args.benchmark, args.compile, args.compile_partials, args.use_jit
			return ::repl(verbose, !no_optimize, dobench, codegen, emit_partials, !interpret);
	} catch (std::exception const &e) {
		cerr << "Uncaught exception: " << e.what() << endl;
	} catch (std::string const &e) {
		cerr << "Uncaught exception: " << e << endl;
	} catch (...) {
		cerr << "Uncaught exception: (source unknown)." << endl;
		throw; // so we can pick it up in the debugger.
	}
}



