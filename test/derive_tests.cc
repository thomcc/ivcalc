#include "derive_tests.hh"
#include "parser/parser.hh"
#include "visitors/derivator.hh"
#include "visitors/simplifier.hh"
#include "visitors/printer.hh"
namespace calc {

ExprSPtr DeriveTest::get_expr(std::string const &str) const {
	ErrorHandler eh(true, false);
	Parser p(str, eh);
	ExprSPtr eptr = p.parse_expression();
	Check(eptr.get());
	Check(eh.errors() == 0);
	return eptr;
}


void DeriveTest::var() {
	ExprSPtr x = get_expr("x + 20*y");
	Derivator xd("x");
	ExprSPtr dx = xd.derive(*x);
	Simplifier s;
	ExprSPtr sdx = s.simplify(*dx);
	Check(sdx.get());
	if (Check(sdx->as_lit_expr())) CheckEq(interval(1), sdx->as_lit_expr()->value());
	Derivator yd("y");
	ExprSPtr dy = yd.derive(*x);
	ExprSPtr sdy = s.simplify(*dy);
	Check(sdy.get());
	if (Check(sdy->as_lit_expr()))
		CheckEq(interval(20), sdy->as_lit_expr()->value());
}

void DeriveTest::expt() {
	ExprSPtr x2 = get_expr("x^2");
	Derivator xd("x");
	ExprSPtr dx = xd.derive(*x2);
	Simplifier s;
	ExprSPtr sdx = s.simplify(*dx);
	Check(sdx.get());
	if (Check(sdx->as_mul_expr())) {
		MulExpr const *m = sdx->as_mul_expr();
		LitExpr const *lhs = m->lhs()->as_lit_expr();
		VarExpr const *rhs = m->rhs()->as_var_expr();
		if (!(lhs || rhs)) {
			lhs = m->rhs()->as_lit_expr();
			rhs = m->lhs()->as_var_expr();
		}
		if (Check(lhs))
			CheckEq(interval(2), lhs->value());
		if (Check(rhs))
			CheckEq("x", rhs->name());
	}

	ExprSPtr ee = get_expr("40^2");
	ExprSPtr eedx = xd.derive(*ee);
	ExprSPtr seedx = s.simplify(*eedx);
	Check(seedx->is_lit_zero());
}

void DeriveTest::lit() {
	ExprSPtr n = get_expr("110");
	Derivator xd("x");
	Simplifier s;
	ExprSPtr ndx = xd.derive(*n);
	ExprSPtr sndx = s.simplify(*ndx);
	Check(sndx->is_lit_zero());
}

void DeriveTest::plus() {
	ExprSPtr x = get_expr("x + x^2");
	Derivator xd("x");
	ExprSPtr dx = xd.derive(*x);
	Simplifier s;
	ExprSPtr sdx = s.simplify(*dx);
	Check(sdx.get());
	if (Check(sdx->as_add_expr())) {
		AddExpr const *ae = sdx->as_add_expr();
		LitExpr const *lhs = ae->lhs()->as_lit_expr();
		Check(lhs);
		Check(lhs && lhs->is_lit_one());
		MulExpr const *rhs = ae->rhs()->as_mul_expr();
		if (!Check(rhs)) return;
		LitExpr const *rlhs = rhs->lhs()->as_lit_expr();
		VarExpr const *rrhs = rhs->rhs()->as_var_expr();
		if (Check(rlhs)) CheckEq(interval(2), rlhs->value());
		if (Check(rrhs)) CheckEq("x", rrhs->name());
	}
}

void DeriveTest::minus() {
	ExprSPtr x = get_expr("x - x^2");
	Derivator xd("x");
	ExprSPtr dx = xd.derive(*x);
	Simplifier s;
	ExprSPtr sdx = s.simplify(*dx);
	Check(sdx.get());
	if (Check(sdx->as_sub_expr())) {
		SubExpr const *se = sdx->as_sub_expr();
		LitExpr const *lhs = se->lhs()->as_lit_expr();
		Check(lhs);
		Check(lhs && lhs->is_lit_one());
		MulExpr const *rhs = se->rhs()->as_mul_expr();
		if (!Check(rhs)) return;
		LitExpr const *rlhs = rhs->lhs()->as_lit_expr();
		VarExpr const *rrhs = rhs->rhs()->as_var_expr();
		if (Check(rlhs)) CheckEq(interval(2), rlhs->value());
		if (Check(rrhs)) CheckEq("x", rrhs->name());
	}
}

void DeriveTest::mul() {
	ExprSPtr x = get_expr("(3*x + 2) * (2*x + 3)");
	// d/dx should = 2 * (3*x + 2) + 3 * (2 * x + 3)
	// this is going to be a pain.
	Derivator d("x");
	ExprSPtr dx = d.derive(*x);
	Simplifier s;
	ExprSPtr e = s.simplify(*dx);
	if (!CheckNull(e.get())) return;
	if (AddExpr const *ae = CheckNull(e->as_add_expr())) {
		if (MulExpr const *lhs = CheckNull(ae->lhs()->as_mul_expr())) {
			// 2 * (3 * x + 2)
			if (LitExpr const *ll = CheckNull(lhs->lhs()->as_lit_expr()))
				CheckEq(interval(2), ll->value());

			if (AddExpr const *lr = CheckNull(lhs->rhs()->as_add_expr())) {

				if (LitExpr const *lrr = CheckNull(lr->rhs()->as_lit_expr()))
					CheckEq(interval(2), lrr->value());

				if (MulExpr const *lrl = CheckNull(lr->lhs()->as_mul_expr())) {

					if (LitExpr const *lrll = CheckNull(lrl->lhs()->as_lit_expr()))
						CheckEq(interval(3), lrll->value());

					if (VarExpr const *lrlr = CheckNull(lrl->rhs()->as_var_expr()))
						CheckEq("x", lrlr->name());
				}
			}
		}

		if (MulExpr const *rhs = CheckNull(ae->rhs()->as_mul_expr())) {
			// 3 * (2 * x + 3)
			if (LitExpr const *rr = CheckNull(rhs->rhs()->as_lit_expr()))
				CheckEq(interval(3), rr->value());

			if (AddExpr const *rl = CheckNull(rhs->lhs()->as_add_expr())) {

				if (LitExpr const *rlr = CheckNull(rl->rhs()->as_lit_expr()))
					CheckEq(interval(3), rlr->value());

				if (MulExpr const *rll = CheckNull(rl->lhs()->as_mul_expr())) {

					if (LitExpr const *rlll = CheckNull(rll->lhs()->as_lit_expr()))
						CheckEq(interval(2), rlll->value());

					if (VarExpr const *rllr = CheckNull(rll->rhs()->as_var_expr()))
						CheckEq("x", rllr->name());
				}
			}
		}
	}
}

void DeriveTest::div() {
	ExprSPtr x = get_expr("(3*x + 2) / (2*x + 3)");
	// d/dx should = ((2*x + 3) * 3 - (3 * x + 2) * 2) / (2*x+3)^2
	// this is going to be a pain.
	Derivator d("x");
	ExprSPtr dx = d.derive(*x);
	Simplifier s;
	ExprSPtr e = s.simplify(*dx);
	if (!CheckNull(e.get())) return;
	if (DivExpr const *de = CheckNull(e->as_div_expr())) {
		if (SubExpr const *ae = CheckNull(de->lhs()->as_sub_expr())) {

			if (MulExpr const *rhs = CheckNull(ae->rhs()->as_mul_expr())) {
				// 2 * (3 * x + 2)
				if (LitExpr const *ll = CheckNull(rhs->lhs()->as_lit_expr()))
					CheckEq(interval(2), ll->value());

				if (AddExpr const *lr = CheckNull(rhs->rhs()->as_add_expr())) {

					if (LitExpr const *lrr = CheckNull(lr->rhs()->as_lit_expr()))
						CheckEq(interval(2), lrr->value());

					if (MulExpr const *lrl = CheckNull(lr->lhs()->as_mul_expr())) {

						if (LitExpr const *lrll = CheckNull(lrl->lhs()->as_lit_expr()))
							CheckEq(interval(3), lrll->value());

						if (VarExpr const *lrlr = CheckNull(lrl->rhs()->as_var_expr()))
							CheckEq("x", lrlr->name());
					}
				}
			}

			if (MulExpr const *lhs = CheckNull(ae->lhs()->as_mul_expr())) {
				// 3 * (2 * x + 3)
				if (LitExpr const *rr = CheckNull(lhs->rhs()->as_lit_expr()))
					CheckEq(interval(3), rr->value());

				if (AddExpr const *rl = CheckNull(lhs->lhs()->as_add_expr())) {

					if (LitExpr const *rlr = CheckNull(rl->rhs()->as_lit_expr()))
						CheckEq(interval(3), rlr->value());

					if (MulExpr const *rll = CheckNull(rl->lhs()->as_mul_expr())) {

						if (LitExpr const *rlll = CheckNull(rll->lhs()->as_lit_expr()))
							CheckEq(interval(2), rlll->value());

						if (VarExpr const *rllr = CheckNull(rll->rhs()->as_var_expr()))
							CheckEq("x", rllr->name());
					}
				}
			}
		}
		if (ExptExpr const *pe = CheckNull(de->rhs()->as_expt_expr())) {
			CheckEq(2, pe->power());
			if (AddExpr const *rl = CheckNull(pe->base()->as_add_expr())) {

				if (LitExpr const *rlr = CheckNull(rl->rhs()->as_lit_expr()))
					CheckEq(interval(3), rlr->value());

				if (MulExpr const *rll = CheckNull(rl->lhs()->as_mul_expr())) {

					if (LitExpr const *rlll = CheckNull(rll->lhs()->as_lit_expr()))
						CheckEq(interval(2), rlll->value());

					if (VarExpr const *rllr = CheckNull(rll->rhs()->as_var_expr()))
						CheckEq("x", rllr->name());
				}
			}
		}
	}
}


void DeriveTest::partials() {
	ExprSPtr x = get_expr("f(x, y, z) = x^2 + x*y + y*z + 2*z");
	FuncExpr const *fe = x->as_func_expr();
	if (!CheckNull(fe)) return;
	// ∂f/∂x = 2*x + y
	// ∂f/∂y = x + z
	// ∂f/∂z = y + 2
	using namespace std;
	vector<pair<string, ExprSPtr>> ps;
	Derivator::partials(*fe, ps);

	Check(ps.size() == 3);
	for (auto &p : ps)
		p.second = Simplifier::simplified(p.second);

	ExprSPtr const &dx = ps[0].second;
	ExprSPtr const &dy = ps[1].second;
	ExprSPtr const &dz = ps[2].second;

	if (AddExpr const *ax = CheckNull(dx->as_add_expr())) {
		// ∂f/∂x = 2*x + y
		ExprSPtr l = ax->lhs(), r = ax->rhs();
		if (!l->as_mul_expr() && r->as_mul_expr()) std::swap(l, r);
		if (MulExpr const *me = CheckNull(l->as_mul_expr())) {
			ExprSPtr ll = me->lhs(), lr = me->rhs();
			if (!ll->as_lit_expr() && lr->as_lit_expr()) std::swap(ll, lr);
			if (LitExpr const *le = CheckNull(ll->as_lit_expr()))
				CheckEq(interval(2), le->value());
			if (VarExpr const *ve = CheckNull(lr->as_var_expr()))
				CheckEq("x", ve->name());
		}
		if (VarExpr const *ve = CheckNull(r->as_var_expr()))
			CheckEq("y", ve->name());
	}

	if (AddExpr const *ay = CheckNull(dy->as_add_expr())) {
		// ∂f/∂y = x + z
		ExprSPtr l = ay->lhs(), r = ay->rhs();
		if (CheckNull(l->as_var_expr()) && CheckNull(r->as_var_expr())) {
			VarExpr const *ve0 = l->as_var_expr();
			VarExpr const *ve1 = r->as_var_expr();
			// need to allow both orderings x+z, z+x
			if (ve0->name() == "x") {
				CheckEq("x", ve0->name());
				CheckEq("z", ve1->name());
			} else {//if (ve1->name() == "x") {
				CheckEq("z", ve0->name());
				CheckEq("x", ve1->name());
			}
		}
	}

	if (AddExpr const *az = CheckNull(dz->as_add_expr())) {
		// ∂f/∂z = y + 2
		ExprSPtr l = az->lhs(), r = az->rhs();
		if (!l->as_var_expr() && r->as_var_expr()) std::swap(l, r);
		if (VarExpr const *ve = CheckNull(l->as_var_expr()))
			CheckEq("y", ve->name());
		if (LitExpr const *le = CheckNull(r->as_lit_expr()))
			CheckEq(interval(2), le->value());
	}
}




}

