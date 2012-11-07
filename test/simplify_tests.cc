#include "parser/parser.hh"
#include "visitors/simplifier.hh"

#include "simplify_tests.hh"

namespace calc {

ExprSPtr SimplifyTest::get_expr(std::string const &str, bool simplify) const {
	ErrorHandler eh(true, false);
	Parser p(str, eh);
	ExprSPtr eptr = p.parse_expression();
	Check(eptr.get());
	Check(eh.errors() == 0);
	if (simplify) {
		Simplifier s;
		ExprSPtr e2 = s.simplify(*eptr);
		Check(e2.get());
		return e2;
	} else return eptr;
}

void SimplifyTest::add_zero() {
	ExprSPtr opz = get_expr("0+o");
	Check(opz->as_var_expr());
	Check(opz->as_var_expr() && (opz->as_var_expr()->name() == "o"));
	ExprSPtr zpo = get_expr("o+0");
	Check(zpo->as_var_expr());
	Check(zpo->as_var_expr() && (zpo->as_var_expr()->name() == "o"));
}

void SimplifyTest::sub_zero() {
	ExprSPtr opz = get_expr("o-0");
	Check(opz->as_var_expr());
	Check(opz->as_var_expr() && (opz->as_var_expr()->name() == "o"));
	ExprSPtr zpo = get_expr("0-o");
	NegExpr const *ne = zpo->as_neg_expr();
	Check(ne);
	if (!ne) return;
	Check(ne->value()->as_var_expr());
	Check(ne->value()->as_var_expr() && (ne->value()->as_var_expr()->name() == "o"));
}

void SimplifyTest::mul_zero() {
	ExprSPtr otz = get_expr("o*0");
	Check(otz->as_lit_expr());
	Check(otz->is_lit_zero());
	ExprSPtr zto = get_expr("0*o");
	Check(zto->as_lit_expr());
	Check(zto->is_lit_zero());
}

void SimplifyTest::zero_div() {
	ExprSPtr odz = get_expr("0/o");
	Check(odz->as_lit_expr());
	Check(odz->is_lit_zero());
}

void SimplifyTest::pow_zero() {
	ExprSPtr op0 = get_expr("o^0");
	Check(op0->as_lit_expr());
	Check(op0->is_lit_one());
}

void SimplifyTest::mul_one() {
	ExprSPtr opz = get_expr("1*o");
	Check(opz->as_var_expr());
	Check(opz->as_var_expr() && (opz->as_var_expr()->name() == "o"));
	ExprSPtr zpo = get_expr("o*1");
	Check(zpo->as_var_expr());
	Check(zpo->as_var_expr() && (zpo->as_var_expr()->name() == "o"));
}

void SimplifyTest::div_one() {
	ExprSPtr opz = get_expr("o/1");
	Check(opz->as_var_expr());
	Check(opz->as_var_expr() && (opz->as_var_expr()->name() == "o"));
}

void SimplifyTest::pow_one() {
	ExprSPtr opz = get_expr("o^1");
	Check(opz->as_var_expr());
	Check(opz->as_var_expr() && (opz->as_var_expr()->name() == "o"));
}

void SimplifyTest::constant_fold() {
	ExprSPtr opz = get_expr("12 + 4 * 3 / 2");
	Check(opz->as_lit_expr());
	Check(opz->as_lit_expr() && (opz->as_lit_expr()->value() == interval(18)));

}

}