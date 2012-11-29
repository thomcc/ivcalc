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


typedef void (*Jitted)(pod_interval *dest, pod_interval *source);

class Compiler;



// Compiled:
//
// return type of Compiler::partials(FuncExpr const &e);
// compiled unit of code computing partial derivatives for a function
class Compiled {
	friend class Compiler;
public:
	// compute partial derivatives.  destination is provided. partials will be ordered
	// (orig, param_0, param_1, ...).  e.g. if this Compiled holds my_f(a, b, c),
	// which is called with parameters 0, 1, and 2, the vector will hold four values:
	// {my_f(0, 1, 2), ∂my_f/∂a(0, 1, 2), ∂my_f/∂b(0, 1, 2), ∂my_f/∂c(0, 1, 2)}
	// takes llvm::MutableArrayRef<T> so that any consecutive block of memory can be provided.
	// (a pod_interval[N], a std::vector<pod_interval> will all work).
	void apply(llvm::MutableArrayRef<pod_interval> dst, llvm::MutableArrayRef<pod_interval> src);
	// same as above but destination need not be provided by the user. thows when v is too small.
	std::vector<pod_interval> apply(llvm::MutableArrayRef<pod_interval> v);

	// same as above, but taking a more convenient array of doubles.
	void apply(llvm::MutableArrayRef<pod_interval> dst, llvm::MutableArrayRef<double> src);
	std::vector<pod_interval> apply(llvm::MutableArrayRef<double> v);

	// also similar to the above, but takes and returns an array of normal (non-pod)
	// intervals.  These are slow, due to all the conversion necessary.
	void apply(llvm::MutableArrayRef<interval> dst, llvm::MutableArrayRef<interval> src);
	std::vector<interval> apply(llvm::MutableArrayRef<interval> v);

	// get the function's name. llvm might store the name differently.
	std::string const &name() const { return _name; }
	// get what llvm stores the name as.
	std::string const &ll_name() const { return _ll_name; }
	// number of arguments
	size_t nargs() const { return _nargs; }
	// get the names of the parameters
	std::vector<std::string> const &params() const { return _e.params(); }
	// the FuncExpr this was compiled from (excluding partials)
	FuncExpr const &expr() { return _e; }
	// the llvm::Function this was compiled into.
	llvm::Function *function() const { return _fn; }
	// get the jitted function, which can be called with two arrays of size `nargs()`.
	std::function<void(pod_interval*, pod_interval*)> jitted() const { return _impl; }

private:
	Compiled(llvm::Function *f, Jitted fp, FuncExpr const &ep)
		: _fn(f), _nargs(ep.params().size()), _name(ep.name()), _ll_name(f->getName()), _e(ep.name(), ep.params(), ep.impl()->clone()), _impl(fp) {}
	llvm::Function *_fn;
	size_t _nargs;
	std::string _name, _ll_name; // llvm renames duplicated things, so these might be different
	FuncExpr _e;
	std::function<void(pod_interval*, pod_interval*)> _impl;
};


class Compiler : public ExprVisitor {
public:
	Compiler();
	~Compiler();


	llvm::Module *module() const { return _module; }

	std::shared_ptr<Compiled> partials(FuncExpr const &e);

	interval execute(ExprPtr const &ep);

	std::shared_ptr<Compiled> lookup_partials(std::string const &name);

	llvm::Function *compile_func_partials(FuncExpr const &f);
	llvm::Function *compile_func(FuncExpr const &f);


	bool is_optimizing() const { return _optimizing; }
	void set_optimizing(bool val=true) { _optimizing = val; }

	int verbiosity() const { return _verbose; }
	void set_verbose(int v) { _verbose = v; }

	// todo:  should more of api be public?
private:
	struct UDF {
		llvm::Function *f;
		ExprPtr ep;
		size_t nargs;
	};
	llvm::Value *_v;
	llvm::Module *_module;
	llvm::IRBuilder<> _builder;
	llvm::StringMap<llvm::Value*> _named;
	llvm::StringMap<llvm::Function*> _builtin;
	// string map is faster, but can't handle smart pointers :(
	std::unordered_map<std::string, UDF> _udfs;
	std::unordered_map<std::string, std::shared_ptr<Compiled>> _compiled;

	llvm::Function *_ivadd, *_ivmul, *_ivsub, *_ivdiv, *_ivneg, *_ivpow;
	llvm::StructType *_iv_type;
	llvm::FunctionPassManager _fpm;
	llvm::FunctionPassManager  *_fpm_ref;
	llvm::ExecutionEngine *_exec_engine;

	bool _optimizing;
	int _verbose;

	template <unsigned level, typename... Args> void log(Args... a);

	void optimize(llvm::Function &e);
	void do_jit(llvm::Function *f);

	void init_builtins();
	void add_builtin(std::string const &name, std::string const &alias, size_t nargs=1);
	void init_module();
	llvm::Value *compile(Expr&);
	void *jit_func(llvm::Function *f);

	std::pair<llvm::Value*, llvm::Value*> unbox(llvm::Value *i);
	llvm::Value *box(llvm::Value *lo, llvm::Value *hi);
	llvm::Value *get_fp(double d=0);

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

	Compiler(Compiler const &) = delete;
	Compiler &operator=(Compiler const &) = delete;
};

}

#endif