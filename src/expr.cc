#include "expr.hh"
namespace calc {

MemPool AddExpr::_pool(sizeof(AddExpr), "add");
MemPool SubExpr::_pool(sizeof(SubExpr), "sub");
MemPool NegExpr::_pool(sizeof(NegExpr), "neg");
MemPool MulExpr::_pool(sizeof(MulExpr), "mul");
MemPool DivExpr::_pool(sizeof(DivExpr), "div");
MemPool VarExpr::_pool(sizeof(VarExpr), "var");
MemPool ExptExpr::_pool(sizeof(ExptExpr), "expt");
MemPool LitExpr::_pool(sizeof(LitExpr), "lit");
MemPool CallExpr::_pool(sizeof(CallExpr), "call");
MemPool EmptyExpr::_pool(sizeof(EmptyExpr), "empty");
MemPool FuncExpr::_pool(sizeof(FuncExpr), "func");
void Expr::stats() {
	AddExpr::stats();
	SubExpr::stats();
	NegExpr::stats();
	MulExpr::stats();
	DivExpr::stats();
	VarExpr::stats();
	ExptExpr::stats();
	LitExpr::stats();
	CallExpr::stats();
	EmptyExpr::stats();
	FuncExpr::stats();
}

}