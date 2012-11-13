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
#include <map>

namespace calc {

// utility struct to simulate multiple return values in c++.
// we could use a std::pair or std::tuple, but this is friendlier
// in the debugger and it's used enough to justify its existence
struct VInterval {
	llvm::Value *lo, *hi;
};

struct ret_interval {
	double lo, hi;
};

inline interval from_ret(ret_interval ri) {
	return interval{ri.lo, ri.hi};
}

// enum to represent the rounding mode.
enum class RoundMode { /* ToNear, ToZero, */ Up, Down, Unknown };

typedef ret_interval (*jitted_function)(void);

class Compiler : public ExprVisitor {
public:

	// these move the module from the other compiler.
	// that is, the created or assigned compiler takes ownership
	// of all data from the other, which is left empty.
	void visit(EmptyExpr&) { assert(0); } // impossible
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
	// JIT compiles the function into a function which takes
	// no arguments and returns an interval. if `f` takes arguments,
	// an exception is thrown.
	jitted_function jit(llvm::Function *f);
	// same as above, but runs compile_expr first, and throws
	// if e is a FuncExpr. (note: if _optimizing is true, this
	// function will be optimized).
	jitted_function jit_expr(ExprPtr const &e);
	// convert a function call to a jitted function.
	// this is somewhat slow, as it needs to compile a small function to do this.
	// when possible, prefer the execute method
	jitted_function jit_call(llvm::Function *f, std::vector<interval> const &args);
	// ame as above, but looks the function up in the symbol table first.
	jitted_function jit_call(std::string const &name, std::vector<interval> const &args);
	// convert a vector of intervals to a vector of ConstantFPs.
	std::vector<llvm::Value*> ivec2vvec(std::vector<interval> const &v);
	// convert a vector of intervals to a vector of llvm generic values (representing doubles).
	// execute the given function with the given arguments
	interval execute(llvm::Function *f, std::vector<interval> const &args);
	// same as above, but look up the expression in the symbol table first
	interval execute(std::string const &s, std::vector<interval> const &args);
	// compile and execute ep.  returns an empty interval if ep is a funcexpr.
	interval execute(ExprPtr const &ep);

	// get and set the value of _optimizing, which effects if
	// a compiled function is optimized by default.
	// the value of _optimizing defaults to true.
	bool is_optimizing() const { return _optimizing; }
	void set_optimizing(bool val=true) { _optimizing = val; }
	// perform optimization using this compilers _fpm on `e`
	void optimize(llvm::Function &e);

	void do_jit(llvm::Function *f);
	Compiler();
	~Compiler();
private:


	VInterval _iv;
	// a pointer to the llvm module.
	llvm::Module *_module;
	// the ir builder we're using.
	llvm::IRBuilder<true, llvm::NoFolder> _builder;
	// vintervals for the named values in this function.
	// We unbox arguments for functions to reduce the calling overhead
	// so `func(a, b, c, d)` is compiled to a llvm::Function taking 8 doubles,
	// specifically a_lo, a_hi, b_lo, b_hi, c_lo, c_hi, d_lo, and d_hi.
	std::map<std::string, VInterval> _named;
	// current rounding mode. initally Unknown.
	// we try to avoid changing this because changes are slow,
	// so we keep track of it here. when round_up and round_down
	// are called with false (default), they check here first to
	// determine if a change is actually needed
	RoundMode _round_mode;
	// functions to change the rounding mode.  we keep a pointer to
	// these so we don't need to call _module->getFunction(...)
	// every time we want to change the rounding mode.
	llvm::Function *_round_up, *_round_down;
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

	VInterval compile(Expr&);

	void init_module();

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
	void reset_round();
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

/*
class PartialCalc {
	Evaluator _ctx; // env with partial functions defined
	std::string _fname; // original func name
	std::vector<std::string> _pnames, _pf_names; // param names and partial func names
	std::vector<ExprPtr> _fpartials; // funcexprs representing the partials
	void initialize(FuncExpr const &fe);
public:
	PartialCalc();
	PartialCalc(FuncExpr const &fe, Evaluator &ev);
	PartialCalc(FuncExpr const &fe);
	PartialCalc(PartialCalc const &) = default;
	PartialCalc&operator=(PartialCalc const &);
	PartialCalc&operator=(PartialCalc&&) = default;
	std::string const &func_name() const { return _fname; }
	void call_args(CallExpr const &ce, std::vector<interval> &parms) { _ctx.call_args(ce, parms); }
	std::vector<std::string> const &params() const { return _pnames; }
	std::vector<ExprPtr> const &partials() const { return _fpartials; }
	size_t partial_count() const { return _fpartials.size() - 1; }
	size_t expr_count() const { return _fpartials.size(); }
	std::vector<interval> calculate(std::vector<interval> const &args);
	std::vector<interval> calculate(std::vector<ExprPtr> const &args);
};

*/

class PartialComp {
	Compiler *_ctx;
	std::string _fname;
	std::vector<std::string> _pnames, _pf_names;
	std::vector<ExprPtr> _fpartials;
	std::vector<llvm::Function*> _funcs;
	void initialize(FuncExpr const &e);
public:
	PartialComp();
	PartialComp(Compiler *c);
	PartialComp(Compiler *c, FuncExpr &fe);
	PartialComp &operator=(PartialComp const &);

	std::vector<interval> calculate(std::vector<interval> const &args);

	PartialComp(PartialComp const &) = default;
	PartialComp &operator=(PartialComp&&) = default;


	std::vector<std::string> const &params() const { return _pnames; }
	std::vector<ExprPtr> const &partials() const { return _fpartials; }
	std::vector<llvm::Function*> const &funcs() const { return _funcs; }
	size_t partial_count() const { return _fpartials.size() - 1; }
	size_t expr_count() const { return _fpartials.size(); }

};









}
#endif