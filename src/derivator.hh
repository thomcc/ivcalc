#ifndef __DERIVATOR_HH__
#define __DERIVATOR_HH__
#include "expr.hh"
#include "visitorbase.hh"
#include "interval.hh"
#include <map>
namespace calc {

class Derivator
: public BaseVisitor
, public Visitor<AddExpr>
, public Visitor<SubExpr>
, public Visitor<NegExpr>
, public Visitor<MulExpr>
, public Visitor<DivExpr>
, public Visitor<VarExpr>
, public Visitor<ExptExpr>
, public Visitor<LitExpr>
//, public Visitor<CallExpr>// hrm.
//, public Visitor<EmptyExpr>
//, public Visitor<AssignExpr>
{

	std::string _var;
	ExprSPtr _derived;
public:

	Derivator(std::string const &v);


	ExprSPtr derive(Expr &e);
	void visit(AddExpr &e);
	void visit(SubExpr &e);
	void visit(NegExpr &e);
	void visit(MulExpr &e);
	void visit(DivExpr &e);
	void visit(VarExpr &e);
	void visit(ExptExpr &e);
	void visit(LitExpr &e);
//	void visit(AssignExpr &e);
//	void visit(CallExpr &e);
//	void visit(EmptyExpr &e);


};






}












#endif