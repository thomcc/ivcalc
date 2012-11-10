#include "compiler.hh"
#include "llvm/DerivedTypes.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Intrinsics.h"
namespace calc {

using namespace llvm;
using namespace std;


// llvm intrinsics for mxcsr register  (controls sse rounding mode):
// declare void @llvm.x86.sse.stmxcsr(i32*) nounwind
// declare void @llvm.x86.sse.ldmxcsr(i32*) nounwind
//
// mxcsr 32 bit format:
//
// | Location   | Mnemonic | Description                       |
// | :--------  | :------  | :-------------------------------- |
// | bits 31-16 |          | Unused as of SSE3                 |
// | bit 15     | FZ       | Flush to zero (underflow -> zero) |
// | bit 14     | R+       | Round positive                    |
// | bit 13     | R-       | Round negative                    |
// | bits 13-14 | RZ       | Round to zero (if both set)       |
// | bits 13-14 | RN       | Round to nearest (if neither set) |
// | bit 12     | PM       | Precision mask                    |
// | bit 11     | UM       | Underflow mask                    |
// | bit 10     | OM       | Overflow mask                     |
// | bit 9      | ZM       | Divide by zero mask               |
// | bit 8      | DM       | Denormal mask                     |
// | bit 7      | IM       | Invalid operation mask            |
// | bit 6      | DAZ      | Denormals are zero                |
// | bit 5      | PE       | Precision flag                    |
// | bit 4      | UE       | Underflow flag                    |
// | bit 3      | OE       | Overflow flag                     |
// | bit 2      | ZE       | Divide by zero flag               |
// | bit 1      | DE       | Denormal flag                     |
// | bit 0      | IE       | Invalid operation flag            |
//
// for more info see http://softpixel.com/~cwright/programming/simd/sse.php
//
// llvm intrinsics for setting fpu mode?
//
// on x86 round down is 0x167f and round up is 0x1a7f (with exceptions masked)
// rounding mode can be set w/ __asm__ __volatile__ ("fldcw %0" : : "m"(mode))
// in c/c++,
//
// needs to be llvm intrinsic for it to work with jit (no inline asm)
//
//     unsigned int mxcsr;
//     __asm__ volatile ("stmxcsr %0" : "=m" (mxcsr))
//     mxcsr &= ~0x6000
//     mxcsr |= (mode << 3)
//     __asm__volatile ("ldmxcsr %0" : : "m" (mxcsr));
//
// for mode in FE_(UP|DOWN)WARD
// hrm...
// can this work?

Compiler::Compiler()
: _iv{nullptr, nullptr}
, _context(llvm::getGlobalContext())
, _module{new llvm::Module("interval jit", _context)}
, _builder(_context) {
	init_module();
}

// add the `setfpr` function to the module, which controls the rounding mode.
void Compiler::init_module() {
	// declare the function
	Function *ldmxcsr = Intrinsic::getDeclaration(_module.get(), Intrinsic::x86_sse_ldmxcsr);
	Function *stmxcsr = Intrinsic::getDeclaration(_module.get(), Intrinsic::x86_sse_stmxcsr);
	{
		// add set_rup()
		FunctionType *set_rup_t = FunctionType::get(Type::getVoidTy(_module->getContext()), false);
		Constant *c = _module->getOrInsertFunction("set_rup", set_rup_t);
		Function *r_up = cast<Function>(c);
		IRBuilder<> rupbuilder(&r_up->getEntryBlock(), r_up->getEntryBlock().begin());
		Value *mxcsr = rupbuilder.CreateAlloca(Type::getIntNTy(_context, 32), 0, "the_mxcsr");
		rupbuilder.CreateCall(stmxcsr, mxcsr);
		Value *l = rupbuilder.CreateLoad(mxcsr);
		Value *l2 = rupbuilder.CreateAnd(l, ~0x6000);
		Value *l3 = rupbuilder.CreateOr(l2, FE_UPWARD << 3);
		rupbuilder.CreateStore(l3, mxcsr);
		rupbuilder.CreateCall(ldmxcsr, mxcsr);
		rupbuilder.CreateRetVoid();
	}
	{
		// add set_rdown()
		FunctionType *set_rdown_t = FunctionType::get(Type::getVoidTy(_module->getContext()), false);
		Constant *c = _module->getOrInsertFunction("set_rdown", set_rdown_t);
		Function *r_down = cast<Function>(c);
		IRBuilder<> rdownbuilder(&r_down->getEntryBlock(), r_down->getEntryBlock().begin());
		Value *mxcsr = rdownbuilder.CreateAlloca(Type::getIntNTy(_context, 32), 0, "the_mxcsr");
		rdownbuilder.CreateCall(stmxcsr, mxcsr);
		Value *l = rdownbuilder.CreateLoad(mxcsr);
		Value *l2 = rdownbuilder.CreateAnd(l, ~0x6000);
		Value *l3 = rdownbuilder.CreateOr(l2, FE_DOWNWARD << 3);
		rdownbuilder.CreateStore(l3, mxcsr);
		rdownbuilder.CreateCall(ldmxcsr, mxcsr);
		rdownbuilder.CreateRetVoid();
	}
}


Value *Compiler::cadd_lo(Value *a, Value *b, const char *name) {
	_builder.CreateCall(_module->getFunction("set_rdown"));
	return _builder.CreateFAdd(a, b, name);
}

Value *Compiler::cadd_hi(Value *a, Value *b, const char *name) {
	_builder.CreateCall(_module->getFunction("set_rup"));
	return _builder.CreateFAdd(a, b, name);
}

Value *Compiler::csub_lo(Value *a, Value *b, const char *name) {
	_builder.CreateCall(_module->getFunction("set_rdown"));
	return _builder.CreateFSub(a, b, name);
}

Value *Compiler::csub_hi(Value *a, Value *b, const char *name) {
	_builder.CreateCall(_module->getFunction("set_rup"));
	return _builder.CreateFSub(a, b, name);
}

Value *Compiler::cmul_lo(Value *a, Value *b, const char *name) {
	_builder.CreateCall(_module->getFunction("set_rdown"));
	return _builder.CreateFMul(a, b, name);
}

Value *Compiler::cmul_hi(Value *a, Value *b, const char *name) {
	_builder.CreateCall(_module->getFunction("set_rup"));
	return _builder.CreateFMul(a, b, name);
}

Value *Compiler::cdiv_lo(Value *a, Value *b, const char *name) {
	_builder.CreateCall(_module->getFunction("set_rdown"));
	return _builder.CreateFDiv(a, b, name);
}

Value *Compiler::cdiv_hi(Value *a, Value *b, const char *name) {
	_builder.CreateCall(_module->getFunction("set_rup"));
	return _builder.CreateFDiv(a, b, name);
}



void Compiler::visit(AddExpr &e) {
	VInterval lhs = compile(*e.lhs());
	VInterval rhs = compile(*e.rhs());
	_iv.lo = cadd_lo(lhs.lo, rhs.lo, "add_lo");
	_iv.hi = cadd_hi(lhs.hi, rhs.hi, "add_hi");
}

void Compiler::visit(SubExpr &e) {
	VInterval lhs = compile(*e.lhs());
	VInterval rhs = compile(*e.rhs());
	_iv.lo = csub_lo(lhs.lo, rhs.hi, "sub_lo");
	_iv.hi = csub_hi(lhs.hi, rhs.lo, "sub_hi");
}

void Compiler::visit(NegExpr &e) {
	VInterval val = compile(*e.value());
	_iv.lo = _builder.CreateNeg(val.hi, "neg_lo");
	_iv.hi = _builder.CreateNeg(val.lo, "neg_hi");
}

void Compiler::visit(MulExpr &e) {
	VInterval lhs = compile(*e.lhs());
	VInterval rhs = compile(*e.rhs());
	// could optimize to emit code similar to interval.cc,
	// but it would be quite ugly, and probably wouldn't have
	// much gain (well, the second part is probably not true)
	{
		// generate code to find _iv.hi
		// basically, do all the multiplications with rounding mode up
		// and then find the max
		_builder.CreateCall(_module->getFunction("set_rup"));
		Value *ah = _builder.CreateFMul(lhs.lo, rhs.lo, "mul_hi_a");
		Value *bh = _builder.CreateFMul(lhs.lo, rhs.hi, "mul_hi_b");
		Value *ch = _builder.CreateFMul(lhs.hi, rhs.lo, "mul_hi_c");
		Value *dh = _builder.CreateFMul(lhs.hi, rhs.hi, "mul_hi_d");
		// compute the max
		Value *abh_test = _builder.CreateFCmpOLT(ah, bh, "mul_hi_ab_test");
		Value *cdh_test = _builder.CreateFCmpOLT(ch, dh, "mul_hi_cd_test");
		Value *ab_hi_max = _builder.CreateSelect(abh_test, bh, ah, "mul_hi_ab_max");
		Value *cd_hi_max = _builder.CreateSelect(cdh_test, dh, ch, "mul_hi_cd_max");
		Value *abcdh_test = _builder.CreateFCmpOLT(ab_hi_max, cd_hi_max, "mul_hi_abcd_test");
		_iv.hi = _builder.CreateSelect(abcdh_test, cd_hi_max, ab_hi_max, "mul_hi_res");
	}
	{
		// generate code to find _iv.lo
		// same as above, but with rounding mode down and finding the min
		_builder.CreateCall(_module->getFunction("set_rdown"));
		Value *al = _builder.CreateFMul(lhs.lo, rhs.lo, "mul_lo_a");
		Value *bl = _builder.CreateFMul(lhs.lo, rhs.hi, "mul_lo_b");
		Value *cl = _builder.CreateFMul(lhs.hi, rhs.lo, "mul_lo_c");
		Value *dl = _builder.CreateFMul(lhs.hi, rhs.hi, "mul_lo_d");
		// compute the min
		Value *abl_test = _builder.CreateFCmpOLT(bl, al, "mul_lo_ab_test");
		Value *cdl_test = _builder.CreateFCmpOLT(dl, cl, "mul_lo_cd_test");
		Value *cd_lo_min = _builder.CreateSelect(cdl_test, dl, cl, "mul_lo_cd_min");
		Value *ab_lo_min = _builder.CreateSelect(abl_test, bl, al, "mul_lo_ab_min");
		Value *abcdl_test = _builder.CreateFCmpOLT(cd_lo_min, ab_lo_min, "mul_lo_abcd_test");
		_iv.lo = _builder.CreateSelect(abcdl_test, cd_lo_min, ab_lo_min, "mul_lo_res");
	}
}

void Compiler::visit(DivExpr &e) {
	VInterval lhs = compile(*e.lhs());
	VInterval rhs = compile(*e.rhs());
	// as above
	{
		// generate code to find _iv.hi
		// basically, do all the divisions with rounding mode up
		// and then find the max
		_builder.CreateCall(_module->getFunction("set_rup"));
		Value *ah = _builder.CreateFDiv(lhs.lo, rhs.lo, "div_hi_a");
		Value *bh = _builder.CreateFDiv(lhs.lo, rhs.hi, "div_hi_b");
		Value *ch = _builder.CreateFDiv(lhs.hi, rhs.lo, "div_hi_c");
		Value *dh = _builder.CreateFDiv(lhs.hi, rhs.hi, "div_hi_d");
		// compute the max
		Value *abh_test = _builder.CreateFCmpOLT(ah, bh, "div_hi_ab_test");
		Value *cdh_test = _builder.CreateFCmpOLT(ch, dh, "div_hi_cd_test");
		Value *ab_hi_max = _builder.CreateSelect(abh_test, bh, ah, "div_hi_ab_max");
		Value *cd_hi_max = _builder.CreateSelect(cdh_test, dh, ch, "div_hi_cd_max");
		Value *abcdh_test = _builder.CreateFCmpOLT(ab_hi_max, cd_hi_max, "div_hi_abcd_test");
		_iv.hi = _builder.CreateSelect(abcdh_test, cd_hi_max, ab_hi_max, "div_hi_res");
	}
	{
		// generate code to find _iv.lo
		// same as above, but with rounding mode down and finding the min
		_builder.CreateCall(_module->getFunction("set_rdown"));
		Value *al = _builder.CreateFDiv(lhs.lo, rhs.lo, "div_lo_a");
		Value *bl = _builder.CreateFDiv(lhs.lo, rhs.hi, "div_lo_b");
		Value *cl = _builder.CreateFDiv(lhs.hi, rhs.lo, "div_lo_c");
		Value *dl = _builder.CreateFDiv(lhs.hi, rhs.hi, "div_lo_d");
		// compute the min
		Value *abl_test = _builder.CreateFCmpOLT(bl, al, "div_lo_ab_test");
		Value *cdl_test = _builder.CreateFCmpOLT(dl, cl, "div_lo_cd_test");
		Value *cd_lo_min = _builder.CreateSelect(cdl_test, dl, cl, "div_lo_cd_min");
		Value *ab_lo_min = _builder.CreateSelect(abl_test, bl, al, "div_lo_ab_min");
		Value *abcdl_test = _builder.CreateFCmpOLT(cd_lo_min, ab_lo_min, "div_lo_abcd_test");
		_iv.lo = _builder.CreateSelect(abcdl_test, cd_lo_min, ab_lo_min, "div_lo_res");
	}
}

void Compiler::visit(ExptExpr &e) {
	VInterval base = compile(*e.base());
	int expt = e.power();
	if (expt == 0) {
		// [a,b]^0 = 0 \in [a,b] ? [0,1] : [1,1]
		Value *zero = ConstantFP::get(_context, APFloat(0.0));
		Value *lo_ltz = _builder.CreateFCmpOLE(base.lo, zero, "expt0_lo_lez");
		Value *hi_gtz = _builder.CreateFCmpOGE(base.hi, zero, "expt0_hi_gez");
		Value *has_zero = _builder.CreateAnd(lo_ltz, hi_gtz, "expt0_base_has_zero");
		Value *one = ConstantFP::get(_context, APFloat(1.0));
		_iv.lo = _builder.CreateSelect(has_zero, zero, one);
		_iv.hi = ConstantFP::get(_context, APFloat(1.0));
	} else if (!(expt&1)) {
		_builder.CreateCall(_module->getFunction("set_rdown"));
		bool negative = false;
		if (expt < 0) {
			negative = true;
			expt *= -1;
		}
		Value *lo_expt = base.lo;
		for (int power = 0; power < expt; ++power) {
			std::string s = stringize() << "even_odd_pow_unroll_" << power << "_lo";
			lo_expt = _builder.CreateFMul(lo_expt, base.lo, s);
		}
		_builder.CreateCall(_module->getFunction("set_rup"));
		Value *hi_expt = base.hi;
		for (int power = 0; power < expt; ++power) {
			std::string s = stringize() << "expt_odd_pow_unroll_" << power << "_hi";
			hi_expt = _builder.CreateFMul(hi_expt, base.hi, s);
		}
		if (!negative) {
			_iv.lo = lo_expt;
			_iv.hi = hi_expt;
			return;
		}
		// handle the negative case... Lets pretend this is less ugly than it is.

		// dont't need
		// _builder.CreateCall(_module->getFunction("set_rup"));
		// becausealready in correct rounding mode

		Value *one = ConstantFP::get(_context, APFloat(1.0));
		Value *ah = _builder.CreateFDiv(one, lo_expt, "expt_inv_hi_a");
		Value *bh = _builder.CreateFDiv(one, hi_expt, "expt_inv_hi_b");
		Value *abtest = _builder.CreateFCmpOLT(ah, bh, "expt_inv_hi_abtest");
		_iv.hi = _builder.CreateSelect(abtest, bh, ah);

		_builder.CreateCall(_module->getFunction("set_rdown"));

		Value *ch = _builder.CreateFDiv(one, lo_expt, "expt_inv_lo_a");
		Value *dh = _builder.CreateFDiv(one, hi_expt, "expt_inv_lo_b");
		Value *cdtest = _builder.CreateFCmpOLT(ch, dh, "expt_inv_lo_cdtest");
		_iv.lo = _builder.CreateSelect(cdtest, ch, dh);

	} else {
		Value *zero = ConstantFP::get(_context, APFloat(0.0));
		Value *hi_lz = _builder.CreateFCmpOLT(base.hi, zero, "expt_hi_ltz");
		Function *fn = _builder.GetInsertBlock()->getParent();
		BasicBlock *both_neg = BasicBlock::Create(_context, "expt_hi_is_neg", fn);
		BasicBlock *one_pos = BasicBlock::Create(_context, "expt_hi_is_pos");
		BasicBlock *both_pos = BasicBlock::Create(_context, "expt_both_are_pos");
		BasicBlock *has_zero = BasicBlock::Create(_context, "expt_has_zero");
		BasicBlock *merge_block = BasicBlock::Create(_context, "expt_done");
		_builder.CreateCondBr(hi_lz, both_neg, one_pos);
		bool negative = false;
		if (expt < 0) {
			negative = true;
			expt *= -1;
		}

		_builder.SetInsertPoint(both_neg);
		Value *lo_expt_bneg = nullptr, *hi_expt_bneg = nullptr;
		// emit code for the exponent
		_builder.CreateCall(_module->getFunction("set_rdown"));
		lo_expt_bneg = base.hi;
		for (int power = 0; power < expt; ++power) {
			std::string s = stringize() << "expteven_pow_unroll_" << power << "_lo";
			lo_expt_bneg = _builder.CreateFMul(lo_expt_bneg, base.hi, s);
		}
		_builder.CreateCall(_module->getFunction("set_rup"));
		hi_expt_bneg = base.lo;
		for (int power = 0; power < expt; ++power) {
			std::string s = stringize() << "expt_pow_unroll_" << power << "_hi";
			hi_expt_bneg = _builder.CreateFMul(hi_expt_bneg, base.lo, s);
		}

		_builder.CreateBr(merge_block);
		fn->getBasicBlockList().push_back(one_pos);

		// emit code to test if this interval has zero

		_builder.SetInsertPoint(one_pos);
		Value *lo_gz = _builder.CreateFCmpOGT(base.lo, zero, "expt_lo_gtz");
		_builder.CreateCondBr(lo_gz, both_pos, has_zero);

		fn->getBasicBlockList().push_back(one_pos);

		_builder.SetInsertPoint(has_zero);
		// emit code for the case that the interval contains zero
		Value *lo_expt_hzero = nullptr, *hi_expt_hzero = nullptr;
		// [0, max(a^n, b^n)]
		_builder.CreateCall(_module->getFunction("set_rup"));
		lo_expt_hzero = base.lo;
		hi_expt_hzero = base.hi;
		for (int power = 0; power < expt; ++power) {
			std::string s = stringize() << "expt_even_hz_pow_unroll_" << power;
			lo_expt_hzero = _builder.CreateFMul(lo_expt_hzero, base.lo, s+"_lo");
			hi_expt_hzero = _builder.CreateFMul(hi_expt_hzero, base.hi, s+"_hi");
		}
		Value *cmp = _builder.CreateFCmpOLT(lo_expt_hzero, hi_expt_hzero, "expt_even_hz_lo_or_hi_lt");
		Value *max = _builder.CreateSelect(cmp, hi_expt_hzero, lo_expt_hzero, "expt_even_hz_max_hi_lo");
		lo_expt_hzero = zero;
		hi_expt_hzero = max;
		_builder.CreateBr(merge_block);

		fn->getBasicBlockList().push_back(has_zero);

		_builder.SetInsertPoint(both_pos);

		Value *lo_expt_pos = base.hi, *hi_expt_pos = nullptr;
		for (int power = 0; power < expt; ++power) {
			std::string s = stringize() << "even_pow_hi_unroll_" << power << "_lo";
			lo_expt_pos = _builder.CreateFMul(lo_expt_pos, base.hi, s);
		}
		_builder.CreateCall(_module->getFunction("set_rup"));
		for (int power = 0; power < expt; ++power) {
			std::string s = stringize() << "expt_pow_hi_unroll_" << power << "_hi";
			hi_expt_pos = _builder.CreateFMul(hi_expt_pos, base.lo, s);
		}

		_builder.CreateBr(merge_block);
		fn->getBasicBlockList().push_back(both_pos);

		_builder.SetInsertPoint(merge_block);

		PHINode *phi_lo = _builder.CreatePHI(Type::getDoubleTy(_context), 3, "expt_lo");
		PHINode *phi_hi = _builder.CreatePHI(Type::getDoubleTy(_context), 3, "expt_hi");

		phi_lo->addIncoming(lo_expt_bneg, both_neg);
		phi_hi->addIncoming(hi_expt_bneg, both_neg);

		phi_lo->addIncoming(lo_expt_hzero, has_zero);
		phi_hi->addIncoming(hi_expt_hzero, has_zero);

		phi_lo->addIncoming(lo_expt_pos, both_pos);
		phi_hi->addIncoming(hi_expt_pos, both_pos);

		if (!negative) {
			_iv.lo = phi_lo;
			_iv.hi = phi_hi;
		} else {
			Value *one = ConstantFP::get(_context, APFloat(1.0));
			Value *ah = _builder.CreateFDiv(one, phi_lo, "expt_inv_hi_a");
			Value *bh = _builder.CreateFDiv(one, phi_hi, "expt_inv_hi_b");
			Value *abtest = _builder.CreateFCmpOLT(ah, bh, "expt_inv_hi_abtest");
			_iv.hi = _builder.CreateSelect(abtest, bh, ah);

			_builder.CreateCall(_module->getFunction("set_rdown"));

			Value *ch = _builder.CreateFDiv(one, phi_lo, "expt_inv_lo_a");
			Value *dh = _builder.CreateFDiv(one, phi_hi, "expt_inv_lo_b");
			Value *cdtest = _builder.CreateFCmpOLT(ch, dh, "expt_inv_lo_cdtest");
			_iv.lo = _builder.CreateSelect(cdtest, ch, dh);

		}
	}
}

void Compiler::visit(LitExpr &e) {
	_iv.lo = ConstantFP::get(_context, APFloat(e.value().lo()));
	_iv.hi = ConstantFP::get(_context, APFloat(e.value().hi()));
}

void Compiler::visit(FuncExpr &e) {
// todo
}

void Compiler::visit(VarExpr &e) {
	auto i = _named.find(e.name());
	if (i != _named.end()) {
		_iv.lo = i->second.lo;
		_iv.hi = i->second.hi;
	} else throw iv_arithmetic_error("Unbound variable: '"+e.name()+"'");
}

void Compiler::visit(CallExpr &e) {
// todo
}

VInterval Compiler::compile(Expr &e) {
	_iv.lo = _iv.hi = nullptr;
	e.accept(*this);
	return _iv;
}


}