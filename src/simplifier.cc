#include "simplifier.hh"

namespace calc {

ExprSPtr
Simplifier::simplify(Expr &e) {
	e.accept(*this);
	return _simplified;
}

void
Simplifier::visit(AddExpr &e) {
	ExprSPtr l = simplify(*e.lhs());
	ExprSPtr r = simplify(*e.rhs());
	LitExpr const *ll = l->as_lit_expr();
	LitExpr const *rl = r->as_lit_expr();
	if (ll && rl) _simplified = Expr::make<LitExpr>(ll->value()+rl->value());
	else if (ll && ll->value().is_zero()) _simplified = r;
	else if (rl && rl->value().is_zero()) _simplified = l;
	else _simplified = Expr::make<AddExpr>(l, r);
}

void
Simplifier::visit(SubExpr &e) {
	ExprSPtr l = simplify(*e.lhs());
	ExprSPtr r = simplify(*e.rhs());
	LitExpr const *ll = l->as_lit_expr();
	LitExpr const *rl = r->as_lit_expr();
	if (ll && rl) _simplified = Expr::make<LitExpr>(ll->value()-rl->value());
	else if (ll && ll->value().is_zero()) _simplified = Expr::make<NegExpr>(r);
	else if (rl && rl->value().is_zero()) _simplified = l;
	else _simplified = Expr::make<SubExpr>(l, r);
}

void
Simplifier::visit(NegExpr &e) {
	ExprSPtr v = simplify(*e.value());
	if (LitExpr const *lv = v->as_lit_expr())
		_simplified = Expr::make<LitExpr>(-lv->value());
	else
		_simplified = Expr::make<NegExpr>(v);
}

void
Simplifier::visit(MulExpr &e) {
	ExprSPtr l = simplify(*e.lhs());
	ExprSPtr r = simplify(*e.rhs());
	LitExpr const *ll = l->as_lit_expr();
	LitExpr const *rl = r->as_lit_expr();
	if (ll && rl) {
		_simplified = Expr::make<LitExpr>(ll->value() * rl->value());
		return;
	} else if (ll) {
		if (ll->value().is_zero()) { _simplified = l; return; }
		if (ll->value().is_one())  { _simplified = r; return; }
	} else if (rl) {
		if (rl->value().is_zero()) { _simplified = r; return; }
		if (rl->value().is_one())  { _simplified = l; return; }
	}
	_simplified = Expr::make<MulExpr>(l, r);
}

void
Simplifier::visit(DivExpr &e) {
	ExprSPtr l = simplify(*e.lhs());
	ExprSPtr r = simplify(*e.rhs());
	LitExpr const *ll = l->as_lit_expr();
	LitExpr const *rl = r->as_lit_expr();
	if (ll && rl) {
		_simplified = Expr::make<LitExpr>(ll->value() / rl->value());
	} else if (ll && ll->value().is_zero()) {
		_simplified = l;
	} else if (rl && rl->value().is_one()) {
		_simplified = l;
	} else _simplified = Expr::make<DivExpr>(l, r);
}

void
Simplifier::visit(VarExpr &e) {
	_simplified = Expr::make<VarExpr>(e.name());
}

void
Simplifier::visit(ExptExpr &e) {
	if (e.power() == 0) _simplified = Expr::make<LitExpr>(1);
	else if (e.power() == 1) _simplified = simplify(*e.base());
	else {
		ExprSPtr eb = simplify(*e.base());
		LitExpr const *l = eb->as_lit_expr();
		if (l && (l->value().is_one() || l->value().is_zero())) _simplified = eb;
		else _simplified = Expr::make<ExptExpr>(eb, e.power());
	}
}

void
Simplifier::visit(LitExpr &e) {
	_simplified = Expr::make<LitExpr>(e.value());
}

void
Simplifier::visit(AssignExpr &e) {
	ExprSPtr rhs = simplify(*e.value());
	_simplified = Expr::make<AssignExpr>(e.name(), rhs);
}

void
Simplifier::visit(CallExpr &e) {
	std::vector<ExprSPtr> nargs;
	for (ExprSPtr oa : e.args())
		nargs.push_back(simplify(*oa));
	_simplified = Expr::make<CallExpr>(e.name(), nargs);
}

void
Simplifier::visit(EmptyExpr &e) {
	_simplified = Expr::make<EmptyExpr>();
}



}