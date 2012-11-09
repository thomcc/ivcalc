#include "expr.hh"
namespace calc {

MemPool AddExpr::_pool(sizeof(AddExpr), "add");
void *AddExpr::operator new(size_t size) { return _pool.allocate(size); }
void AddExpr::operator delete(void *p) { _pool.deallocate(p); }


MemPool SubExpr::_pool(sizeof(SubExpr), "sub");
void *SubExpr::operator new(size_t size) { return _pool.allocate(size); }
void SubExpr::operator delete(void *p) { _pool.deallocate(p); }

MemPool NegExpr::_pool(sizeof(NegExpr), "neg");
void *NegExpr::operator new(size_t size) { return _pool.allocate(size); }
void NegExpr::operator delete(void *p) { _pool.deallocate(p); }

MemPool MulExpr::_pool(sizeof(MulExpr), "mul");
void *MulExpr::operator new(size_t size) { return _pool.allocate(size); }
void MulExpr::operator delete(void *p) { _pool.deallocate(p); }

MemPool DivExpr::_pool(sizeof(DivExpr), "div");
void *DivExpr::operator new(size_t size) { return _pool.allocate(size); }
void DivExpr::operator delete(void *p) { _pool.deallocate(p); }

MemPool VarExpr::_pool(sizeof(VarExpr), "var");
void *VarExpr::operator new(size_t size) { return _pool.allocate(size); }
void VarExpr::operator delete(void *p) { _pool.deallocate(p); }

MemPool ExptExpr::_pool(sizeof(ExptExpr), "expt");
void *ExptExpr::operator new(size_t size) { return _pool.allocate(size); }
void ExptExpr::operator delete(void *p) { _pool.deallocate(p); }

MemPool LitExpr::_pool(sizeof(LitExpr), "lit");
void *LitExpr::operator new(size_t size) { return _pool.allocate(size); }
void LitExpr::operator delete(void *p) { _pool.deallocate(p); }

MemPool CallExpr::_pool(sizeof(CallExpr), "call");
void *CallExpr::operator new(size_t size) { return _pool.allocate(size); }
void CallExpr::operator delete(void *p) { _pool.deallocate(p); }

MemPool EmptyExpr::_pool(sizeof(EmptyExpr), "empty");
void *EmptyExpr::operator new(size_t size) { return _pool.allocate(size); }
void EmptyExpr::operator delete(void *p) { _pool.deallocate(p); }

MemPool FuncExpr::_pool(sizeof(FuncExpr), "func");
void *FuncExpr::operator new(size_t size) { return _pool.allocate(size); }
void FuncExpr::operator delete(void *p) { _pool.deallocate(p); }


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