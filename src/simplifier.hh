#ifndef __SIMPLIFIER_HH__
#define __SIMPLIFIER_HH__
#include "expr.hh"
#include "visitorbase.hh"
#include "interval.hh"
#include <map>
namespace calc {

class Simplifier
: public BaseVisitor
, public Visitor<AddExpr>
, public Visitor<SubExpr>
, public Visitor<NegExpr>
, public Visitor<MulExpr>
, public Visitor<DivExpr>
, public Visitor<VarExpr>
, public Visitor<ExptExpr>
, public Visitor<LitExpr>
, public Visitor<CallExpr>
, public Visitor<EmptyExpr>
, public Visitor<AssignExpr>
, public Visitor<FuncExpr> {
	ExprSPtr _simplified;
public:

	Simplifier() {}
	ExprSPtr simplify(Expr &e);
	void visit(AddExpr &e);
	void visit(SubExpr &e);
	void visit(NegExpr &e);
	void visit(MulExpr &e);
	void visit(DivExpr &e);
	void visit(VarExpr &e);
	void visit(ExptExpr &e);
	void visit(LitExpr &e);
	void visit(AssignExpr &e);
	void visit(CallExpr &e);
	void visit(FuncExpr &e);
	void visit(EmptyExpr &e);

	static ExprSPtr simplified(ExprSPtr const &e) { return Simplifier().simplify(*e); }

};






}












#endif