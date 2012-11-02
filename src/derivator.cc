#include "derivator.hh"

namespace calc {
Derivator::Derivator(std::string const &v)
: _var(v), _derived(Expr::make<LitExpr>(0)) {}

ExprSPtr
Derivator::derive(Expr &e) {
	_derived = NULL;
	e.accept(*this);
	return _derived;
}

void
Derivator::visit(AddExpr &e) {
	ExprSPtr left = derive(*e.lhs());
	ExprSPtr right = derive(*e.rhs());
	_derived = Expr::make<AddExpr>(left, right);
}

void
Derivator::visit(SubExpr &e) {
	ExprSPtr left = derive(*e.lhs());
	ExprSPtr right = derive(*e.rhs());
	_derived = Expr::make<SubExpr>(left, right);
}

void
Derivator::visit(NegExpr &e) {
	ExprSPtr v = derive(*e.value());
	_derived = Expr::make<NegExpr>(v);
}

void
Derivator::visit(MulExpr &e) {
	ExprSPtr left = derive(*e.lhs());
	ExprSPtr right = derive(*e.rhs());
	ExprSPtr rr = Expr::make<MulExpr>(right, e.lhs());
	ExprSPtr ll = Expr::make<MulExpr>(e.rhs(), left);
	_derived = Expr::make<AddExpr>(rr, ll);
}

void
Derivator::visit(DivExpr &e) {
	ExprSPtr dl = derive(*e.lhs());
	ExprSPtr dr = derive(*e.rhs());
	ExprSPtr denom = Expr::make<ExptExpr>(e.rhs(), 2);
	ExprSPtr nl = Expr::make<MulExpr>(e.rhs(), dl);
	ExprSPtr nr = Expr::make<MulExpr>(dr, e.lhs());
	ExprSPtr numer = Expr::make<SubExpr>(nl, nr);
	_derived = Expr::make<DivExpr>(numer, denom);
}

void
Derivator::visit(VarExpr &e) {
	if (e.name() == _var)
		_derived = Expr::make<LitExpr>(1);
	else
		_derived = Expr::make<LitExpr>(0);
}

void
Derivator::visit(ExptExpr &e) {
	ExprSPtr left = derive(*e.base());
	ExprSPtr pc = Expr::make<LitExpr>(e.power());
	ExprSPtr npow = Expr::make<ExptExpr>(e.base(), e.power()-1);
	ExprSPtr mul = Expr::make<MulExpr>(left, npow);
	_derived = Expr::make<MulExpr>(pc, mul);
}

void
Derivator::visit(LitExpr &e) {
	_derived = Expr::make<LitExpr>(0);
}


}