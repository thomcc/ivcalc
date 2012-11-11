#ifndef __SIMPLIFIER_HH__
#define __SIMPLIFIER_HH__
//#include <map>
#include "expr.hh"
#include "visitors/visitorbase.hh"
#include "interval.hh"
namespace calc {

class Simplifier : public ExprVisitor {
	ExprPtr _simplified;
public:
	Simplifier() : _simplified(nullptr) {}
	ExprPtr simplify(Expr &e);
	void visit(AddExpr &e);
	void visit(SubExpr &e);
	void visit(NegExpr &e);
	void visit(MulExpr &e);
	void visit(DivExpr &e);
	void visit(VarExpr &e);
	void visit(ExptExpr &e);
	void visit(LitExpr &e);
	void visit(CallExpr &e);
	void visit(FuncExpr &e);
	void visit(EmptyExpr &e);
	static ExprPtr simplified(ExprPtr const &e) { return Simplifier().simplify(*e); }


};






}












#endif