#ifndef __VAR_ADDRESSER_HH__
#define __VAR_ADDRESSER_HH__
#include "common.hh"
#include "interval.hh"
#include "expr.hh"
#include <vector>
#include <string>
namespace calc {
// fake lexical addressing (depth of 1)
class VarAddresser : public ExprVisitor {
	std::vector<std::string> const &_prams;
	ExprPtr _addressed;
public:
	VarAddresser(std::vector<std::string> const &prams) : _prams(prams), _addressed(nullptr) {}

	ExprPtr address(Expr &e) {
		_addressed = nullptr;
		e.accept(*this);
		return move(_addressed);
	}

	void visit(AddExpr &e) {
		ExprPtr rhs = address(*e.rhs());
		ExprPtr lhs = address(*e.lhs());
		_addressed = Expr::make_add(move(lhs), move(rhs));
	}

	void visit(SubExpr &e) {
		ExprPtr rhs = address(*e.rhs());
		ExprPtr lhs = address(*e.lhs());
		_addressed = Expr::make_sub(move(lhs), move(rhs));
	}

	void visit(NegExpr &e) {
		ExprPtr v = address(*e.value());
		_addressed = Expr::make_neg(move(v));
	}

	void visit(MulExpr &e) {
		ExprPtr rhs = address(*e.rhs());
		ExprPtr lhs = address(*e.lhs());
		_addressed = Expr::make_mul(move(lhs), move(rhs));
	}

	void visit(DivExpr &e) {
		ExprPtr rhs = address(*e.rhs());
		ExprPtr lhs = address(*e.lhs());
		_addressed = Expr::make_div(move(lhs), move(rhs));
	}

	void visit(VarExpr &e) {
		for (size_t i = 0; i < _prams.size(); ++i) {
			if (_prams.at(i) == e.name()) {
				 _addressed = Expr::make_var(e.name(), i);
				 return;
			}
		}
	}

	void visit(ExptExpr &e) {
		_addressed = Expr::make_expt(address(*e.base()), e.power());
	}

	void visit(LitExpr &e) { _addressed = e.clone(); }

	void visit(CallExpr &e) {
		std::vector<ExprPtr> args;
		for (auto const &e : e.args()) args.push_back(address(*e));
		_addressed = Expr::make_call(e.name(), move(args));
	}
	void visit(EmptyExpr &e) { _addressed = e.clone(); }
	void visit(FuncExpr &e) {
		throw iv_arithmetic_error("nested functions not allowed");
	}
};
}
#endif