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
	if (ll && rl)
		_simplified = ExprSPtr(new LitExpr(ll->value()+rl->value()));
	else if (ll && ll->value().is_zero())
		_simplified = r;
	else if (rl && rl->value().is_zero())
		_simplified = l;
	else
		_simplified = ExprSPtr(new AddExpr(l, r));
}

void
Simplifier::visit(SubExpr &e) {
	ExprSPtr l = simplify(*e.lhs());
	ExprSPtr r = simplify(*e.rhs());
	LitExpr const *ll = l->as_lit_expr();
	LitExpr const *rl = r->as_lit_expr();
	if (ll && rl)
		_simplified = ExprSPtr(new LitExpr(ll->value()-rl->value()));
	else if (ll && ll->value().is_zero())
		_simplified = ExprSPtr(new NegExpr(r));
	else if (rl && rl->value().is_zero())
		_simplified = l;
	else
		_simplified = ExprSPtr(new SubExpr(l, r));
}

void
Simplifier::visit(NegExpr &e) {
	ExprSPtr v = simplify(*e.value());
	if (LitExpr const *lv = v->as_lit_expr())
		_simplified = ExprSPtr(new LitExpr(-lv->value()));
	else
		_simplified = ExprSPtr(new NegExpr(v));
}

void
Simplifier::visit(MulExpr &e) {
	ExprSPtr l = simplify(*e.lhs());
	ExprSPtr r = simplify(*e.rhs());
	LitExpr const *ll = l->as_lit_expr();
	LitExpr const *rl = r->as_lit_expr();
	if (ll && rl) {
		_simplified = ExprSPtr(new LitExpr(ll->value() * rl->value()));
		return;
	} else if (ll) {
		if (ll->value().is_zero()) { _simplified = l; return; }
		if (ll->value().is_one())  { _simplified = r; return; }
	} else if (rl) {
		if (rl->value().is_zero()) { _simplified = r; return; }
		if (rl->value().is_one())  { _simplified = l; return; }
	}
	_simplified = ExprSPtr(new MulExpr(l, r));
}

void
Simplifier::visit(DivExpr &e) {
	ExprSPtr l = simplify(*e.lhs());
	ExprSPtr r = simplify(*e.rhs());
	LitExpr const *ll = l->as_lit_expr();
	LitExpr const *rl = r->as_lit_expr();
	if (ll && rl) {
		_simplified = ExprSPtr(new LitExpr(ll->value() / rl->value()));
	} else if (ll && ll->value().is_zero()) { 
		_simplified = l;
// even worthwile to check?
//
//		if (ll->value().is_one()) {
//			if (ExptExpr *ed = r->as_expt_expr())
//				_simplified = ExprSPtr(new PowExpr(-ed->power(), ed->base()));
//		}
	}
//	lets not worry about division by zero here.
	else if (rl && rl->value().is_one()) {
		_simplified = l;
	} else 
		_simplified = ExprSPtr(new DivExpr(l, r));
}

void
Simplifier::visit(VarExpr &e) {
	_simplified = ExprSPtr(new VarExpr(e.name()));
}

void
Simplifier::visit(ExptExpr &e) {
	if (e.power() == 0) _simplified = ExprSPtr(new LitExpr(1));
	else if (e.power() == 1) _simplified = simplify(*e.base());
	else {
		ExprSPtr eb = simplify(*e.base());
		LitExpr const *l = eb->as_lit_expr();
		if (l && (l->value().is_one() || l->value().is_zero())) _simplified = eb;
		else _simplified = ExprSPtr(new ExptExpr(e.power(), eb));
	}
}

void
Simplifier::visit(LitExpr &e) {
	_simplified = ExprSPtr(new LitExpr(e.value()));
}

void
Simplifier::visit(AssignExpr &e) {
	ExprSPtr rhs = simplify(*e.value());
	_simplified = ExprSPtr(new AssignExpr(e.name(), rhs));
}

void
Simplifier::visit(CallExpr &e) {
	std::vector<ExprSPtr> nargs;
	for (ExprSPtr oa : e.args())
		nargs.push_back(simplify(*oa));
	_simplified = ExprSPtr(new CallExpr(e.name(), nargs));
}

void
Simplifier::visit(EmptyExpr &e) {
	_simplified = ExprSPtr(new EmptyExpr);
}



}