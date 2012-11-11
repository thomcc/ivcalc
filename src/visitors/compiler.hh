#ifndef __COMPILER_HH__
#define __COMPILER_HH__
#include "common.hh"
#include "expr.hh"
#include "llvm/Support/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include <map>
namespace calc {

// utility struct to simulate multiple return values in c++.
// we could use a std::pair or std::tuple, but this is friendlier
// in the debugger and it's used enough to justify its existence
struct VInterval {
	llvm::Value *lo, *hi;
};

// enum to represent the rounding mode.
enum class RoundMode { ToNear, ToZero, Up, Down, Unknown };

class Compiler : public ExprVisitor {
public:
	Compiler();
	// initialize from a module.  we check if the the module has
	// the r_up and r_down functions and define them if not.  otherwise
	// we assume it's been initiallized already.
	Compiler(std::unique_ptr<llvm::Module>);
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

	std::unique_ptr<llvm::Module> const &module() const { return _module; }
	llvm::Function *compile_func(FuncExpr const &f);
private:
	// visitor return value.
	VInterval _iv;
	// a pointer to the llvm module.
	std::unique_ptr<llvm::Module> _module;
	// the ir builder we're using.
	llvm::IRBuilder<> _builder;
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
	// utility method for the visit methods.
	VInterval compile(Expr&);
	// initialize the module, and, to a large extent, this class.
	//
	void init_module();
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

	// convert a vinterval into an llvm::Value as an aggregate of two doubles.
	llvm::Value *c_i2v(llvm::Value *a, llvm::Value *b, std::string const &name="iv");
	llvm::Value *c_i2v(VInterval vi, std::string const &name="iv");

	Compiler(Compiler const &) = delete;
	Compiler &operator=(Compiler const &) = delete;
};











}
#endif