#ifndef __COMPILER_HH__
#define __COMPILER_HH__
#include "common.hh"
#include "expr.hh"
#include "llvm/Support/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Support/NoFolder.h"
#include "llvm/ADT/StringMap.h"
#include <map>
#include <unordered_map>

namespace calc {

// utility struct to simulate multiple return values in c++.
// we could use a std::pair or std::tuple, but this is friendlier
// in the debugger and it's used enough to justify its existence
struct VInterval {
	llvm::Value *lo, *hi;
};

inline interval from_ret(pod_interval ri) {
	return interval{ri.lo, ri.hi};
}


// enum to represent the rounding mode.
enum class RoundMode { /* ToNear, ToZero, */ Up, Down, Unknown };

typedef pod_interval(*POD_JITFunc)(void);

// represents a single compiled function returned from the compiler.
// note that this isn't the compiler's interal representation, but
// instead the representation it returns.

class Compiled {
	friend class Compiler;
	std::function<void(double*, double*)> _jitted;
	size_t _nargs;
	std::unique_ptr<double[]> _dbls;
	std::unique_ptr<double[]> _retdbls;
public:
	typedef std::function<void(double*, double*)> FuncType;
	typedef void (*FuncPtrType)(double*, double*);
	~Compiled() {}
	Compiled() : _jitted(nullptr), _nargs(0), _dbls(nullptr), _retdbls(nullptr) {}
	Compiled(size_t n) : _nargs(n), _dbls{new double[n*2]}, _retdbls{new double[(n+1)*2]} {}
	Compiled(Compiled &&o) : Compiled() { using std::swap; swap(*this, o); }
	Compiled(Compiled const &o) : Compiled(o._nargs) {
		_jitted = o._jitted;
		std::copy(o._dbls.get(), o._dbls.get() + 2 * o._nargs, _dbls.get());
		std::copy(o._retdbls.get(), o._retdbls.get() + 2 * (o._nargs+1), _retdbls.get());
	}
//	Compiled &operator=(Compiled &&o) {
//		_dbls = std::move(o._dbls);
//		_retdbls = std::move(o._retdbls);
//		_nargs = o._nargs;
//		_jitted = o._jitted;
//		o._nargs = 0;
//		o._jitted = nullptr;
//		return *this;
//	}
	Compiled &operator=(Compiled o) { using std::swap; swap(*this, o); return *this; }
	interval operator()(interval const &i) {
		std::vector<interval> r = operator()(std::vector<interval>{i});
		if (r.size() == 0) return interval::empty();
		return r[0];
	}
	std::vector<interval> operator()(std::vector<interval> const &);
	friend void swap(Compiled& first, Compiled& second) {
		using std::swap; // allow adl
		swap(first._dbls, second._dbls);
		swap(first._retdbls, second._retdbls);
		swap(first._nargs, second._nargs);
		swap(first._jitted, second._jitted);
	}
};

// this is the compiler's internal representation of functions.
// its more heavyweight than the above representation, and stores
// information about all the partials, and their values.
struct FuncCode {
	struct Partial {
		std::string param, func_name;
		llvm::Function *function;
		ExprPtr expr;
		int idx;
	};
	~FuncCode();
	ExprPtr self;
	llvm::Function *compiled, *allpartials;
	std::unordered_map<std::string, std::unique_ptr<Partial>> partials;
};

class Compiler : public ExprVisitor {
public:

	void visit(EmptyExpr&) { assert(0); }
	void visit(AddExpr &e);
	void visit(SubExpr &e);
	void visit(NegExpr &e);
	void visit(MulExpr &e);
	void visit(DivExpr &e);
	void visit(VarExpr &e);
	void visit(ExptExpr &e);
	void visit(LitExpr &e);
	void visit(FuncExpr &e);
	void visit(CallExpr &e);
	// getter for the module
	llvm::Module *module() const { return _module; }
	// compile a function into llvm ir. if _optimizing is true,
	// then it optimizes this function as well. otherwise, the function
	// can be optimized using the optimize method.
	llvm::Function *compile_func_partials(FuncExpr const &f);
	llvm::Function *compile_func(FuncExpr const &f);



	// compile an expression into llvm ir. if it's a FuncExpr
	// it returns compile_func.  otherwise it compiles it into
	// a function which has no name and takes no arguments but
	// returns the value computed by the expression.
	llvm::Function *compile_expr(ExprPtr const &e);
	// JIT compiles the function an returns a pointer to void
	// which may be reinterpret_cast into a function with the
	// appropriate arguments.
	void *jit_to_void(llvm::Function *f);



	Compiled jit(llvm::Function *f, size_t nargs); // nargs is, e.g. FuncExpr::params().size()

	// same as above, but runs compile_expr first, and throws
	// if e is a FuncExpr. (note: if _optimizing is true, this
	// function will be optimized).
	Compiled jit_expr(ExprPtr const &e);
	// convert a function call to a jitted function.
	// this is somewhat slow, as it needs to compile a small function to do this.
	// when possible, prefer the execute method
	Compiled jit_call(llvm::Function *f, std::vector<interval> const &args);
	// ame as above, but looks the function up in the symbol table first.
	Compiled jit_call(std::string const &name, std::vector<interval> const &args);
	// convert a vector of intervals to a vector of ConstantFPs.
	std::vector<llvm::Value*> ivec2vvec(std::vector<interval> const &v);
	// convert a vector of intervals to a vector of llvm generic values (representing doubles).
	// execute the given function with the given arguments
//	interval execute(llvm::Function *f, std::vector<interval> const &args);
	// same as above, but look up the expression in the symbol table first
//	interval execute(std::string const &s, std::vector<interval> const &args);
	// compile and execute ep.  returns an empty interval if ep is a funcexpr.
	interval execute(ExprPtr const &ep);

	// get and set the value of _optimizing, which effects if
	// a compiled function is optimized by default.
	// the value of _optimizing defaults to true.
	bool is_optimizing() const { return _optimizing; }
	int verbiosity() const { return _verbose; }
	void set_verbose(int v) { _verbose = v; }
	void set_optimizing(bool val=true) { _optimizing = val; }
	// perform optimization using this compilers _fpm on `e`
	void optimize(llvm::Function &e);

	void do_jit(llvm::Function *f);

	llvm::Function *lookup(std::string const &name);
	llvm::Function *lookup_partials(std::string const &name);

	Compiler();
	~Compiler();
private:


	VInterval _iv;
	// a pointer to the llvm module.
	llvm::Module *_module;
	// the ir builder we're using.
	llvm::IRBuilder<> _builder;
	// vintervals for the named values in this function.
	// We unbox arguments for functions to reduce the calling overhead
	// so `func(a, b, c, d)` is compiled to a llvm::Function taking 8 doubles,
	// specifically a_lo, a_hi, b_lo, b_hi, c_lo, c_hi, d_lo, and d_hi.

	llvm::StringMap<std::shared_ptr<FuncCode>> _compiled;
	llvm::StringMap<VInterval> _named;
	llvm::StringMap<llvm::Function*> _builtin;
	// current rounding mode. initally Unknown.
	// we try to avoid changing this because changes are slow,
	// so we keep track of it here. when round_up and round_down
	// are called with false (default), they check here first to
	// determine if a change is actually needed
	RoundMode _round_mode;
	// functions to change the rounding mode.  we keep a pointer to
	// these so we don't need to call _module->getFunction(...)
	// every time we want to change the rounding mode.
	llvm::Function *_round_up, *_round_down, *_fesetrnd;
	// a struct type for intervals.  this is only used for the return value of functions
	// (as llvm doesn't support multiple return values). it's simply an aggregate of two
	// doubles. it's possible that we'd get better efficiency by using a vector type,
	// but this way we maintain some sort of binary compatibility with the functions
	// in the interpreter, though, due to the argument type differences, this isn't
	// really true.
	llvm::StructType *_iv_type;

	// funciton pass manager for optimization
	llvm::FunctionPassManager _fpm;
	// pointer to the fpm (for polymorphism)
	llvm::FunctionPassManager  *_fpm_ref;
	// execution engine for jit
	llvm::ExecutionEngine *_exec_engine;
	// whether or not expressions given to compile_expr and compile_func
	// should be optimized.
	// (note: Expressions given to optimize are always optimized)
	bool _optimizing;
	int _verbose;
	int _vd;

	VInterval compile(Expr&);
	void init_builtins();
	void add_builtin(std::string const &name, std::string const &alias, size_t nargs=1);
	void init_module();
	llvm::Function *compile_1func(FuncExpr const &f);

	llvm::Value *cforce_round(llvm::Value *a, std::string const &name="force_round");

	// utility functions for compiling these operations.  some of
	// them, e.g. {mul,div}_{hi,lo} aren't used.
	llvm::Value *cadd_lo(llvm::Value *a, llvm::Value *b, char const *name="add_lo");
	llvm::Value *cadd_hi(llvm::Value *a, llvm::Value *b, char const *name="add_hi");
	llvm::Value *csub_lo(llvm::Value *a, llvm::Value *b, char const *name="sub_lo");
	llvm::Value *csub_hi(llvm::Value *a, llvm::Value *b, char const *name="sub_hi");
	llvm::Value *cmul_lo(llvm::Value *a, llvm::Value *b, char const *name="mul_lo");
	llvm::Value *cmul_hi(llvm::Value *a, llvm::Value *b, char const *name="mul_hi");
	llvm::Value *cdiv_lo(llvm::Value *a, llvm::Value *b, char const *name="div_lo");
	llvm::Value *cdiv_hi(llvm::Value *a, llvm::Value *b, char const *name="div_hi");
	// compile the inverting of an interval
	VInterval cinvert(llvm::Value *lo, llvm::Value *hi, std::string const &name="invert");
	// compile repeated exponentiation
	llvm::Value *cpow(llvm::Value *val, int times, std::string const &name="pow");
	// compile a call to set_rup and set_rdown if force is true or if there's a different
	// rounding mode active than
	void round_up(bool force=false);
	void round_down(bool force=false);
	void branched();
	// compile code computing the min/max of two numbers, and return those numbers.
	llvm::Value *cmax(llvm::Value *a, llvm::Value *b, std::string const &name="max");
	llvm::Value *cmin(llvm::Value *a, llvm::Value *b, std::string const &name="min");
	// compile code computing the min/max of four numbers, and return the min/max respectively
	llvm::Value *cmax4(llvm::Value *a, llvm::Value *b, llvm::Value *c, llvm::Value *d, std::string const &name="max4");
	llvm::Value *cmin4(llvm::Value *a, llvm::Value *b, llvm::Value *c, llvm::Value *d, std::string const &name="min4");

	llvm::Value *zero();
	llvm::Value *get_dbl(double d);

	llvm::Value *ceq0test(llvm::Value *lo, llvm::Value *hi, std::string const &name="zero_test");
	llvm::Value *clt0test(llvm::Value *lo, llvm::Value *hi, std::string const &name="lt_zero_test");
	llvm::Value *cgt0test(llvm::Value *lo, llvm::Value *hi, std::string const &name="gt_zero_test");
	llvm::Value *chas0test(llvm::Value *lo, llvm::Value *hi, std::string const &name="has_zero_test");
	llvm::Value *cemptytest(llvm::Value *lo, llvm::Value *hi, std::string const &name="empty_test");

	// convert a vinterval into an llvm::Value as an aggregate of two doubles.
	llvm::Value *c_i2v(llvm::Value *a, llvm::Value *b, std::string const &name="iv");
	llvm::Value *c_i2v(VInterval vi, std::string const &name="iv");

	Compiler(Compiler const &) = delete;
	Compiler &operator=(Compiler const &) = delete;
};

class PartialComp {
private:
	Compiler *_ctx;
	std::string _fname;
	std::vector<ExprPtr> _fpartials;
	std::vector<std::string> _pnames, _pf_names;
	Compiled _compiled;
	llvm::Function *_fn;
	void initialize(FuncExpr const &e);
public:
	PartialComp();
	PartialComp(Compiler *c);
	PartialComp(Compiler *c, FuncExpr &fe);
	PartialComp &operator=(PartialComp const &);

	std::vector<interval> calculate(std::vector<interval> const &args);
	llvm::Function *fn() const { return _fn; }
	PartialComp(PartialComp const &) = default;
	PartialComp &operator=(PartialComp&&) = default;
	std::vector<std::string> const &params() const { return _pnames; }
	std::vector<ExprPtr> const &partials() const { return _fpartials; }
	size_t partial_count() const { return _fpartials.size() - 1; }
	size_t expr_count() const { return _fpartials.size(); }
};



}
#endif