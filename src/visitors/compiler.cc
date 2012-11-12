#include "visitors/compiler.hh"
#include "visitors/simplifier.hh"
#include "visitors/derivator.hh"

#include <llvm/DerivedTypes.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Intrinsics.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>

namespace calc {

using namespace llvm;
using namespace std;


Compiler::Compiler()
: _iv{nullptr, nullptr}
, _module(new Module("interval jit", getGlobalContext()))
, _builder(_module->getContext())
, _round_mode(RoundMode::Unknown)
, _fpm(_module)
, _optimizing(true) {
	init_module();
}

Compiler *Compiler::get() {
	static Compiler c;
	return &c;
}

Compiler::~Compiler() { /*delete _module;*/ } // todo: figure out why this crashes
/*
Compiler::Compiler(Module *module)
: _iv{nullptr, nullptr}
, _module(module)
, _builder(_module->getContext())
, _round_mode(RoundMode::Unknown)
, _fpm(_module)
, _optimizing(true) {
	init_module();
}*/

void Compiler::init_module() {

	InitializeNativeTarget();
	EngineBuilder eb{_module};
//	eb.setEngineKind(EngineKind::Interpreter);
	eb.setEngineKind(EngineKind::JIT);
	_exec_engine = eb.create();
	_fpm.add(new TargetData(*_exec_engine->getTargetData()));
	_fpm.add(createBasicAliasAnalysisPass());
	_fpm.add(createInstructionCombiningPass());
	_fpm.add(createReassociatePass());
	_fpm.add(createGVNPass());
// segfaults due to llvm bug.  bug is fixed in a newer (unstable) release
//	_fpm.add(createCFGSimplificationPass());
	_fpm.doInitialization();
	_fpm_ref = &_fpm;

	FunctionType *voidvoid = FunctionType::get(Type::getVoidTy(_module->getContext()), false);

	Constant *ru = _module->getOrInsertFunction("set_rounding_mode_up", voidvoid);
	Constant *rd = _module->getOrInsertFunction("set_rounding_mode_down", voidvoid);

	_round_up = cast<Function>(ru);
	_round_down = cast<Function>(rd);

	vector<Type*> dbls;
	dbls.push_back(Type::getDoubleTy(_module->getContext()));
	dbls.push_back(Type::getDoubleTy(_module->getContext()));
	_iv_type = StructType::get(_module->getContext(), dbls, false);

}


void Compiler::round_up(bool force) {
	if (force || (_round_mode != RoundMode::Up)) {
		_builder.CreateCall(_round_up);
		_round_mode = RoundMode::Up;
	}
}

void Compiler::round_down(bool force) {
	if (force || (_round_mode != RoundMode::Down)) {
		_builder.CreateCall(_round_down);
		_round_mode = RoundMode::Down;
	}
}
void Compiler::reset_round() {
	_round_mode = RoundMode::Unknown;
}


Value *Compiler::cadd_lo(Value *a, Value *b, const char *name) {
	round_down();
	return _builder.CreateFAdd(a, b, name);
}

Value *Compiler::cadd_hi(Value *a, Value *b, const char *name) {
	round_up();
	return _builder.CreateFAdd(a, b, name);
}

Value *Compiler::csub_lo(Value *a, Value *b, const char *name) {
	round_down();
	return _builder.CreateFSub(a, b, name);
}

Value *Compiler::csub_hi(Value *a, Value *b, const char *name) {
	round_up();
	return _builder.CreateFSub(a, b, name);
}

Value *Compiler::cmul_lo(Value *a, Value *b, const char *name) {
	round_down();
	return _builder.CreateFMul(a, b, name);
}

Value *Compiler::cmul_hi(Value *a, Value *b, const char *name) {
	round_up();
	return _builder.CreateFMul(a, b, name);
}

Value *Compiler::cdiv_lo(Value *a, Value *b, const char *name) {
	round_down();
	return _builder.CreateFDiv(a, b, name);
}

Value *Compiler::cdiv_hi(Value *a, Value *b, const char *name) {
	round_up();
	return _builder.CreateFDiv(a, b, name);
}

Value *Compiler::cmax(Value *a, Value *b, string const &name) {
	Value *abtest = _builder.CreateFCmpOLT(a, b, name+"_abtest");
	return _builder.CreateSelect(abtest, b, a, name+"_abmax");
}

Value *Compiler::cmin(Value *a, Value *b, string const &name) {
	Value *abtest = _builder.CreateFCmpOLT(a, b, name+"_abtest");
	return _builder.CreateSelect(abtest, a, b, name+"_abmin");
}

Value *Compiler::cmax4(Value *a, Value *b, Value *c, Value *d, string const &name) {
	Value *ab_test = _builder.CreateFCmpOLT(a, b, name+"_ab_test");
	Value *cd_test = _builder.CreateFCmpOLT(c, d, name+"_cd_test");
	Value *ab_max = _builder.CreateSelect(ab_test, b, a, name+"_ab_max");
	Value *cd_max = _builder.CreateSelect(cd_test, d, c, name+"_cd_max");
	Value *abcd_test = _builder.CreateFCmpOLT(ab_max, cd_max, name+"_abcd_test");
	return _builder.CreateSelect(abcd_test, cd_max, ab_max, name+"_abcd_max");
}

Value *Compiler::cmin4(Value *a, Value *b, Value *c, Value *d, string const &name) {
	Value *ab_test = _builder.CreateFCmpOLT(b, a, name+"_ab_test");
	Value *cd_test = _builder.CreateFCmpOLT(d, c, name+"_cd_test");
	Value *cd_min = _builder.CreateSelect(cd_test, d, c, name+"_cd_min");
	Value *ab_min = _builder.CreateSelect(ab_test, b, a, name+"_ab_min");
	Value *abcd_test = _builder.CreateFCmpOLT(cd_min, ab_min, name+"_abcd_test");
	return _builder.CreateSelect(abcd_test, cd_min, ab_min, name+"_abcd_min");
}

VInterval Compiler::cinvert(Value *lo, Value *hi, string const &name) {
	VInterval ret{nullptr, nullptr};
	Value *one = ConstantFP::get(_module->getContext(), APFloat(1.0));

	round_up();

	Value *ah = _builder.CreateFDiv(one, lo, name+"_inv_hi_a");
	Value *bh = _builder.CreateFDiv(one, hi, name+"_inv_hi_b");
	ret.hi = cmax(ah, bh, name+"_inv_hi");

	round_down();

	Value *ch = _builder.CreateFDiv(one, lo, name+"_inv_lo_a");
	Value *dh = _builder.CreateFDiv(one, hi, name+"_inv_lo_b");
	ret.lo = cmin(ch, dh, name+"_inv_lo");

	return ret;
}

Value *Compiler::c_i2v(VInterval vi, string const &name) { return c_i2v(vi.lo, vi.hi, name); }
Value *Compiler::c_i2v(Value *a, Value *b, string const &name) {
	vector<unsigned> idxs;
	idxs.push_back(0);
	Value *insert = _builder.CreateInsertValue(UndefValue::get(_iv_type), a, idxs, name+"_interval_init");
	idxs.clear();
	idxs.push_back(1);
	return _builder.CreateInsertValue(insert, b, idxs, name+"_interval");
}


void Compiler::visit(AddExpr &e) {
	VInterval lhs = compile(*e.lhs());
	VInterval rhs = compile(*e.rhs());
	_iv.lo = cadd_lo(lhs.lo, rhs.lo);
	_iv.hi = cadd_hi(lhs.hi, rhs.hi);
}

void Compiler::visit(SubExpr &e) {
	VInterval lhs = compile(*e.lhs());
	VInterval rhs = compile(*e.rhs());
	_iv.lo = csub_lo(lhs.lo, rhs.hi);
	_iv.hi = csub_hi(lhs.hi, rhs.lo);
}

void Compiler::visit(NegExpr &e) {
	VInterval val = compile(*e.value());
	_iv.lo = _builder.CreateFNeg(val.hi, "neg_lo");
	_iv.hi = _builder.CreateFNeg(val.lo, "neg_hi");
}

void Compiler::visit(MulExpr &e) {
	VInterval lhs = compile(*e.lhs());
	VInterval rhs = compile(*e.rhs());
	// todo: consider optimizing in a manner similar to interval.cc

	round_up();
	Value *ah = _builder.CreateFMul(lhs.lo, rhs.lo, "mul_hi_a");
	Value *bh = _builder.CreateFMul(lhs.lo, rhs.hi, "mul_hi_b");
	Value *ch = _builder.CreateFMul(lhs.hi, rhs.lo, "mul_hi_c");
	Value *dh = _builder.CreateFMul(lhs.hi, rhs.hi, "mul_hi_d");
	_iv.hi = cmax4(ah, bh, ch, dh, "mul_hi");

	round_down();
	Value *al = _builder.CreateFMul(lhs.lo, rhs.lo, "mul_lo_a");
	Value *bl = _builder.CreateFMul(lhs.lo, rhs.hi, "mul_lo_b");
	Value *cl = _builder.CreateFMul(lhs.hi, rhs.lo, "mul_lo_c");
	Value *dl = _builder.CreateFMul(lhs.hi, rhs.hi, "mul_lo_d");
	_iv.lo = cmin4(al, bl, cl, dl, "mul_lo");

}

void Compiler::visit(DivExpr &e) {
	VInterval lhs = compile(*e.lhs());
	VInterval rhs = compile(*e.rhs());

	round_up();
	Value *ah = _builder.CreateFDiv(lhs.lo, rhs.lo, "div_hi_a");
	Value *bh = _builder.CreateFDiv(lhs.lo, rhs.hi, "div_hi_b");
	Value *ch = _builder.CreateFDiv(lhs.hi, rhs.lo, "div_hi_c");
	Value *dh = _builder.CreateFDiv(lhs.hi, rhs.hi, "div_hi_d");
	_iv.hi = cmax4(ah, bh, ch, dh, "div_hi");

	round_down();
	Value *al = _builder.CreateFDiv(lhs.lo, rhs.lo, "div_lo_a");
	Value *bl = _builder.CreateFDiv(lhs.lo, rhs.hi, "div_lo_b");
	Value *cl = _builder.CreateFDiv(lhs.hi, rhs.lo, "div_lo_c");
	Value *dl = _builder.CreateFDiv(lhs.hi, rhs.hi, "div_lo_d");
	_iv.lo = cmin4(al, bl, cl, dl, "div_lo");

}

Value *Compiler::cpow(Value *val, int times, string const &name) {
	assert(times > 0);
	Value *res = val;
	for (int i = 1; i < times; ++i) {
		string s = stringize() << name << "_iter_" << i << "_";
		res = _builder.CreateFMul(res, val, s);
	}
	return res;
}

void Compiler::visit(ExptExpr &e) {
	VInterval base = compile(*e.base());
	int expt = e.power();
	bool negative = false;
	if (expt < 0) { negative = true; expt *= -1; }
	if (expt == 0) {
		// [a,b]^0 = 0 \in [a,b] ? [0,1] : [1,1]
		Value *zero = ConstantFP::get(_module->getContext(), APFloat(0.0));
		Value *lo_ltz = _builder.CreateFCmpOLE(base.lo, zero, "expt0_lo_lez");
		Value *hi_gtz = _builder.CreateFCmpOGE(base.hi, zero, "expt0_hi_gez");
		Value *has_zero = _builder.CreateAnd(lo_ltz, hi_gtz, "expt0_base_has_zero");
		Value *one = ConstantFP::get(_module->getContext(), APFloat(1.0));
		_iv.lo = _builder.CreateSelect(has_zero, zero, one);
		_iv.hi = ConstantFP::get(_module->getContext(), APFloat(1.0));
		return;
	} else if (expt&1) {

		round_down();
		_iv.lo = cpow(base.lo, expt, "expt_odd_pow_lo");
		round_up();
		_iv.hi = cpow(base.hi, expt, "expt_odd_pow_hi");

	} else {
		Value *zero = ConstantFP::get(_module->getContext(), APFloat(0.0));
		Value *hi_lz = _builder.CreateFCmpOLT(base.hi, zero, "expt_ev_hi_ltz");
		Function *fn = _builder.GetInsertBlock()->getParent();
		BasicBlock *both_neg = BasicBlock::Create(_module->getContext(), "expt_ev_hi_is_neg", fn);
		BasicBlock *one_pos = BasicBlock::Create(_module->getContext(), "expt_ev_hi_is_pos");
		BasicBlock *both_pos = BasicBlock::Create(_module->getContext(), "expt_ev_both_are_pos");
		BasicBlock *has_zero = BasicBlock::Create(_module->getContext(), "expt_ev_has_zero");
		BasicBlock *merge_block = BasicBlock::Create(_module->getContext(), "expt_ev_done");
		_builder.CreateCondBr(hi_lz, both_neg, one_pos);
		bool negative = false;
		if (expt < 0) { negative = true; expt *= -1; }
		// handle if they're both negative
		_builder.SetInsertPoint(both_neg);
		round_down();
		Value *lo_expt_bneg = cpow(base.hi, expt, "expt_even_bothneg_lo");
		round_up();
		Value *hi_expt_bneg = cpow(base.lo, expt, "expt_even_bothneg_hi");

		// rounding mode is up gooing to merge block
		_builder.CreateBr(merge_block);
		fn->getBasicBlockList().push_back(one_pos);

		// test if the interval has zero

		_builder.SetInsertPoint(one_pos);
		Value *lo_gz = _builder.CreateFCmpOGT(base.lo, zero, "expt_lo_gtz");
		_builder.CreateCondBr(lo_gz, both_pos, has_zero);
		fn->getBasicBlockList().push_back(one_pos);

		// handle when the interval contains zero, e.g. [0, max(a^n, b^n)]
		_builder.SetInsertPoint(has_zero);

		round_up();
		Value *lo_expt_hzero = cpow(base.lo, expt, "expt_even_hz_testlo");
		Value *hi_expt_hzero = cpow(base.hi, expt, "expt_even_hz_testhi");
		hi_expt_hzero = cmax(lo_expt_hzero, hi_expt_hzero, "expt_even_hz_hi");
		lo_expt_hzero = zero;

		// rounding mode is up going into merge block
		_builder.CreateBr(merge_block);
		fn->getBasicBlockList().push_back(has_zero);

		_builder.SetInsertPoint(both_pos);

		round_down();
		Value *lo_expt_pos = cpow(base.lo, expt, "even_pow_bothpos_lo");

		round_up();
		Value *hi_expt_pos = cpow(base.hi, expt, "even_pow_bothpos_hi");

		_builder.CreateBr(merge_block);
		fn->getBasicBlockList().push_back(both_pos);

		_builder.SetInsertPoint(merge_block);

		PHINode *phi_lo = _builder.CreatePHI(Type::getDoubleTy(_module->getContext()), 3, "expt_lo");
		PHINode *phi_hi = _builder.CreatePHI(Type::getDoubleTy(_module->getContext()), 3, "expt_hi");

		phi_lo->addIncoming(lo_expt_bneg, both_neg);
		phi_hi->addIncoming(hi_expt_bneg, both_neg);

		phi_lo->addIncoming(lo_expt_hzero, has_zero);
		phi_hi->addIncoming(hi_expt_hzero, has_zero);

		phi_lo->addIncoming(lo_expt_pos, both_pos);
		phi_hi->addIncoming(hi_expt_pos, both_pos);

		_iv.lo = phi_lo;
		_iv.hi = phi_hi;
		fn->getBasicBlockList().push_back(merge_block);
	}

	if (negative) {
		VInterval iv = cinvert(_iv.lo, _iv.hi, "expt_inv");
		_iv.lo = iv.lo;
		_iv.hi = iv.hi;
	}

}

void Compiler::visit(LitExpr &e) {
	_iv.lo = ConstantFP::get(_module->getContext(), APFloat(e.value().lo()));
	_iv.hi = ConstantFP::get(_module->getContext(), APFloat(e.value().hi()));
}

void Compiler::visit(FuncExpr &e) {
	compile_func(e);
}

Function *Compiler::compile_expr(ExprPtr const &e) {
	if (!e.get()) throw iv_arithmetic_error("Cannot compile null expr!");
	if (FuncExpr const *fe = e->as_func_expr()) return compile_func(*fe);
	return compile_func(FuncExpr(e->clone()));
}

void Compiler::optimize(Function &e) {
	_fpm_ref->run(e);
}


Function *Compiler::compile_func(FuncExpr const &e) {
	vector<Type*> eparms(2*e.params().size(), Type::getDoubleTy(_module->getContext()));
	FunctionType *etype = FunctionType::get(_iv_type, eparms, false);
	Function *efunc = Function::Create(etype, Function::ExternalLinkage, e.name(), _module);
	auto argiter = efunc->arg_begin();
	for (auto const &parm : e.params()) {
		VInterval &v = _named[parm];
		argiter->setName(parm+"_lo");
		v.lo = argiter++;
		argiter->setName(parm+"_hi");
		v.hi = argiter++;
	}
	BasicBlock *bb = BasicBlock::Create(_module->getContext(), "entry", efunc);
	_builder.SetInsertPoint(bb);
	VInterval v = compile(*e.impl());
	if (!v.lo || !v.hi)
		throw iv_arithmetic_error("Error compiling definition of "+e.name()+". Recived null Value interval");

	Value *agg = c_i2v(v, e.name()+"_res");
	_builder.CreateRet(agg);

	verifyFunction(*efunc);
	if (is_optimizing())
		optimize(*efunc);
	return efunc;
}


void Compiler::visit(VarExpr &e) {
	auto i = _named.find(e.name());
	if (i != _named.end()) {
		_iv.lo = i->second.lo;
		_iv.hi = i->second.hi;
	} else throw iv_arithmetic_error("Unbound variable: '"+e.name()+"'");
}

void Compiler::visit(CallExpr &e) {
	Function *callee = _module->getFunction(e.name());
	if (!callee) throw iv_arithmetic_error("Unknown function: '"+e.name()+"'");
	if ((e.name() == "set_rup") || (e.name() == "set_rdown"))
		throw iv_arithmetic_error("Calls to '"+e.name()+"' are prohibited");
	if (callee->arg_size() & 1)
		throw iv_arithmetic_error("Bug: odd number of arguments (internal) to function: '"+e.name()+"'");
	if (callee->arg_size() != 2 * e.args().size())
		throw iv_arithmetic_error(stringize() << "Invalid argument count for call to '" << e.name() << "'. expected: " << (callee->arg_size()/2) << ", got: " << e.args().size());

	vector<Value*> args;
	for (auto const &expr : e.args()) {
		VInterval res = compile(*expr);
		args.push_back(res.lo);
		args.push_back(res.hi);
	}
	Value *res = _builder.CreateCall(callee, args, "call_"+e.name()+"_res");
	vector<unsigned> idxs;
	idxs.push_back(0);
	_iv.lo = _builder.CreateExtractValue(res, idxs);
	idxs.clear();
	idxs.push_back(1);
	_iv.hi = _builder.CreateExtractValue(res, idxs);
}

void *Compiler::jit_to_void(Function *f) {
	return _exec_engine->getPointerToFunction(f);
}

jitted_function Compiler::jit(Function *f) {
	if (f->arg_size() != 0) throw iv_arithmetic_error("JIT: expected f->arg_size == 0");
	// note: UB before c++11. the earlier equivalent cast would be (jitted_function)(intptr_t)(f)
	return reinterpret_cast<jitted_function>(jit_to_void(f));
}

jitted_function Compiler::jit_expr(ExprPtr const &e) {
	if (e->as_func_expr()) throw iv_arithmetic_error("Expected expression in call to jit_expr");
	Function *f = compile_expr(e);
	return jit(f);
}

vector<Value*> Compiler::ivec2vvec(vector<interval> const &v) {
	vector<Value *> vals;
	for (auto const &iv : v) {
		vals.push_back(ConstantFP::get(_module->getContext(), APFloat(iv.lo())));
		vals.push_back(ConstantFP::get(_module->getContext(), APFloat(iv.hi())));
	}
	return vals;
}


interval Compiler::execute(Function *f, vector<interval> const &args) {
	jitted_function fn = jit_call(f, args);
	return from_ret(fn());
}


interval Compiler::execute(string const &s, vector<interval> const &args) {
	jitted_function f = jit_call(s, args);
	return from_ret(f());
}

interval Compiler::execute(ExprPtr const &ep) {
	if (ep->as_func_expr()) {
		compile(*ep);
		return interval::empty();
	}
	jitted_function f = jit_expr(ep);
	return from_ret(f());
}


void Compiler::do_jit(llvm::Function *f) {
	_exec_engine->runJITOnFunction(f);
}

jitted_function Compiler::jit_call(Function *f, vector<interval> const &args) {
	if (f->arg_size() != 2 * args.size()) throw iv_arithmetic_error("Wrong number of args to function");
	vector<Type*> empty_argl;
	FunctionType *type = FunctionType::get(_iv_type, empty_argl, false);
	Function *wrapper = Function::Create(type, Function::ExternalLinkage, "", _module);
	BasicBlock *bb = BasicBlock::Create(_module->getContext(), "wrapper_start", wrapper);
	_builder.SetInsertPoint(bb);
	vector<Value*> val_args = ivec2vvec(args);
	Value *res = _builder.CreateCall(f, val_args);
	_builder.CreateRet(res);
	verifyFunction(*wrapper);
	if (is_optimizing())
		optimize(*wrapper); // nfc what this could do.  maybe inline?
//	Type *wrapty = wrapper->getReturnType();
//	cout << "wrapty: ";
//	wrapty->dump();
//	cout << endl;
	return jit(wrapper);
}

jitted_function Compiler::jit_call(string const &name, vector<interval> const &args) {
	Function *f = _module->getFunction(name);
	if (!f) throw iv_arithmetic_error("Unknown function: '"+name+"'");
	return jit_call(f, args);
}


VInterval Compiler::compile(Expr &e) {
	_iv.lo = _iv.hi = nullptr;
	e.accept(*this);
	return _iv;
}



PartialComp::PartialComp() : _ctx(nullptr) {}

PartialComp::PartialComp(Compiler *c) : _ctx(c) {}

PartialComp::PartialComp(Compiler *c, FuncExpr &fe) : _ctx(c), _pnames(fe.params()) { initialize(fe); }

PartialComp &PartialComp::operator=(PartialComp const &pc) {
	_ctx = pc._ctx;
	_fname = pc._fname;
	_pnames = pc._pnames;
	_pf_names = pc._pf_names;
	_funcs = pc._funcs;
	for (auto const &ep : pc._fpartials)
		_fpartials.push_back(ep->clone());
	EXPECT(_ctx, "Error: PartialComp initialized with null compiler!");
	return *this;
}

void PartialComp::initialize(FuncExpr const &fe) {
	EXPECT(_ctx, "Error: PartialComp initialized with null compiler!");
	string deriv_prefix = stringize() << "d" << _fname << "/d";
	_pf_names.push_back(fe.name());
	ExprPtr ffe = Expr::make_func(fe.name(), fe.params(), Simplifier::simplified(fe.impl()));
	_fpartials.push_back(move(ffe));
	Function *f = _ctx->compile_func(*_fpartials.back()->as_func_expr());
	_funcs.push_back(f);
	for (auto const &i : Derivator::partials(fe)) {
		_pf_names.push_back(deriv_prefix + i.first);
		ExprPtr pfe = Expr::make_func(_pf_names.back(), _pnames, Simplifier::simplified(i.second));
		_fpartials.push_back(move(pfe));
		Function *pf = _ctx->compile_func(*_fpartials.back()->as_func_expr());
		_ctx->do_jit(pf);
		_funcs.push_back(pf);
	}
}



vector<interval> PartialComp::calculate(vector<interval> const &args) {
	vector<interval> res;
	// idea: generate a function which takes a function as an argument,
	// and calls it with `args`, then call that function wich each
	// Function in _funcs.
	for (auto f : _funcs)
		res.push_back(_ctx->execute(f, args));
	return res;
}




}