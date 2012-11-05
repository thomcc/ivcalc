#ifndef __DERIVATOR_HH__
#define __DERIVATOR_HH__
//#include "expr.hh"
#include "visitorbase.hh"
#include "common.hh"
#include <string>
#include <vector>

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
, public Visitor<FuncExpr>
, public Visitor<CallExpr>
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
	void visit(FuncExpr &e);
	void visit(CallExpr &e);
	static void partials(FuncExpr const &e, std::vector<std::pair<std::string, ExprSPtr>> &dest);

	static std::vector<std::pair<std::string, ExprSPtr>> partials(FuncExpr const &e) {
		std::vector<std::pair<std::string, ExprSPtr>> res;
		partials(e, res);
		return res;
	}



};






}












#endif