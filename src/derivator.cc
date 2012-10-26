#include "derivator.hh"

namespace calc {
Derivator::Derivator(std::string const &v)
: _var(v), _derived(new LitExpr(0)) {}

ExprSPtr
Derivator::derive(ExprSPtr e) {
	_derived = NULL;
	e->accept(*this);
	return _derived;
}

void
Derivator::visit(AddExpr &e) {
	ExprSPtr left = derive(e.lhs());
	ExprSPtr right = derive(e.rhs());
	_derived = ExprSPtr(new AddExpr(left, right));
}

void
Derivator::visit(SubExpr &e) {
	ExprSPtr left = derive(e.lhs());
	ExprSPtr right = derive(e.rhs());
	_derived = ExprSPtr(new SubExpr(left, right));
}

void
Derivator::visit(NegExpr &e) {
	ExprSPtr v = derive(e.value());
	_derived = ExprSPtr(new NegExpr(v));
}

void
Derivator::visit(MulExpr &e) {
	ExprSPtr left = derive(e.lhs());
	ExprSPtr right = derive(e.rhs());
	ExprSPtr rr(new MulExpr(right, e.lhs()));
	ExprSPtr ll(new MulExpr(e.rhs(), left));
	_derived = ExprSPtr(new AddExpr(rr, ll));
}

void
Derivator::visit(DivExpr &e) {
	ExprSPtr dl = derive(e.lhs());
	ExprSPtr dr = derive(e.rhs());
	ExprSPtr denom(new ExptExpr(2, e.rhs()));
	ExprSPtr nl(new MulExpr(e.rhs(), dl));
	ExprSPtr nr(new MulExpr(dr, e.lhs()));
	ExprSPtr numer(new SubExpr(nl, nr));
	_derived = ExprSPtr(new DivExpr(numer, denom));
}

void
Derivator::visit(VarExpr &e) {
	if (e.name() == _var)
		_derived = ExprSPtr(new LitExpr(1));
	else
		_derived = ExprSPtr(new LitExpr(0));
}

void
Derivator::visit(ExptExpr &e) {
	ExprSPtr left = derive(e.base());
	ExprSPtr pc(new LitExpr(e.power()));
	ExprSPtr npow(new ExptExpr(e.power()-1, e.base()));
	ExprSPtr mul(new MulExpr(left, npow));
	_derived = ExprSPtr(new MulExpr(pc, mul));
}

void
Derivator::visit(LitExpr &e) {
	_derived = ExprSPtr(new LitExpr(0));
}


}