#include "parser_tests.hh"
#include "parser/parser.hh"
#include "visitors/printer.hh"
#include <vector>
namespace calc {
using namespace std;
void ParserTest::add() {
//	ErrorHandler eh(true, false);
	Parser p("1 + 2", ErrorHandler::make_silent());
	ExprPtr e = p.parse_expression();
	Check(!!p);
//	cout << "add: " << *e << endl;
	AddExpr const *e2 = e->as_add_expr();
	Check(e2);
	if (e2) {

		LitExpr const *lhs = e2->lhs()->as_lit_expr();

		LitExpr const *rhs = e2->rhs()->as_lit_expr();
		Check(lhs);
		Check(rhs);
		if (lhs && rhs) {
			Check(lhs->value().is_one());
			Check(rhs->value() == interval(2));
		}
	}
}

void ParserTest::sub() {
//	ErrorHandler eh(true, false);
	Parser p("1-2", ErrorHandler::make_silent());
	ExprPtr e = p.parse_expression();
	Check(!!p);
//	cout << "sub: " << *e << endl;
	SubExpr const *e2 = e->as_sub_expr();
	Check(e2);
	if (e2) {
		LitExpr const *lhs = e2->lhs()->as_lit_expr();
		LitExpr const *rhs = e2->rhs()->as_lit_expr();
		Check(lhs);
		Check(rhs);
		if (lhs && rhs) {
			Check(lhs->value().is_one());
			Check(rhs->value() == interval(2));
		}
	}
}

void ParserTest::mul() {
//	ErrorHandler eh(true, false);
	Parser p("1 * 2", ErrorHandler::make_silent());
	ExprPtr e = p.parse_expression();
	Check(!!p);
//	cout << "mul: " << *e << endl;
	MulExpr const *e2 = e->as_mul_expr();
	Check(e2);
	if (e2) {
		LitExpr const *lhs = e2->lhs()->as_lit_expr();
		LitExpr const *rhs = e2->rhs()->as_lit_expr();
		Check(lhs);
		Check(rhs);
		if (lhs && rhs) {
			Check(lhs->value().is_one());
			Check(rhs->value() == interval(2));
		}
	}
}

void ParserTest::div() {
	Parser p("1 / 2", ErrorHandler::make_silent());
	ExprPtr e = p.parse_expression();
	Check(!!p);
//	cout << "div: " << *e << endl;
	DivExpr const *e2 = e->as_div_expr();
	Check(e2);
	if (e2) {
		LitExpr const *lhs = e2->lhs()->as_lit_expr();
		LitExpr const *rhs = e2->rhs()->as_lit_expr();
		Check(lhs);
		Check(rhs);
		if (lhs && rhs) {
			Check(lhs->value().is_one());
			Check(rhs->value() == interval(2));
		}
	}
}

void ParserTest::pow() {
//	ErrorHandler eh(true, false);
	Parser p("1 / 1 ^ 2", ErrorHandler::make_silent());
	ExprPtr e = p.parse_expression();
	Check(!!p);
//	cout << "pow: " << *e << endl;
	if (DivExpr const *ed = CheckNull(e->as_div_expr())) {
		if (LitExpr const *e1 = CheckNull(ed->lhs()->as_lit_expr()))
			CheckEq(e1->value(), interval::one());
		if (ExptExpr const *e2 = CheckNull(ed->rhs()->as_expt_expr())) {
			LitExpr const *lhs = e2->base()->as_lit_expr();
			int power = e2->power();
			Check(lhs);
			Check(power == 2);
		}
	}
}

void ParserTest::lit() {
//	ErrorHandler eh(true, false);
	Parser p("[1, 3]", ErrorHandler::make_silent());
	ExprPtr e = p.parse_expression();
	Check(!!p);
//	cout << "lit: " << *e << endl;
	LitExpr const *e2 = e->as_lit_expr();
	Check(e2);
	if (e2) {
		real lo = e2->value().lo();
		real hi = e2->value().hi();
		CheckEq(static_cast<real>(1), lo);
		CheckEq(static_cast<real>(3), hi);
	}
}

void ParserTest::precedence() {
//	ErrorHandler eh(true, false);
	Parser p("1 + 0 * 1 ^ 3 / -do_stuff(a^2, b)", ErrorHandler::make_silent());
	// should parse as 1 + ((0 * (1^3)) / (-do_stuff((a^2), b)))
	ExprPtr e = p.parse_expression();
	Check(!!p);
//	cout << "precedence: " << *e << endl;
	AddExpr const *e2 = e->as_add_expr();
	Check(e2);
	if (e2) {
		LitExpr const *lhs = e2->lhs()->as_lit_expr();
		DivExpr const *rhs = e2->rhs()->as_div_expr();
		Check(lhs);
		Check(lhs && lhs->value().is_one());
		Check(rhs);
		if (rhs) {
			// 0 * 1^3 / -do_stuff(a^2, b)
			MulExpr const *me = rhs->lhs()->as_mul_expr();
			NegExpr const *ne = rhs->rhs()->as_neg_expr();
			Check(ne);
			if (ne) {
				// -do_stuff(a^2, b)
				CallExpr const *ce = ne->value()->as_call_expr();
				Check(ce);
				if (ce) {
					// do_stuff(a^2, b)
					Check(ce->name() == "do_stuff");
					std::vector<ExprPtr> const &args = ce->args();
					// b
					VarExpr const *b = args.at(1)->as_var_expr();
					Check(b);
					Check(b && (b->name() == "b"));
					// a^2
					ExptExpr const *a2 = args.at(0)->as_expt_expr();
					Check(a2);
					Check(a2 && (a2->power() == 2));
					// laziness to get a.
					Check(a2 && a2->base()->as_var_expr());
					Check(a2 && (a2->base()->as_var_expr() && (a2->base()->as_var_expr()->name() == "a")));
				}
			}
			Check(me);
			if (me) {
				// (0 * 1^3)
				LitExpr const *le = me->lhs()->as_lit_expr();
				Check(le);
				Check(le && le->value().is_zero());

				ExptExpr const *ee = me->rhs()->as_expt_expr();
				Check(ee);
				if (ee) {
					// 1^3
					Check(ee->power() == 3);
					LitExpr const *be = ee->base()->as_lit_expr();
					Check(be);
					Check(be && be->value().is_one());

				}
			}
		}
	}
}

void ParserTest::group() {
//	ErrorHandler eh(true, false);
	Parser p("1 * (1 + 0)", ErrorHandler::make_silent());
	ExprPtr e = p.parse_expression();
	Check(!!p);
//	cout << "group: " << *e << endl;
	MulExpr const *e2 = e->as_mul_expr();
	Check(e2);
	if (e2) {
		LitExpr const *lhs = e2->lhs()->as_lit_expr();
		AddExpr const *rhs = e2->rhs()->as_add_expr();
		Check(lhs);
		if (lhs) Check(lhs->value().is_one());
		Check(rhs);
		if (rhs) {
			LitExpr const *rlhs = rhs->lhs()->as_lit_expr();
			LitExpr const *rrhs = rhs->rhs()->as_lit_expr();
			Check(rlhs && rlhs->value().is_one());
			Check(rrhs && rrhs->value().is_zero());
		}
	}
}


void ParserTest::func() {
//	ErrorHandler eh(true, false);
	Parser p("f(x) = x", ErrorHandler::make_silent());
	ExprPtr e = p.parse_expression();
	Check(!!p);
//	cout << "func: " << *e << endl;
	if (!CheckNull(e.get())) return;
	if (FuncExpr const *fe = CheckNull(e->as_func_expr())) {
		std::vector<std::string> const &ps = fe->params();
		CheckEq(1, ps.size());
		CheckEq("x", ps.at(0));
		CheckEq("f", fe->name());
		if (VarExpr const *ve = CheckNull(fe->impl()->as_var_expr()))
			CheckEq("x", ve->name());
	}
}

}