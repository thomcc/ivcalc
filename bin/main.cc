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

		if (benchmark) {			if (FuncExpr *fe = expr->as_func_expr()) {
				Timer t;
				t.start();
				PartialComp pcomp(c, *fe);
				t.stop();
				PartialCalc pcalc(*fe, e);

				cout << (jit ? "Derived and compiled " : "Derived ") << pcalc.partial_count() << " partials in " << t.get_time<chrono::milliseconds>() <<"ms." << endl;
				if (partials) {
					for (size_t i = 0; i < pcomp.expr_count(); ++i) {
						cout << "\t";
						print.print(*pcomp.partials().at(i));
						cout << endl;
						pcomp.funcs().at(i)->dump();
					}
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
						pc.funcs().at(i)->dump();
					}
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
	cout << "Interval compile." << endl;
	cout << "Press Ctrl+C to exit" << endl;
	cout << "enter `!h` for options" << endl;
	cout << endl;

	Evaluator e;
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
			ErrorHandler eh;
			ltrim(src);
			if (src[0] == '!') {
				bool opt = c.is_optimizing();
				parse_cmd(src, verbose, benchmark, codegen, emit_partials, jit, opt);
				c.set_optimizing(opt);
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

	Evaluator e;
	Compiler c;
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

Args::Args(int argc, char **argv) {
	verbose = benchmark = compile = compile_partials = false;
	bench_iters = 1000;
	// convert to vector of strings, check for conflicts.
	struct { bool v, b, c, p, r, e, f, s, j, I; } flags = { 0 };

	string fname;
	string expr_str;
	for (argc--, argv++; argc > 0; argc--, argv++) {
		char *item = argv[0] +1;
		switch (*item++) {
		case 'b':
			args.push_back("--bench 1000");
			if (flags.b)
				printf("%s: warning: -b or --bench repeated.\n", program_name.c_str());
			flags.b = true;
			break;
		case 'h':
			usage(EXIT_SUCCESS);
			break;
		case 'k':
			args.push_back("--jit");
			if (flags.j)
				printf("%s: warning: -j or --jit repeated.\n", program_name.c_str());
		case 'v':
verbose_flag:
			args.push_back("--verbose");
			if (flags.v)
				printf("%s: warning: -v or --verbose repeated.\n", program_name.c_str());
			flags.v = true;
			break;
		case 'c':
codegen_flag:
			args.push_back("--codegen");
			if (flags.c)
				printf("%s: warning: -c or --codegen repeated.\n", program_name.c_str());
			flags.c = true;
			break;
		case 'p':
partials_flag:
			args.push_back("--emit-partials");
			if (flags.p)
				printf("%s: warning: -p or --emit-partials repeated.\n", program_name.c_str());
			flags.p = true;
			break;
		case 'f':
file_flag:
			if (flags.f)
				printf("%s: warning: -f or --file repeated.\n", program_name.c_str());
			if (argc > 1) {
				++argv; --argc;
				flags.f = true;
				fname = string(*argv);
				args.push_back(string("--file")+fname);
			} else {
				printf("%s: no file given for -f\n", program_name.c_str());
				exit(-1);
			}
			if (flags.e || flags.r || flags.s) {
				printf("%s: conflicting options: only one of -[refs] may be specified\n", program_name.c_str());
				usage();
			}
			break;
		case 'e':
expr_flag:
			if (flags.e)
				printf("%s: warning: -e or --expr repeated.\n", program_name.c_str());
			if (argc > 1) {
				++argv; --argc;
				flags.e = true;
				expr_str = string(*argv);
				args.push_back(string("--expr ")+expr_str);
			} else {
				printf("%s: no expression given for -e\n", program_name.c_str());
				exit(-1);
			}
			if (flags.f || flags.r || flags.s) {
				printf("%s: conflicting options: only one of -[refs] may be specified\n", program_name.c_str());
				usage();
			}
			break;
		case 's':
stdin_flag:
			if (flags.s)
				printf("%s: warning: -s or --stdin repeated.\n", program_name.c_str());
			flags.s = true;
			args.push_back("--stdin");
			if (flags.e || flags.r || flags.f) {
				printf("%s: conflicting options: only one of -[refs] may be specified\n", program_name.c_str());
				usage();
			}
			break;
		case 'r':
repl_flag:
			if (flags.r)
				printf("%s: warning: -r or --repl repeated.\n", program_name.c_str());
			flags.r = true;
			args.push_back("--repl");
			if (flags.e || flags.r || flags.f) {
				printf("%s: conflicting options: only one of -[refs] may be specified\n", program_name.c_str());
				usage();
			}
			break;
		case 'j':
jit_flag:
			if (flags.j)
				printf("%s: warning: -j or --jit repeated.\n", program_name.c_str());
			if (flags.I) {
				printf("%s: error: -I (--interpret) and -j (--jit) are incompatible\n", program_name.c_str());
				usage();
			}
			flags.j = true;
			args.push_back("--jit");
			break;
		case 'I':
interp_flag:
			if (flags.I)
				printf("%s: warning -I or --interpret repeated.\n", program_name.c_str());
			if (flags.j) {
				printf("%s: error: -I (--interpret) and -j (--jit) are incompatible\n", program_name.c_str());
				usage();
			}
			flags.I = true;
			args.push_back("--interpret");
			break;
		case '-':
			if (strcmp((*argv)+2, "bench") == 0) {
				if (argc > 1) {
					++argv; --argc;
					if (flags.b)
						printf("%s: warning: -b or --bench repeated.\n", program_name.c_str());
					flags.b = true;
					string msg = string(*argv);
					args.push_back(string("--bench ")+msg);
					istringstream ss(msg);
					if (!(ss >> bench_iters)) {
						printf("%s: bad number specified for --bench: %s\n", program_name.c_str(), msg.c_str());
						exit(-1);
					}
				} else {
					printf("%s: no number specified for --bench\n", program_name.c_str());
					exit(-1);
				}
				break;
			} else if (strcmp((*argv)+2, "help") == 0) usage(EXIT_SUCCESS);
			else if (strcmp((*argv)+2, "verbose") == 0) goto verbose_flag;
			else if (strcmp((*argv)+2, "codegen") == 0) goto codegen_flag;
			else if (strcmp((*argv)+2, "emit-partials") == 0) goto partials_flag;
			else if (strcmp((*argv)+2, "repl") == 0) goto repl_flag;
			else if (strcmp((*argv)+2, "expr") == 0) goto expr_flag;
			else if (strcmp((*argv)+2, "file") == 0) goto file_flag;
			else if (strcmp((*argv)+2, "stdin") == 0) goto stdin_flag;
			else if (strcmp((*argv)+2, "jit") == 0) goto jit_flag;
			else if (strcmp((*argv)+2, "interpret") == 0) goto interp_flag;
		default: // fallthrough
			printf("%s: unknown argument: %s\n", program_name.c_str(), *argv);
			usage();
			break;
		}
	}
	if (flags.p) flags.c = true;

	if (flags.I) use_jit = false;
	else use_jit = true; // hrm.

	verbose = flags.v;
	benchmark = flags.b;
	compile = flags.c;
//	use_jit = flags.j;

	compile_partials = flags.p;

	if (!(flags.e || flags.f || flags.s || flags.r))
		flags.r = true;

	if (flags.r) {
		assert(!(flags.e || flags.s || flags.f));
		from_str = false;
	} else if (flags.e) {
		assert(!(flags.f || flags.s || flags.r));
		from_str = true;
		// e_expr already has the text
	} else if (flags.f) {
		assert(!(flags.e || flags.s || flags.r));
		ifstream file(fname);
		if (!file.is_open()) {
			printf("%s: Cannot open file (%s)\n", program_name.c_str(), fname.c_str());
			exit(1);
		}
		try {
			e_expr = (static_cast<stringstream const&>(stringstream() << file.rdbuf()).str());
		} catch (std::exception e) {
			printf("%s: Error reading file (%s)\n", program_name.c_str(), fname.c_str());
			exit(1);
		}
		from_str = true;
	} else if (flags.s) {
		assert(!(flags.e || flags.f || flags.r));
		string line;
		while (getline(cin, line)) e_expr += " "+line;
		from_str = true;
	} else {
		printf("%s: bug: error parsing arguments. read:\n", program_name.c_str());
		for (auto const &arg : args) printf("  %s\n", arg.c_str());
	}
}


void Args::usage(int exit_with) {
	static const char *const usage_msg[] = {
		"switches:",
		"  -v              set verbose mode",
		"  -b              benchmark partials for functions",
		"  -c              emit llvm ir for all expressions",
		"  -p              also emit llvm ir for partial derivatives of functions. implies -c.",
		"  -j              use the JIT instead of interpreter",
		"  -h              print this message and quit",
		"  --verbose       set verbose mode",
		"  --bench <uint>  run <uint> benchmarks. (-b is equivalent to --bench 1000)",
		"  --codegen       emit llvm ir",
		"  --emit-partials emit llvm ir for partial derivatives of functions.",
		"  --jit           use the JIT instead of the interpreter",
		"  --help          print this message and quit",
		"modes:",
		"  -r              run interactive prompt (default)",
		"  -e 'expr'       run on expression 'expr'",
		"  -f 'file'       run on the file 'file'",
		"  -s              run on stdin",
		"  --repl          run interactive prompt",
		"  --expr 'expr'   run on expression 'expr'",
		"  --file 'file'   run on file 'file'",
		"  --stdin         run on stdin.",
		nullptr
	};
	const char *const *p = usage_msg;
	printf("Usage: %s [switches] [one mode]\n", program_name.c_str());
	while (*p) printf("  %s\n", *p++);
	exit(exit_with);
}

int main(int argc, char *argv[]) {
	Args args(argc, argv);
	partial_iterations = args.bench_iters;
	if (args.from_str) return handle_expr(args.e_expr, args.verbose, args.benchmark, args.compile, args.compile_partials, args.use_jit);
	else return repl(args.verbose, args.benchmark, args.compile, args.compile_partials, args.use_jit);
}



