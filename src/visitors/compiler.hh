#ifndef __COMPILER_HH__
#define __COMPILER_HH__
#include "common.hh"
#include "expr.hh"
#include "llvm/Support/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include <map>
namespace calc {

struct VInterval {
	llvm::Value *lo, *hi;
};

class Compiler : public ExprVisitor {
	VInterval _iv;
	llvm::LLVMContext &_context;
	std::unique_ptr<llvm::Module> _module;
	llvm::IRBuilder<> _builder;
	std::map<std::string, VInterval> _named;
public:
	Compiler();
	void init_module();
	void visit(EmptyExpr&) {}
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
	VInterval compile(Expr&);
	llvm::Value *cadd_lo(llvm::Value *a, llvm::Value *b, const char *name="");
	llvm::Value *cadd_hi(llvm::Value *a, llvm::Value *b, const char *name="");
	llvm::Value *csub_lo(llvm::Value *a, llvm::Value *b, const char *name="");
	llvm::Value *csub_hi(llvm::Value *a, llvm::Value *b, const char *name="");
	llvm::Value *cmul_lo(llvm::Value *a, llvm::Value *b, const char *name="");
	llvm::Value *cmul_hi(llvm::Value *a, llvm::Value *b, const char *name="");
	llvm::Value *cdiv_lo(llvm::Value *a, llvm::Value *b, const char *name="");
	llvm::Value *cdiv_hi(llvm::Value *a, llvm::Value *b, const char *name="");
};











}
#endif