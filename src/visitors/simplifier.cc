#include "visitors/simplifier.hh"

namespace calc {
// todo: remove constant folding?
using namespace std;
ExprPtr Simplifier::simplify(Expr &e) { e.accept(*this); return move(_simplified); }

void Simplifier::visit(AddExpr &e) {
	ExprPtr l = simplify(*e.lhs());
	ExprPtr r = simplify(*e.rhs());
	LitExpr const *ll = l->as_lit_expr();
	LitExpr const *rl = r->as_lit_expr();
	if (ll && rl) _simplified = Expr::make_lit(ll->value()+rl->value());
	else if (ll && ll->value().is_zero()) _simplified = move(r);
	else if (rl && rl->value().is_zero()) _simplified = move(l);
	else _simplified = Expr::make_add(move(l), move(r));
}

void Simplifier::visit(SubExpr &e) {
	ExprPtr l = simplify(*e.lhs());
	ExprPtr r = simplify(*e.rhs());
	LitExpr const *ll = l->as_lit_expr();
	LitExpr const *rl = r->as_lit_expr();
	if (ll && rl) _simplified = Expr::make_lit(ll->value()-rl->value());
	else if (ll && ll->value().is_zero()) _simplified = Expr::make_neg(move(r));
	else if (rl && rl->value().is_zero()) _simplified = move(l);
	else if (NegExpr *rhs = r->as_neg_expr()) _simplified = Expr::make_add(move(l), move(rhs->value()));
	else _simplified = Expr::make_sub(move(l), move(r));
}

void Simplifier::visit(NegExpr &e) {
	ExprPtr v = simplify(*e.value());
	if (LitExpr const *lv = v->as_lit_expr()) _simplified = Expr::make_lit(-lv->value());
	else if (NegExpr *ne = v->as_neg_expr()) _simplified = move(ne->value());
	else _simplified = Expr::make_neg(move(v));
}

void Simplifier::visit(MulExpr &e) {
	ExprPtr l = simplify(*e.lhs());
	ExprPtr r = simplify(*e.rhs());
	LitExpr const *ll = l->as_lit_expr();
	LitExpr const *rl = r->as_lit_expr();
	if (ll && rl) { _simplified = Expr::make_lit(ll->value() * rl->value()); return; }
	else if (ll) {
		if (ll->value().is_zero()) { _simplified = Expr::make_lit(interval::zero()); return; }
		else if (ll->value().is_one()) { _simplified = move(r); return; }
	} else if (rl) {
		if (rl->value().is_zero()) { _simplified = Expr::make_lit(interval::zero()); return; }
		else if (rl->value().is_one()) { _simplified = move(l); return; }
	}
	NegExpr *ln = l->as_neg_expr();
	NegExpr *rn = r->as_neg_expr();
	if (ln && rn) _simplified = Expr::make_mul(move(ln->value()), move(rn->value()));
	else _simplified = Expr::make_mul(move(l), move(r));
}

void Simplifier::visit(DivExpr &e) {
	ExprPtr l = simplify(*e.lhs());
	ExprPtr r = simplify(*e.rhs());
	LitExpr const *ll = l->as_lit_expr();
	LitExpr const *rl = r->as_lit_expr();
	if (ll && rl) _simplified = Expr::make_lit(ll->value() / rl->value());
	else if (ll && ll->value().is_zero()) _simplified = move(l); // 0 / foo = 0
	else if (rl && rl->value().is_one()) _simplified = move(l); // foo / 1 = foo
	else {
		NegExpr *ln = l->as_neg_expr();
		NegExpr *rn = r->as_neg_expr();
		if (ln && rn) _simplified = Expr::make_div(move(ln->value()), move(rn->value()));
		else _simplified = Expr::make_div(move(l), move(r));
	}
}

void Simplifier::visit(VarExpr &e) { _simplified = e.clone(); }
void Simplifier::visit(LitExpr &e) { _simplified = e.clone(); }
void Simplifier::visit(EmptyExpr &e) { _simplified = e.clone(); }

void Simplifier::visit(ExptExpr &e) {
	if (e.power() == 0) _simplified = Expr::make_lit(1);
	else if (e.power() == 1) _simplified = simplify(*e.base());
	else {
		ExprPtr eb = simplify(*e.base());
		LitExpr const *l = eb->as_lit_expr();
		if (l && (l->value().is_one() || l->value().is_zero())) _simplified = move(eb);
		else if (l) _simplified = Expr::make_lit(int_pow(l->value(), e.power()));
		else if (e.power() < 0) _simplified = Expr::make_div(Expr::make_lit(interval::one()), Expr::make_expt(move(eb), e.power()));
		else _simplified = Expr::make_expt(move(eb), e.power());
	}
}

void Simplifier::visit(CallExpr &e) {
	vector<ExprPtr> nargs;
	for (auto const &oa : e.args()) nargs.push_back(simplify(*oa));
	_simplified = Expr::make_call(e.name(), move(nargs));
}

void Simplifier::visit(FuncExpr &e) {
	_simplified = Expr::make_func(e.name(), e.params(), simplify(*e.impl()));
}




}