#include "visitors/compiler.hh"
#include "visitors/simplifier.hh"
#include "visitors/derivator.hh"
#include "visitors/varaddresser.hh"
#include <llvm/DerivedTypes.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Intrinsics.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Support/TargetSelect.h>
#include "llvm/Transforms/Vectorize.h"
#include <llvm/ExecutionEngine/GenericValue.h>

#ifndef SSE_INTERVAL_FUNCS
#  define SSE_INTERVAL_FUNCS 1
#endif

#ifndef PURE_FUNCTIONS
#  define PURE_FUNCTIONS 1
#endif

#define PURE_FUNCTIONS 1

namespace calc {

using namespace llvm;
using namespace std;
Compiler::Compiler()
: _v(nullptr)
, _module(new Module("interval jit", getGlobalContext()))
, _builder(_module->getContext())
, _fpm(_module)
, _optimizing(true)
, _verbose(0) {
	init_module();
}

Compiler::~Compiler() { delete _module; }

template <unsigned level, typename... Args>
void Compiler::log(Args... args) {
	if (level < _verbose) {
		print_out(cerr, args...);
		cerr << endl;
	}
}

void Compiler::init_module() {

	InitializeNativeTarget();
	EngineBuilder eb{_module};
//	TargetOptions tgopts;
//	tgopts.HonorSignDependentRoundingFPMathOption = 1;
//	eb.setTargetOptions(tgopts);
	eb.setEngineKind(EngineKind::JIT);
	_exec_engine = eb.create();
	_fpm.add(new TargetData(*_exec_engine->getTargetData()));

	// passes from vmkit
//	_fpm.add(createCFGSimplificationPass()); // clean up
	_fpm.add(createPromoteMemoryToRegisterPass());

//	_fpm.add(createInstructionCombiningPass());
	_fpm.add(createScalarReplAggregatesPass());

	_fpm.add(createInstructionCombiningPass());
	_fpm.add(createEarlyCSEPass());
	_fpm.add(createReassociatePass());

//	_fpm.add(createJumpThreadingPass());
	_fpm.add(createCFGSimplificationPass());
	_fpm.add(createCorrelatedValuePropagationPass());
	//	_fpm.add(createInstructionCombiningPass());
	_fpm.add(createGVNPass());

	//_fpm.add(createCFGSimplificationPass());
	_fpm.add(createReassociatePass());

	_fpm.add(createBBVectorizePass());

	_fpm.add(createEarlyCSEPass());
	// no loops really so...
	//_fpm.add(createLoopIdiomPass());
	//_fpm.add(createLoopRotatePass());
	//_fpm.add(createLICMPass());
	//_fpm.add(createLoopUnswitchPass());
	_fpm.add(createInstructionCombiningPass());
	//_fpm.add(createIndVarSimplifyPass());
	//_fpm.add(createLoopDeletionPass());
	//_fpm.add(createLoopUnrollPass());
	//_fpm.add(createLoopStrengthReducePass());

	//_fpm.add(createInstructionCombiningPass());
	_fpm.add(createGVNPass());
	_fpm.add(createMemCpyOptPass());
	_fpm.add(createSCCPPass());

	_fpm.add(createSinkingPass());
	_fpm.add(createDeadCodeEliminationPass());
	_fpm.add(createInstructionSimplifierPass());
	_fpm.add(createCorrelatedValuePropagationPass());

	_fpm.add(createGVNPass());


	_fpm.add(createInstructionCombiningPass());

//	_fpm.add(createJumpThreadingPass());
//	_fpm.add(createDeadStoreEliminationPass());

//	_fpm.add(createAggressiveDCEPass());
//	_fpm.add(createCFGSimplificationPass());

	_fpm.doInitialization();
	_fpm_ref = &_fpm;

	vector<Type*> dbls(2, Type::getDoubleTy(_module->getContext()));
	_iv_type = StructType::get(_module->getContext(), dbls, false);

	vector<Type*> ivs(2, _iv_type);
	FunctionType *iv_op = FunctionType::get(_iv_type, ivs, false);
#if SSE_INTERVAL_FUNCS
	Constant *ivadd = _module->getOrInsertFunction("iv_vadd", iv_op);
	Constant *ivsub = _module->getOrInsertFunction("iv_vsub", iv_op);
	Constant *ivmul = _module->getOrInsertFunction("iv_vmul", iv_op);
	Constant *ivdiv = _module->getOrInsertFunction("iv_vdiv", iv_op);
#else
	Constant *ivadd = _module->getOrInsertFunction("iv_vadd", iv_op);
	Constant *ivsub = _module->getOrInsertFunction("iv_vsub", iv_op);
	Constant *ivmul = _module->getOrInsertFunction("iv_vmul", iv_op);
	Constant *ivdiv = _module->getOrInsertFunction("iv_vdiv", iv_op);
#endif
	Type *powargs[2] = {_iv_type, Type::getInt32Ty(_module->getContext())};
	FunctionType *iv_pow_type = FunctionType::get(_iv_type, powargs, false);
	Constant *ivpow = _module->getOrInsertFunction("iv_pow", iv_pow_type);

	_ivadd = cast<Function>(ivadd);
	_ivsub = cast<Function>(ivsub);
	_ivdiv = cast<Function>(ivdiv);
	_ivmul = cast<Function>(ivmul);
	_ivpow = cast<Function>(ivpow);
#if PURE_FUNCTIONS

	_ivadd->addFnAttr(Attribute::ReadNone);
	_ivadd->addFnAttr(Attribute::NoUnwind);

	_ivsub->addFnAttr(Attribute::ReadNone);
	_ivsub->addFnAttr(Attribute::NoUnwind);

	_ivdiv->addFnAttr(Attribute::ReadNone);
	_ivdiv->addFnAttr(Attribute::NoUnwind);

	_ivmul->addFnAttr(Attribute::ReadNone);
	_ivmul->addFnAttr(Attribute::NoUnwind);

	_ivpow->addFnAttr(Attribute::ReadNone);
	_ivpow->addFnAttr(Attribute::NoUnwind);

#endif


	init_builtins();
}

void Compiler::init_builtins() {
#define BUILTIN_N(name, n) add_builtin("iv_" STRINGIZE(name), STRINGIZE(name))
#define BUILTIN(name) BUILTIN_N(name, 1)
	BUILTIN(sqrt);  BUILTIN(exp);   BUILTIN(log);   BUILTIN(sin);   BUILTIN(cos);
	BUILTIN(tan);   BUILTIN(sec);   BUILTIN(csc);   BUILTIN(cot);   BUILTIN(asin);
	BUILTIN(acos);  BUILTIN(atan);  BUILTIN(asec);  BUILTIN(acsc);  BUILTIN(acot);
	BUILTIN(sinh);  BUILTIN(cosh);  BUILTIN(tanh);  BUILTIN(sech);  BUILTIN(csch);
	BUILTIN(coth);  BUILTIN(asinh); BUILTIN(acosh); BUILTIN(atanh); BUILTIN(asech);
	BUILTIN(acsch); BUILTIN(acoth);
#undef BUILTIN
#undef BUILTIN_N
}

void Compiler::add_builtin(string const &name, string const &alias, size_t nargs) {
	vector<Type*> args(nargs, _iv_type);
	FunctionType *builtin_type = FunctionType::get(_iv_type, args, false);
	Constant *cf = _module->getOrInsertFunction(name, builtin_type);
	Function *f = cast<Function>(cf);
#if PURE_FUNCTIONS
	f->addFnAttr(Attribute::ReadNone);
	f->addFnAttr(Attribute::NoUnwind);
#endif
	if (_builtin.find(alias) != _builtin.end())
		throw iv_arithmetic_error("BUG: builtin duplicated!");
	_builtin[alias] = f;
}

Value *Compiler::box(Value *lo, Value *hi) {
	unsigned zero[1] = {0}, one[1] = {1};
	Value *insert = _builder.CreateInsertValue(UndefValue::get(_iv_type), lo, zero);
	return _builder.CreateInsertValue(insert, hi, one);
}

pair<Value*, Value*> Compiler::unbox(Value *i) {
	unsigned zero[1] = {0}, one[1] = {1};
	Value *lo = _builder.CreateExtractValue(i, zero);
	Value *hi = _builder.CreateExtractValue(i, one);
	return make_pair(lo, hi);
}

Value *Compiler::compile(Expr &e) {
	_v = nullptr;
	e.accept(*this);
	return _v;
}

void Compiler::visit(AddExpr &e) {
	log<2>("compile ADD");
	Value *lhs = compile(*e.lhs());
	Value *rhs = compile(*e.rhs());
	_v = _builder.CreateCall2(_ivadd, lhs, rhs, "add");
}

void Compiler::visit(SubExpr &e) {
	log<2>("compile SUB");
	Value *lhs = compile(*e.lhs());
	Value *rhs = compile(*e.rhs());
	_v = _builder.CreateCall2(_ivsub, lhs, rhs, "sub");
}

void Compiler::visit(NegExpr &e) {
	log<2>("compile NEG");
	Value *v = compile(*e.value());
	pair<Value*, Value*> lohi = unbox(v);
	Value *lv = _builder.CreateFNeg(lohi.second, "neg.lo");
	Value *hv = _builder.CreateFNeg(lohi.first, "neg.hi");
	_v = box(lv, hv);
}

void Compiler::visit(MulExpr &e) {
	log<2>("compile MUL");
	Value *lhs = compile(*e.lhs());
	Value *rhs = compile(*e.rhs());
	_v = _builder.CreateCall2(_ivmul, lhs, rhs, "mul");
}

void Compiler::visit(DivExpr &e) {
	log<2>("compile DIV");
	Value *lhs = compile(*e.lhs());
	Value *rhs = compile(*e.rhs());
	_v = _builder.CreateCall2(_ivdiv, lhs, rhs, "div");
}

void Compiler::visit(VarExpr &e) {
	auto it = _named.find(e.name());
	if (it != _named.end())
		_v = it->getValue();
	else throw iv_arithmetic_error("Error: unbound variable in function: "+e.name());
}

void Compiler::visit(ExptExpr &e) {
	log<2>("compile EXPT (power = ", e.power(), ")");
	Value *base = compile(*e.base());
	Value *pow = ConstantInt::get(Type::getInt32Ty(_module->getContext()), e.power());
	_v = _builder.CreateCall2(_ivpow, base, pow, "expt");
}

void Compiler::visit(LitExpr &e) {
	log<2>("compile LIT (value = ", e.value(), ")");
	_v = box(get_fp(e.value().lo()), get_fp(e.value().hi()));
}

void Compiler::visit(CallExpr &e) {
	log<2>("compile CALL (callee = '", e.name(), "')");
	Function *callee = nullptr;
	auto bit = _builtin.find(e.name());
	if (bit != _builtin.end()) callee = bit->getValue();
	else {
		auto uit = _udfs.find(e.name());
		if (uit != _udfs.end()) callee = uit->second.f;
	}
	if (!callee) throw iv_arithmetic_error("Unknown function in call: "+e.name());
	vector<Value*> args;
	for (auto const &expr : e.args())
		args.push_back(compile(*expr));
	_v = _builder.CreateCall(callee, args);
}

void Compiler::visit(FuncExpr &e) {
	log<2>("compile FUNC (name = '", e.name(), "')");
	compile_func(e);
	_v = box(get_fp(rmath::NaN()), get_fp(rmath::NaN()));
}

void Compiler::optimize(Function &e) {
	log<1>("OPTIMIZE");
	_fpm_ref->run(e);
}

void Compiler::do_jit(Function *f) {
	log<1>("JIT FUNCTION (llvm name = '", f->getName().data(), "')");
	_exec_engine->runJITOnFunction(f);
}

interval Compiler::execute(ExprPtr const &ep) {
	if (ep->as_func_expr()) {
		compile(*ep);
		return interval::empty();
	}

	Function *f = compile_func(FuncExpr(move(ep->clone())));
	assert(f);
	assert(f->arg_size() == 0);
	assert(ep.get());
	pod_interval (*ff)(void) = (pod_interval (*)(void))reinterpret_cast<uintptr_t>(_exec_engine->getPointerToFunction(f));
	pod_interval i = ff();
	return interval(i.lo, i.hi);
}

shared_ptr<Compiled> Compiler::lookup_partials(string const &name) {
	auto it = _compiled.find(name);
	if (it == _compiled.end()) return nullptr;
	return it->second;
}

Value *Compiler::get_fp(double d) {
	return ConstantFP::get(_module->getContext(), APFloat(d));
}


struct NamedEraser {
	// raii struct for making sure if we throw we aren't left in an invalid state
	NamedEraser(vector<string> const &todelete, llvm::StringMap<Value*> &map)
		: _map(map), _todelete(todelete) {}
	~NamedEraser() {
		for (auto const &s : _todelete) _map.erase(s);
	}
	llvm::StringMap<Value*> &_map;
	vector<string> const &_todelete;
};

Function *Compiler::compile_func(FuncExpr const &e) {
	log<0>("compiling Function: ", e.name(), "(", strjoin(e.params(), ", "), ")");
	vector<Type*> parms(e.params().size(), _iv_type);
	FunctionType *ftype = FunctionType::get(_iv_type, parms, false);
	Function *func = Function::Create(ftype, Function::ExternalLinkage, e.name(), _module);
	assert(func);
	auto argiter = func->arg_begin();
	for (auto const &name : e.params()) {
		argiter->setName(name);
		_named[name] = argiter++;
	}
	NamedEraser eraser(e.params(), _named);
	BasicBlock *bb = BasicBlock::Create(_module->getContext(), "entry", func);
	_builder.SetInsertPoint(bb);
	Value *v = compile(*Simplifier::simplified(e.impl()));
	if (!v) throw iv_arithmetic_error("Error during compilation of function '"+e.name()+"'");
	_builder.CreateRet(v);
	func->dump();
	log<0>("\tVerifying...");
	verifyFunction(*func);
	log<0>("\tOptimizing...");
	if (is_optimizing())
		optimize(*func);
	// store it so other functions can call it, unless its a temporary (e.g. from execute)
	if (e.name().size()) {
		auto uit = _udfs.find(e.name());
		if (uit != _udfs.end()) {
			cerr << "[Warning]: redefining '" << e.name() << "'" << endl;
			uit->second.f->eraseFromParent();
		}
		UDF &udf = _udfs[e.name()];
		udf.f = func;
		udf.nargs = e.params().size();
		udf.ep = e.clone();
	}
	return func;
}


Function *Compiler::compile_func_partials(FuncExpr const &e) {
	vector<Type*> parms(2, _iv_type->getPointerTo());
	log<0>("Compiling partials for ", e.name(), "(", strjoin(e.params(), ", "), ")");
	FunctionType *ft = FunctionType::get(Type::getVoidTy(_module->getContext()), parms, false);
	Function *func = Function::Create(ft, Function::ExternalLinkage, e.name()+"_partials", _module);
	BasicBlock *bb = BasicBlock::Create(_module->getContext(), "entry", func);
	auto argiter = func->arg_begin();
	argiter->setName("dst");
	Value *ary_dest = argiter++;
	argiter->setName("src");
//	Type *i64 = Type::getInt64Ty(_module->getContext());
	Value *ary_src = argiter++;
	uint32_t idx = 0;
	_builder.SetInsertPoint(bb);
	for (auto const &parm : e.params()) {
		log<0>("PARAM: "+parm);
//		Value *index = _builder.getInt32(idx++); //idx++; // eat hi
		Value *v = _builder.CreateConstGEP1_64(ary_src, idx++, parm);//_builder.CreateGEP(ary_src, index, parm+"_gep");
		_named[parm] = _builder.CreateLoad(v, parm);
	}
	// need to remove _named should we throw.
	NamedEraser helper(e.params(), _named);

	log<1>("\tCompiling actual");
	Value *v = compile(*Simplifier::simplified(e.impl()));
	if (!v) throw iv_arithmetic_error("Error while compiling: "+e.name());
	idx = 0;
	Value *gep_dest = _builder.CreateGEP(ary_dest, _builder.getInt32(idx++));
	_builder.CreateStore(v, gep_dest);

	for (auto const &part : Derivator::partials(e)) {
		std::string const &n = part.first;
		log<1>("\tCompiling d", e.name(), " / d", n);
		Value *v = compile(*Simplifier::simplified(part.second));
		if (!v) throw iv_arithmetic_error("Error while compiling: d"+e.name()+"/d"+n);
		Value *gep_dest = _builder.CreateGEP(ary_dest, _builder.getInt32(idx++));
		_builder.CreateStore(v, gep_dest);
	}

	_builder.CreateRetVoid();
	log<0>("\tVerifying...");

	verifyFunction(*func);
	log<0>("\tOptimizing...");
	if (is_optimizing()) optimize(*func);
	return func;

}

shared_ptr<Compiled> Compiler::partials(FuncExpr const &e) {
	Function *partials = compile_func_partials(e);
	log<0>("\tJitting...");
	Jitted jitted = reinterpret_cast<Jitted>(reinterpret_cast<uintptr_t>(_exec_engine->getPointerToFunction(partials)));
	// can't use make_shared b/c of private ctor
	auto cp = shared_ptr<Compiled>{new Compiled(partials, jitted, e)};
	_compiled[e.name()] = cp;
	return cp;
}


void Compiled::apply(MutableArrayRef<pod_interval> dst,  MutableArrayRef<pod_interval> src) {
	if (dst.size() != nargs()+1) throw iv_arithmetic_error("Compiled: Bad destination array size");
	if (src.size() != nargs()) throw iv_arithmetic_error("Compiled: Bad source array size.");
	// src.data() is not mutated, but i'm not sure how to specify this in the IR...
	// hence the const_cast.

	_impl(dst.data(), src.data());
}

vector<pod_interval> Compiled::apply(MutableArrayRef<pod_interval> v) {
	vector<pod_interval> vec(nargs(), iv_make_empty());
	apply(vec, v);
	return vec;
}


void Compiled::apply(MutableArrayRef<pod_interval> dst, MutableArrayRef<double> src) {
	vector<pod_interval> ivsrc;
	for (auto const &d : src) ivsrc.push_back(pod_interval{d, d});
	apply(dst, ivsrc);
}

vector<pod_interval> Compiled::apply(MutableArrayRef<double> v) {
	vector<pod_interval> vec(nargs(), iv_make_empty());
	apply(vec, v);
	return vec;
}


void Compiled::apply(MutableArrayRef<interval> dst, MutableArrayRef<interval> src) {
	vector<pod_interval> vec(nargs(), iv_make_empty()), ivsrc;
	for (auto const &d : src)
		ivsrc.push_back(pod_interval{d.lo(), d.hi()});
	apply(vec, ivsrc);
	size_t i = 0;
	for (auto const &iv : vec)
		dst[i++] = interval(iv.lo, iv.hi);
}

vector<interval> Compiled::apply(MutableArrayRef<interval> v) {
	vector<interval> vec(nargs(), interval::empty());
	apply(vec, v);
	return vec;
}


}