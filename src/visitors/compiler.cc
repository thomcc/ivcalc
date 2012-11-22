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
#include <llvm/ExecutionEngine/GenericValue.h>


//#define VOLATILE_FESETROUND


namespace calc {

using namespace llvm;
using namespace std;

Compiler::Compiler()
: _iv{nullptr, nullptr}
, _module(new Module("interval jit", getGlobalContext()))
, _builder(_module->getContext())
, _round_mode(RoundMode::Unknown)
, _fpm(_module)
, _optimizing(true)
, _verbose(0)
, _vd(0) {
	init_module();
}


Compiler::~Compiler() { delete _module; }

void Compiler::init_module() {

	InitializeNativeTarget();
	EngineBuilder eb{_module};
	TargetOptions tgopts;
	tgopts.HonorSignDependentRoundingFPMathOption = 1;
	eb.setTargetOptions(tgopts);
	eb.setEngineKind(EngineKind::JIT);
	_exec_engine = eb.create();
	_fpm.add(new TargetData(*_exec_engine->getTargetData()));

//	_fpm.add(createBasicAliasAnalysisPass());
//	_fpm.add(createInstructionCombiningPass());
//	_fpm.add(createReassociatePass());
//	_fpm.add(createGVNPass());
//	_fpm.add(createCFGSimplificationPass());

	_fpm.add(createCFGSimplificationPass());
	_fpm.add(createPromoteMemoryToRegisterPass());

	//_fpm.add(createInstructionCombiningPass());
	_fpm.add(createScalarReplAggregatesPass());
	//_fpm.add(createInstructionCombiningPass());
	_fpm.add(createJumpThreadingPass());
	_fpm.add(createCFGSimplificationPass());
	//_fpm.add(createInstructionCombiningPass());

	//_fpm.add(createCFGSimplificationPass());
	_fpm.add(createReassociatePass());

#if defined(LLVM_VERSION_MAJOR) && LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR >= 1
	//_fpm.add(createBBVectorizePass());
#endif
	_fpm.add(createEarlyCSEPass());

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
	//_fpm.add(createMemCpyOptPass());
	_fpm.add(createSCCPPass());

	//_fpm.add(createSinkingPass());
	//_fpm.add(createInstructionSimplifierPass());
	_fpm.add(createInstructionCombiningPass());
	_fpm.add(createJumpThreadingPass());
	_fpm.add(createDeadStoreEliminationPass());

	_fpm.add(createAggressiveDCEPass());
	_fpm.add(createCFGSimplificationPass());

	_fpm.doInitialization();
	_fpm_ref = &_fpm;

	Type *inttype = Type::getInt32Ty(_module->getContext());
	Type *oneint[1] = {inttype};
	FunctionType *intint = FunctionType::get(inttype, oneint, false);

	FunctionType *voidvoid = FunctionType::get(Type::getVoidTy(_module->getContext()), false);
	Constant *fsr = _module->getOrInsertFunction("fesetround", intint);
	Constant *ru = _module->getOrInsertFunction("set_rounding_mode_up", voidvoid);
	Constant *rd = _module->getOrInsertFunction("set_rounding_mode_down", voidvoid);

	_round_up = cast<Function>(ru);
	_round_down = cast<Function>(rd);
	_fesetrnd = cast<Function>(fsr);
	vector<Type*> dbls(2, Type::getDoubleTy(_module->getContext()));
	_iv_type = StructType::get(_module->getContext(), dbls, false);

	vector<Type*> ivs(2, _iv_type);
	FunctionType *iv_op = FunctionType::get(_iv_type, ivs, false);
	Constant *ivadd = _module->getOrInsertFunction("iv_add", iv_op);
	Constant *ivsub = _module->getOrInsertFunction("iv_sub", iv_op);
	Constant *ivmul = _module->getOrInsertFunction("iv_mul", iv_op);
	Constant *ivdiv = _module->getOrInsertFunction("iv_div", iv_op);
	Type *powargs[2] = {_iv_type, Type::getInt32Ty(_module->getContext())};
	FunctionType *iv_pow_type = FunctionType::get(_iv_type, powargs, false);
	Constant *ivpow = _module->getOrInsertFunction("iv_pow", iv_pow_type);
	_ivadd = cast<Function>(ivadd);
	_ivsub = cast<Function>(ivsub);
	_ivdiv = cast<Function>(ivdiv);
	_ivmul = cast<Function>(ivmul);
	_ivpow = cast<Function>(ivpow);
//ivvmul
//ivvsub
//ivvadd
//ivvdiv

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
	std::vector<Type*> args(nargs, _iv_type);
	FunctionType *builtin_type = FunctionType::get(_iv_type, args, false);
	Constant *cf = _module->getOrInsertFunction(name, builtin_type);
	Function *f = cast<Function>(cf);
	if (_builtin.find(alias) != _builtin.end())
		throw iv_arithmetic_error("BUG: builtin duplicated!");
	_builtin[alias] = f;
}

void Compiler::round_up(bool force) {
	if (force || (_round_mode != RoundMode::Up)) {
		Type *i32 = Type::getInt32Ty(_module->getContext());
#ifdef VOLATILE_FESETROUND
		Value *mem = _builder.CreateAlloca(i32, 0, "rup_forcer");
		_builder.CreateLifetimeStart(mem);
#endif
		Value *fearg[1] = {ConstantInt::get(i32, FE_UPWARD)};
		Value *v = _builder.CreateCall(_fesetrnd, fearg, "rup_force_unused");
		(void)v;
#ifdef VOLATILE_FESETROUND
		_builder.CreateStore(v, mem, true);
		_builder.CreateLifetimeEnd(mem);
//		_builder.CreateCall(_round_up);
		_round_mode = RoundMode::Up;
#endif
	}
}

void Compiler::round_down(bool force) {

	if (force || (_round_mode != RoundMode::Down)) {
		Type *i32 = Type::getInt32Ty(_module->getContext());
#ifdef VOLATILE_FESETROUND

		Value *mem = _builder.CreateAlloca(i32, 0, "rdown_forcer");
		_builder.CreateLifetimeStart(mem);
#endif
		Value *fearg[1] = {ConstantInt::get(i32, FE_DOWNWARD)};
		Value *v = _builder.CreateCall(_fesetrnd, fearg, "rdown_force_unused");
		(void)v;
#ifdef VOLATILE_FESETROUND
		_builder.CreateStore(v, mem, true);
		_builder.CreateLifetimeEnd(mem);
//		_builder.CreateCall(_round_down);
		_round_mode = RoundMode::Down;
#endif
	}

}
void Compiler::branched() {
	_round_mode = RoundMode::Unknown;
}

Value *Compiler::cforce_round(Value *a, string const &name) {
	Value *mem = _builder.CreateAlloca(Type::getDoubleTy(_module->getContext()), 0, name+"_fr_mem");
	_builder.CreateLifetimeStart(mem);
	_builder.CreateStore(a, mem, true);
	Value *rounded = _builder.CreateLoad(mem, true, name+"fr_rounded");
	_builder.CreateLifetimeEnd(mem);
	return rounded;
}

Value *Compiler::cadd_lo(Value *a, Value *b, const char *name) {
	round_down();
	return cforce_round(_builder.CreateFAdd(a, b, name));
}

Value *Compiler::cadd_hi(Value *a, Value *b, const char *name) {
	round_up();
	return cforce_round(_builder.CreateFAdd(a, b, name));
}

Value *Compiler::csub_lo(Value *a, Value *b, const char *name) {
	round_down();
	return cforce_round(_builder.CreateFSub(a, b, name));
}

Value *Compiler::csub_hi(Value *a, Value *b, const char *name) {
	round_up();
	return cforce_round(_builder.CreateFSub(a, b, name));
}

Value *Compiler::cmul_lo(Value *a, Value *b, const char *name) {
	round_down();
	return cforce_round(_builder.CreateFMul(a, b, name));
}

Value *Compiler::cmul_hi(Value *a, Value *b, const char *name) {
	round_up();
	return cforce_round(_builder.CreateFMul(a, b, name));
}

Value *Compiler::cdiv_lo(Value *a, Value *b, const char *name) {
	round_down();
	return cforce_round(_builder.CreateFDiv(a, b, name));
}

Value *Compiler::cdiv_hi(Value *a, Value *b, const char *name) {
	round_up();
	return cforce_round(_builder.CreateFDiv(a, b, name));
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
	Value *ah = cforce_round(_builder.CreateFDiv(one, lo, name+"_hi_a"));
	Value *bh = cforce_round(_builder.CreateFDiv(one, hi, name+"_hi_b"));
	ret.hi = cmax(ah, bh, name+"_hi");

	round_down();
	Value *al = cforce_round(_builder.CreateFDiv(one, lo, name+"_lo_a"));
	Value *bl = cforce_round(_builder.CreateFDiv(one, hi, name+"_lo_b"));
	ret.lo = cmin(al, bl, name+"_lo");

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
	int vv = ++_vd;
	if (_verbose > 1) cout << "Compiler: ADD (" << vv << ")" << endl;
	if (_verbose > 2) cout << "Compiler: ADD lhs (" << vv << ")" << endl;
	VInterval lhs = compile(*e.lhs());
	if (_verbose > 2) cout << "Compiler: ADD rhs (" << vv << ")" << endl;
	VInterval rhs = compile(*e.rhs());
//	_iv.lo = cadd_lo(lhs.lo, rhs.lo);
//	_iv.hi = cadd_hi(lhs.hi, rhs.hi);
	_iv = Compiler::opcall(_ivadd, lhs, rhs, "addcall");
	if (_verbose > 2) cout << "Compiler: ADD done (" << vv << ")" << endl;
	--_vd;

}

void Compiler::visit(SubExpr &e) {
	int vv = ++_vd;
	if (_verbose > 1) cout << "Compiler: SUB (" << vv << ")" << endl;
	if (_verbose > 2) cout << "Compiler: SUB lhs (" << vv << ")" << endl;
	VInterval lhs = compile(*e.lhs());
	if (_verbose > 2) cout << "Compiler: SUB rhs (" << vv << ")" << endl;
	VInterval rhs = compile(*e.rhs());
	_iv = Compiler::opcall(_ivsub, lhs, rhs, "subcall");
//	_iv.lo = csub_lo(lhs.lo, rhs.hi);
//	_iv.hi = csub_hi(lhs.hi, rhs.lo);
	if (_verbose > 2) cout << "Compiler: SUB done (" << vv << ")" << endl;
	--_vd;
}

void Compiler::visit(NegExpr &e) {
	int vv = ++_vd;
	if (_verbose > 1) cout << "Compiler: NEG (" << vv << ")" << endl;
	if (_verbose > 2) cout << "Compiler: NEG value (" << vv << ")" << endl;
	VInterval val = compile(*e.value());
	_iv.lo = _builder.CreateFNeg(val.hi, "neg_lo");
	_iv.hi = _builder.CreateFNeg(val.lo, "neg_hi");
	if (_verbose > 2) cout << "Compiler: NEG done (" << vv << ")" << endl;
	--_vd;
}


void Compiler::visit(MulExpr &e) {
	int vv = ++_vd;
	if (_verbose > 1) cout << "Compiler: MUL (" << vv << ")" << endl;
	if (_verbose > 2) cout << "Compiler: MUL lhs (" << vv << ")" << endl;
	VInterval lhs = compile(*e.lhs());
	if (_verbose > 2) cout << "Compiler: MUL rhs (" << vv << ")" << endl;
	VInterval rhs = compile(*e.rhs());
	// todo: consider optimizing in a manner similar to interval.cc
	_iv = Compiler::opcall(_ivmul, lhs, rhs, "mulcall");

//
//	round_up();
//	Value *ah = cforce_round(_builder.CreateFMul(lhs.lo, rhs.lo, "mul_hi_a"));
//	Value *bh = cforce_round(_builder.CreateFMul(lhs.lo, rhs.hi, "mul_hi_b"));
//	Value *ch = cforce_round(_builder.CreateFMul(lhs.hi, rhs.lo, "mul_hi_c"));
//	Value *dh = cforce_round(_builder.CreateFMul(lhs.hi, rhs.hi, "mul_hi_d"));
//	_iv.hi = cmax4(ah, bh, ch, dh, "mul_hi");
//
//	round_down();
//	Value *al = cforce_round(_builder.CreateFMul(lhs.lo, rhs.lo, "mul_lo_a"));
//	Value *bl = cforce_round(_builder.CreateFMul(lhs.lo, rhs.hi, "mul_lo_b"));
//	Value *cl = cforce_round(_builder.CreateFMul(lhs.hi, rhs.lo, "mul_lo_c"));
//	Value *dl = cforce_round(_builder.CreateFMul(lhs.hi, rhs.hi, "mul_lo_d"));
//	_iv.lo = cmin4(al, bl, cl, dl, "mul_lo");
	if (_verbose > 2) cout << "Compiler: MUL done (" << vv << ")" << endl;
	--_vd;
}

void Compiler::visit(DivExpr &e) {
	int vv = ++_vd;
	if (_verbose > 1) cout << "Compiler: DIV (" << vv << ")" << endl;
	if (_verbose > 2) cout << "Compiler: DIV lhs (" << vv << ")" << endl;

	VInterval lhs = compile(*e.lhs());
	if (_verbose > 2) cout << "Compiler: DIV rhs (" << vv << ")" << endl;

	VInterval rhs = compile(*e.rhs());

	_iv = Compiler::opcall(_ivdiv, lhs, rhs, "divcall");

//
//	round_up();
//	Value *ah = cforce_round(_builder.CreateFDiv(lhs.lo, rhs.lo, "div_hi_a"));
//	Value *bh = cforce_round(_builder.CreateFDiv(lhs.lo, rhs.hi, "div_hi_b"));
//	Value *ch = cforce_round(_builder.CreateFDiv(lhs.hi, rhs.lo, "div_hi_c"));
//	Value *dh = cforce_round(_builder.CreateFDiv(lhs.hi, rhs.hi, "div_hi_d"));
//	_iv.hi = cmax4(ah, bh, ch, dh, "div_hi");
//
//	round_down();
//	Value *al = cforce_round(_builder.CreateFDiv(lhs.lo, rhs.lo, "div_lo_a"));
//	Value *bl = cforce_round(_builder.CreateFDiv(lhs.lo, rhs.hi, "div_lo_b"));
//	Value *cl = cforce_round(_builder.CreateFDiv(lhs.hi, rhs.lo, "div_lo_c"));
//	Value *dl = cforce_round(_builder.CreateFDiv(lhs.hi, rhs.hi, "div_lo_d"));
//	_iv.lo = cmin4(al, bl, cl, dl, "div_lo");
	if (_verbose > 2) cout << "Compiler: DIV done (" << vv << ")" << endl;
	--_vd;
}

Value *Compiler::ceq0test(Value *lo, Value *hi, string const &name) {
	Value *zero = ConstantFP::get(_module->getContext(), APFloat(0.0));
	Value *lo_is0 = _builder.CreateFCmpUEQ(lo, zero, name+"_lo_is_zero");
	Value *hi_is0 = _builder.CreateFCmpUEQ(hi, zero, name+"_hi_is_zero");
	return _builder.CreateAnd(lo_is0, hi_is0, name+"_is_zero");
}

Value *Compiler::clt0test(Value *lo, Value *hi, string const &name) {
	UNUSED(lo);
	return _builder.CreateFCmpOLT(hi, zero(), name);
}

Value *Compiler::cgt0test(Value *lo, Value *hi, string const &name) {
	UNUSED(hi);
	return _builder.CreateFCmpOGT(lo, zero(), name);
}

Value *Compiler::chas0test(Value *lo, Value *hi, string const &name) {
	Value *lo_ltz = _builder.CreateFCmpOLE(lo, zero(), name+"_lo_lez");
	Value *hi_gtz = _builder.CreateFCmpOGE(hi, zero(), name+"_hi_gez");
	return _builder.CreateAnd(lo_ltz, hi_gtz, name+"_has_zero");
}

Value *Compiler::cemptytest(Value *lo, Value *hi, string const &name) {
	return _builder.CreateFCmpUNO(lo, hi, name);
}

Value *Compiler::zero() {
	return get_dbl(0);
}

Value *Compiler::get_dbl(double d) {
	return ConstantFP::get(_module->getContext(), APFloat(d));
}

Value *Compiler::cpow(Value *val, int pwr, string const &name) {
	Value *x = val;
	Value *y = (pwr & 1) ? x : get_dbl(1);
	pwr >>= 1;
	while (pwr > 0) {
		x = cforce_round(_builder.CreateFMul(x, x, name+"_unroll_x"));
		if (pwr & 1) y = cforce_round(_builder.CreateFMul(x, y, name+"pwr_do_y"));
		pwr >>= 1;
	}
	return y;
}

void Compiler::visit(ExptExpr &e) {
	int vv = ++_vd;
	if (_verbose > 1) cout << "Compiler: EXPT ^ " << e.power() << " (" << vv << ")" << endl;
	if (_verbose > 2) cout << "Compiler: EXPT ^ " << e.power() << " value (" << vv << ")" << endl;

	VInterval base = compile(*e.base());
	int expt = e.power();
	if (expt == 0) {
		Value *isNaN = cemptytest(base.lo, base.hi, "expt_zero");
		Value *isZero = ceq0test(base.lo, base.hi, "expt_zero");
		Value *retnan = _builder.CreateOr(isZero, isNaN, "expt_zero_result_empty");
		_iv.lo = _builder.CreateSelect(retnan, get_dbl(rmath::NaN()), get_dbl(1), "expt_zero_res_lo");
		_iv.hi = _builder.CreateSelect(retnan, get_dbl(rmath::NaN()), get_dbl(1), "expt_zero_res_hi");
	} else {
		Value *bbox = c_i2v(base, "expt_base_box");
		Value *pow = ConstantInt::get(Type::getInt32Ty(_module->getContext()), expt);
		Value *cres = _builder.CreateCall2(_ivpow, bbox, pow, "expt_res");
		unsigned idx0[1] = {0};
		unsigned idx1[1] = {1};
		_iv.lo = _builder.CreateExtractValue(cres, idx0, "expt_res_hi");
		_iv.hi = _builder.CreateExtractValue(cres, idx1, "expt_res_lo");



//		bool negative = false;
//		Value *reslo = nullptr, *reshi = nullptr;
//		if (expt < 0) { negative = true; expt *= -1; }
//		if (expt & 1) {
//			round_down();
//			reslo = cpow(base.lo, e.power(), "expt_odd_lo");
//			round_up();
//			reshi = cpow(base.hi, e.power(), "expt_odd_hi");
//		} else {
//			Value *test = _builder.CreateFCmpOGT(base.lo, zero(), "expt_even_are_both_pos_p");
//			Function *fn = _builder.GetInsertBlock()->getParent();
//
//			BasicBlock *pos_block = BasicBlock::Create(_module->getContext(), "expt_even_both_are_pos", fn);
//			BasicBlock *test_block = BasicBlock::Create(_module->getContext(), "expt_even_both_are_not_pos");
//
//			BasicBlock *neg_block = BasicBlock::Create(_module->getContext(), "expt_even_both_are_neg");
//			BasicBlock *mid_block = BasicBlock::Create(_module->getContext(), "expt_even_contains_zero");
//			BasicBlock *merge_block = BasicBlock::Create(_module->getContext(), "expt_even_merge");
//			round_up(); // ensure that we're rounding up at the start of each block. no_op if this is alread the case
//
//			_builder.CreateCondBr(test, pos_block, test_block);
//
//			// no need to push the block list back because fn was a parameter to BasicBlock::Create
//			_builder.SetInsertPoint(pos_block); // both are pos
//			Value *pos_block_res_lo = nullptr, *pos_block_res_hi = nullptr;
//			{
//				branched();
//				round_down();
//				pos_block_res_lo = cpow(base.lo, e.power(), "expt_even_pos_block_res_lo");
//				round_up();
//				pos_block_res_hi = cpow(base.hi, e.power(), "expt_even_pos_block_res_hi");
//
//				_builder.CreateBr(merge_block);
//			}
//
//			fn->getBasicBlockList().push_back(test_block);
//			_builder.SetInsertPoint(test_block); // lo < 0, dont know about hi.
//			{
//				branched();
//				Value *test2 = _builder.CreateFCmpOLT(base.hi, zero(), "expt_even_are_both_neg_p");
//
//				_builder.CreateCondBr(test2, neg_block, mid_block);
//
//			}
//
//			fn->getBasicBlockList().push_back(neg_block);
//			_builder.SetInsertPoint(neg_block); // both are neg
//			Value *neg_block_res_lo = nullptr, *neg_block_res_hi = nullptr;
//			{
//				branched();
//				round_down();
//				neg_block_res_lo = cpow(base.hi, e.power(), "expt_even_neg_block_res_lo");
//				round_up();
//				neg_block_res_hi = cpow(base.lo, e.power(), "expt_even_pos_block_res_hi");
//
//				_builder.CreateBr(merge_block);
//			}
//
//			fn->getBasicBlockList().push_back(mid_block);
//			_builder.SetInsertPoint(mid_block); // interval contains zero
//			Value *mid_block_res_lo = nullptr, *mid_block_res_hi = nullptr;
//			{
//				branched();
//				round_up();
//				mid_block_res_lo = zero();
//				Value *lo_pow = cpow(base.lo, e.power(), "expt_even_mid_block_lo_");
//				Value *hi_pow = cpow(base.hi, e.power(), "expt_even_mid_block_hi_");
//				mid_block_res_hi = cmax(lo_pow, hi_pow, "expt_even_mid_block_res_hi");
//				_builder.CreateBr(merge_block);
//			}
//
//			fn->getBasicBlockList().push_back(merge_block);
//			_builder.SetInsertPoint(merge_block); // last block, creates phi nodes
//
//			PHINode *phi_lo = _builder.CreatePHI(Type::getDoubleTy(_module->getContext()), 3, "expt_lo");
//			PHINode *phi_hi = _builder.CreatePHI(Type::getDoubleTy(_module->getContext()), 3, "expt_hi");
//
//			phi_lo->addIncoming(pos_block_res_lo, pos_block);
//			phi_hi->addIncoming(pos_block_res_hi, pos_block);
//
//			phi_lo->addIncoming(neg_block_res_lo, neg_block);
//			phi_hi->addIncoming(neg_block_res_hi, neg_block);
//
//			phi_lo->addIncoming(mid_block_res_lo, mid_block);
//			phi_hi->addIncoming(mid_block_res_hi, mid_block);
//
//			// we don't want to assign to _iv yet because we might still need to invert.
//			reslo = phi_lo;
//			reshi = phi_hi;
//		}
//		if (negative) {
//			branched();
//			_iv = cinvert(reslo, reshi, "expt_invert_result");
//		} else {
//			_iv.lo = reslo;
//			_iv.hi = reshi;
//		}
	}
	if (_verbose > 2) cout << "Compiler: EXPT ^ " << e.power() << " done (" << vv << ")" << endl;
	--_vd;
}

void Compiler::visit(LitExpr &e) {
	if (_verbose > 1) cout << "Compiler: LIT " << e.value() << endl;
	_iv.lo = ConstantFP::get(_module->getContext(), APFloat(e.value().lo()));
	_iv.hi = ConstantFP::get(_module->getContext(), APFloat(e.value().hi()));
}

void Compiler::visit(FuncExpr &e) {
	int vv = ++_vd;
	if (_verbose > 1) cout << "Compiler: FUNC " << e.name() << " (" << vv << ")" << endl;
	compile_1func(e);
	if (_verbose > 1) cout << "Compiler: FUNC done " << e.name() << " (" << vv << ")" << endl;
	_iv.lo = ConstantFP::get(_module->getContext(), APFloat(rmath::NaN()));
	_iv.hi = ConstantFP::get(_module->getContext(), APFloat(rmath::NaN()));
	--_vd;
}

Function *Compiler::compile_expr(ExprPtr const &e) {
	if (!e.get()) throw iv_arithmetic_error("Cannot compile null expr!");
	if (FuncExpr const *fe = e->as_func_expr()) return compile_func(*fe);
	return compile_func(FuncExpr(e->clone()));
}

void Compiler::optimize(Function &e) {
	if (_verbose > 0) cout << "Compiler: OPTIMIZE" << endl;
	_fpm_ref->run(e);
	if (_verbose > 0) cout << "Compiler: OPTIMIZE done" << endl;
}

VInterval Compiler::opcall(Function *op, VInterval const &a, VInterval const &b, string const &name) {
	Value *lbox = c_i2v(a, name+"_lhs");
	Value *rbox = c_i2v(b, name+"_rhs");
	Value *cres = _builder.CreateCall2(op, lbox, rbox, name+"_res");
	unsigned idx0[1] = {0};
	unsigned idx1[1] = {1};
	VInterval res;
	res.lo = _builder.CreateExtractValue(cres, idx0, name+"_res_hi");
	res.hi = _builder.CreateExtractValue(cres, idx1, name+"_res_lo");
	return res;
}


Function *Compiler::compile_func_partials(FuncExpr const &e) {
	int vv = ++_vd;
	if (_verbose > 0) cout << "Compiler: PARTIALS OF " << e.name() << " (" << vv << ")" << endl;
	vector<Type*> eparms(2, Type::getDoublePtrTy(_module->getContext()));
	// takes a vector of <src, dst>
	FunctionType *etype = FunctionType::get(Type::getVoidTy(_module->getContext()), eparms, false);
	Function *efunc = Function::Create(etype, Function::ExternalLinkage, e.name()+"_partials", _module);
	BasicBlock *bb = BasicBlock::Create(_module->getContext(), "entry", efunc);
	auto argiter = efunc->arg_begin();
	argiter->setName("dst");
	Value *ary_dest = argiter++;
	argiter->setName("src");
	Type *i64 = Type::getInt64Ty(_module->getContext());
	Value *ary_src = argiter++;
	size_t idx = 0;
	_builder.SetInsertPoint(bb);

	// add everything to the _named map.
	for (auto const &parm : e.params()) {
		VInterval &v = _named[parm];
		Value *gepmem = _builder.CreateConstGEP1_64(ary_src, idx++, parm+"_lo_gep");
		v.lo = _builder.CreateLoad(gepmem, parm+"_lo");
		Value *gepmem2 = _builder.CreateConstGEP1_64(ary_src, idx++, parm+"_hi_gep");
		v.hi = _builder.CreateLoad(gepmem2, parm+"_hi");
	}
	if (_verbose > 0) cout << "Compiler: PARTIALS OF " << e.name() << " COMPILING ORIGINAL (" << vv << ")" << endl;
	// compile the function itself
	VInterval v = compile(*e.impl());
	if (!v.lo || !v.hi)
		throw iv_arithmetic_error("Error during compilation of "+e.name()+" got null VInterval from compiler");

	idx = 0;
	// compile storing it in the dest array
	Value *gep_dst_meml = _builder.CreateConstGEP1_64(ary_dest, idx++, e.name()+"_orig_res_loptr");
	_builder.CreateStore(v.lo, gep_dst_meml);
	Value *gep_dst_memh = _builder.CreateConstGEP1_64(ary_dest, idx++, e.name()+"_orig_res_hiptr");
	_builder.CreateStore(v.hi, gep_dst_memh);
	// compile each of the partials, and their storing in the dest array.
	for (auto const &part : Derivator::partials(e)) {
//		vector<pair<string, ExprPtr>> partials = Derivator::partials(e);

		std::string const &n = part.first;
		if (_verbose > 0) cout << "Compiler: PARTIALS OF " << e.name() << " COMPILING " << part.first << " (" << vv << ")" << endl;
		VInterval v = compile(*Simplifier::simplified(part.second));
		if (!v.lo || !v.hi)
			throw iv_arithmetic_error("Error during compilation of d"+n+"/d"+e.name()+" got null VInterval from compiler");

		Value *gep_dst_meml = _builder.CreateGEP(ary_dest, ConstantInt::get(i64, idx++), e.name()+"_d"+n+"_res_loptr");
		_builder.CreateStore(v.lo, gep_dst_meml);

		Value *gep_dst_memh = _builder.CreateGEP(ary_dest, ConstantInt::get(i64, idx++), e.name()+"_d"+n+"_res_hiptr");
		_builder.CreateStore(v.hi, gep_dst_memh);

	}
	_builder.CreateRetVoid();
	// check, optimize, return.
	verifyFunction(*efunc);
	if (is_optimizing())
		optimize(*efunc);
	if (_verbose > 0)
		cout << "Compiler: PARTIALS OF " << e.name() << " DONE (" << vv << ")" << endl;
	--_vd;
	return efunc;
}

Function *Compiler::compile_1func(FuncExpr const &e) {
	int vv = ++_vd;
	if (_verbose > 1) cout << "Compiler: COMPILING SINGLE FUNCTION " << e.name() << " (" << vv << ")" << endl;
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
	if (_verbose > 1) cout << "Compiler: COMPILED SINGLE FUNCTION " << e.name() << " (" << vv << ")" << endl;
	--_vd;
	return efunc;
}

FuncCode::~FuncCode() {
	compiled->eraseFromParent();
	for (auto &i : partials)
		if (i.second.get())
			i.second->function->eraseFromParent();
	allpartials->eraseFromParent();
}

Function *Compiler::compile_func(FuncExpr const &e) {
	int vv = ++_vd;
	if (_verbose > 0) cout << "Compiler: COMPILING FUNCTION AND ALL PARTIALS TOGETHER AND SEPARATELY " << e.name() << " (" << vv << ")" << endl;

	shared_ptr<FuncCode> fc{new FuncCode};
//	FuncCode &fc = _compiled[e.name()];
	auto it = _compiled.find(e.name());
	if (it != _compiled.end()) {
		// todo: find something reasonable to do here...
		std::cerr << "Warning: redefining function '" << e.name() << "'." << endl;
	}
	fc->self = Simplifier::simplified(e.clone());
	if (_verbose > 0) cout << "Compiler: COMPILING JUST FUNCTION  " << e.name() << " (" << vv << ")" << endl;
	fc->compiled = compile_1func(*fc->self->as_func_expr());
	if (_verbose > 0) cout << "Compiler: COMPILING ALL PARTIALS " << e.name() << " (" << vv << ")" << endl;
	fc->allpartials = compile_func_partials(*fc->self->as_func_expr());
	size_t i = 0;
	for (auto const &part : Derivator::partials(e)) {
		std::string name = "d_"+part.first+"_d_"+e.name();
		if (_verbose > 0) cout << "Compiler: COMPILING SEPARATE PARTIAL  " << name << " (" << vv << ")" << endl;
		ExprPtr s = Expr::make_func(name, e.params(), Simplifier::simplified(part.second));
		//FuncCode::Partial &fcp = fc->partials[name];
		unique_ptr<FuncCode::Partial> fcp{new FuncCode::Partial};
		fcp->idx = i++;
		fcp->function = compile_1func(*s->as_func_expr());
		fcp->param = part.first;
		fcp->func_name = name;
		fcp->expr = move(s);
		fc->partials[name] = move(fcp);
	}
	if (_verbose > 0) cout << "Compiler: DONE COMPILING FUNCTION AND ALL PARTIALS TOGETHER AND SEPARATELY " << e.name() << " (" << vv << ")" << endl;
	--_vd;
	return fc->compiled;
}

void Compiler::visit(VarExpr &e) {
	auto i = _named.find(e.name());
	if (i != _named.end()) {
		_iv.lo = i->second.lo;
		_iv.hi = i->second.hi;
	} else throw iv_arithmetic_error("Unbound variable: '"+e.name()+"'");
}

void Compiler::visit(CallExpr &e) {
	// this is a cludge.  ideally, compiled functions would have the
	// same, uh, 'calling convention', for lack of a better term
	// than builtin functions, however due to necessary changes in how
	// compiled functions are represented, this is not the case.
	//
	// builtins take some number of pod_intervals, and return a pod_interval.
	// compiled functions take two pointers to arrays of doubles (dst, src),
	// with the actual function arguments in src, and store the results in dst.
	int vv = ++_vd;
	if (_verbose > 1) cout << "Compiler: CALL " << e.name() << " (" << vv << ")" << endl;
	auto it = _builtin.find(e.name());
	if (it != _builtin.end()) {
		if (_verbose > 2) cout << "Compiler: CALL (prim) " << e.name() << " (" << vv << ")" << endl;
		llvm::Function *callee = it->getValue();
		vector<Value*> args;
		for (auto const &expr : e.args())
			args.push_back(c_i2v(compile(*expr), "call_arg_"));
		Value *res = _builder.CreateCall(callee, args, "call_"+e.name()+"_res");
		vector<unsigned> idxs;
		idxs.push_back(0);
		_iv.lo = _builder.CreateExtractValue(res, idxs);
		idxs.clear();
		idxs.push_back(1);
		_iv.hi = _builder.CreateExtractValue(res, idxs);
	} else {
		if (_verbose > 1) cout << "Compiler: CALL (udf) " << e.name() << " (" << vv << ")" << endl;
		auto it = _compiled.find(e.name());
//		Function *callee = _module->getFunction(e.name());
		if (it == _compiled.end()) throw iv_arithmetic_error("Unknown function: '"+e.name()+"'");
		Function *callee = it->getValue()->compiled;
		if (callee->arg_size() & 1)
			throw iv_arithmetic_error("Bug: odd number of arguments (internal) to function: '"+e.name()+"'");
		if (callee->arg_size() != 2 * e.args().size())
			throw iv_arithmetic_error(stringize() << "Invalid argument count for call to '"+e.name()+"'. expected: "
			                                      << (callee->arg_size()/2)
			                                      << ", got: " << e.args().size());

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
	if (_verbose > 1)
		cout << "Compiler: CALL done " << e.name() << " (" << vv << ")" << endl;
	--_vd;
}

void *Compiler::jit_to_void(Function *f) {
	return _exec_engine->getPointerToFunction(f);
}

vector<interval> Compiled::operator()(vector<interval> const &v) {
	if (v.size() != _nargs)
		throw iv_arithmetic_error(stringize() << "Wrong number of arguments to compiled function. expected: " << _nargs << " got: " << v.size());
	assert(_jitted != nullptr);
	size_t dsz = 2 * v.size();
	std::vector<interval> result;
	result.reserve(v.size());
	size_t i = 0;
	for (auto const &e : v) { _dbls[i++] = e.lo(); _dbls[i++] = e.hi(); }
	memset(&_retdbls[0], '\0', sizeof(double) * dsz);
	_jitted(_retdbls.get(), _dbls.get());
	rmath::set_rnear();
	for (i = 0; i < dsz; i += 2)
		result.push_back(interval(_dbls[i], _dbls[i+1]));
	return result;
}

Compiled Compiler::jit(Function *f, size_t nargs) {
//	if (f->arg_size() != 0) throw iv_arithmetic_error("JIT: expected f->arg_size == 0");
	Compiled c(nargs);
	// probably ub, but probably will work too.
	c._jitted = Compiled::FuncType(
		reinterpret_cast<Compiled::FuncPtrType>(
			reinterpret_cast<uintptr_t>(jit_to_void(f))));
	return std::move(c);
}



Compiled Compiler::jit_expr(ExprPtr const &e) {
	if (e->as_func_expr()) throw iv_arithmetic_error("Expected expression in call to jit_expr");

	Function *f = compile_expr(e);
	assert(f);
	return jit(f, 0);
}

vector<Value*> Compiler::ivec2vvec(vector<interval> const &v) {
	vector<Value *> vals;
	for (auto const &iv : v) {
		vals.push_back(ConstantFP::get(_module->getContext(), APFloat(iv.lo())));
		vals.push_back(ConstantFP::get(_module->getContext(), APFloat(iv.hi())));
	}
	return vals;
}


//interval Compiler::execute(Function *f, vector<interval> const &args) {
//	jitted_function fn = jit_call(f, args);
//	return from_ret(fn());
//}
//
//
//interval Compiler::execute(string const &s, vector<interval> const &args) {
//	jitted_function f = jit_call(s, args);
//	return from_ret(f());
//}
//
interval Compiler::execute(ExprPtr const &ep) {
	if (ep->as_func_expr()) {
		compile(*ep);
		return interval::empty();
	}

	Function *f = compile_1func(FuncExpr(ep->clone()));
	assert(f);
	assert(f->arg_size() == 0);
	assert(ep.get());
//	POD_JITFunc ff = reinterpret_cast<POD_JITFunc>(
//		reinterpret_cast<uintptr_t>(jit_to_void(f)));
	pod_interval (*ff)(void) = (pod_interval (*)(void))reinterpret_cast<uintptr_t>(jit_to_void(f));
	return from_ret(ff());
}


void Compiler::do_jit(llvm::Function *f) {
	if (_verbose > 0) cout << "Compiler: JITTING TO NATIVE CODE " << endl;
	_exec_engine->runJITOnFunction(f);
	if (_verbose > 0) cout << "Compiler: DONE JITTING TO NATIVE CODE " << endl;
}
/*
Compiled Compiler::jit_call(Function *f, vector<interval> const &args) {
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

Compiled Compiler::jit_call(string const &name, vector<interval> const &args) {
	Function *f = _module->getFunction(name);
	if (!f) throw iv_arithmetic_error("Unknown function: '"+name+"'");
	return jit_call(f, args);
}
*/

VInterval Compiler::compile(Expr &e) {
	_iv.lo = _iv.hi = nullptr;
	e.accept(*this);
	return _iv;
}

Function *Compiler::lookup(string const &name) {
	auto it = _compiled.find(name);
	if (it == _compiled.end()) return nullptr;
	return it->getValue()->compiled;
}

Function *Compiler::lookup_partials(string const &name) {
	auto it = _compiled.find(name);
	if (it == _compiled.end()) return nullptr;
	return it->getValue()->allpartials;
}


//Compiled Compiler::from_fn(Function *f) {
//	return jit(f);
//}


PartialComp::PartialComp() : _ctx(nullptr) {}

PartialComp::PartialComp(Compiler *c) : _ctx(c) {}

PartialComp::PartialComp(Compiler *c, FuncExpr &fe) : _ctx(c), _pnames(fe.params()) { initialize(fe); }

PartialComp &PartialComp::operator=(PartialComp const &pc) {
	_ctx = pc._ctx;
	_fname = pc._fname;
	_pnames = pc._pnames;
	_pf_names = pc._pf_names;
	_compiled = pc._compiled;
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
	for (auto const &i : Derivator::partials(fe)) {
		_pf_names.push_back(deriv_prefix + i.first);
		ExprPtr pfe = Expr::make_func(_pf_names.back(), _pnames, Simplifier::simplified(i.second));
		_fpartials.push_back(move(pfe));
	}
	_fn = _ctx->compile_func_partials(fe);
	_ctx->do_jit(_fn);
	_compiled = _ctx->jit(_fn, fe.params().size());
}



vector<interval> PartialComp::calculate(vector<interval> const &args) {
//	vector<interval> res;
//	for (auto f : _funcs)
//		res.push_back(_ctx->execute(f, args));
	return std::move(_compiled(args));
}




}