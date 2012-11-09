#ifndef __DERIVATOR_HH__
#define __DERIVATOR_HH__

#include "visitors/visitorbase.hh"
#include "common.hh"
#include <string>
#include <vector>
#include "expr.hh"
namespace calc {

class Derivator : public ExprVisitor {
	std::string _var;
	ExprPtr _derived;
public:

	Derivator(std::string const &v);


	ExprPtr derive(Expr &e);
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
	static void partials(FuncExpr const &e, std::vector<std::pair<std::string, ExprPtr>> &dest);

	static std::vector<std::pair<std::string, ExprPtr>> partials(FuncExpr const &e) {
		std::vector<std::pair<std::string, ExprPtr>> res;
		partials(e, res);
		return res;
	}



};






}












#endif