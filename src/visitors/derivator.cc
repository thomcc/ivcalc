#include <map>
#include "parser/parser.hh"
#include "visitors/replacer.hh"
#include "visitors/derivator.hh"
namespace calc {

using namespace std;

Derivator::Derivator(string const &v): _var(v), _derived(nullptr) {}

ExprPtr Derivator::derive(Expr &e) {
	_derived = nullptr;
	e.accept(*this);
	return move(_derived);
}

void Derivator::visit(AddExpr &e) {
	ExprPtr left = derive(*e.lhs());
	ExprPtr right = derive(*e.rhs());
	_derived = Expr::make_add(move(left), move(right));
}

void Derivator::visit(SubExpr &e) {
	ExprPtr left = derive(*e.lhs());
	ExprPtr right = derive(*e.rhs());
	_derived = Expr::make_sub(move(left), move(right));
}

void Derivator::visit(NegExpr &e) {
	ExprPtr v = derive(*e.value());
	_derived = Expr::make_neg(move(v));
}

void Derivator::visit(MulExpr &e) {
	ExprPtr left = derive(*e.lhs());
	ExprPtr right = derive(*e.rhs());
	ExprPtr rr = Expr::make_mul(move(right), e.lhs()->clone());
	ExprPtr ll = Expr::make_mul(e.rhs()->clone(), move(left));
	_derived = Expr::make_add(move(rr), move(ll));
}

void Derivator::visit(DivExpr &e) {
	ExprPtr dl = derive(*e.lhs());
	ExprPtr dr = derive(*e.rhs());
	ExprPtr denom = Expr::make_expt(e.rhs()->clone(), 2);
	ExprPtr nl = Expr::make_mul(e.rhs()->clone(), move(dl));
	ExprPtr nr = Expr::make_mul(move(dr), e.lhs()->clone());
	ExprPtr numer = Expr::make_sub(move(nl), move(nr));
	_derived = Expr::make_div(move(numer), move(denom));
}

void Derivator::visit(VarExpr &e) { _derived = Expr::make_lit(e.name() == _var ? 1 : 0); }
void Derivator::visit(LitExpr &e) { _derived = Expr::make_lit(0); }
void Derivator::visit(FuncExpr &e) { _derived = Expr::make_func(e.name(), e.params(), derive(*e.impl())); }

void Derivator::visit(ExptExpr &e) {
	ExprPtr left = derive(*e.base());
	ExprPtr pc = Expr::make_lit(e.power());
	ExprPtr npow = Expr::make_expt(e.base()->clone(), e.power()-1);
	ExprPtr mul = Expr::make_mul(move(left), move(npow));
	_derived = Expr::make_mul(move(pc), move(mul));
}
static struct {
	const char *name, *impl;
	ExprPtr cached;
} dx_rules_ary[] = {
	{"log", "1 / _1", ExprPtr{nullptr}},
	{"exp", "exp(_1)", ExprPtr{nullptr}},
	{"abs", "sgn(_1)", ExprPtr{nullptr}},
	{"sqrt", "1 / (2 * sqrt(_1))", ExprPtr{nullptr}},

	{"sin", "cos(_1)", ExprPtr{nullptr}},
	{"cos", "-sin(_1)", ExprPtr{nullptr}},
	{"tan", "1 / (cos(_1)^2)", ExprPtr{nullptr}},

	{"sec", "sec(_1) * tan(_1)", ExprPtr{nullptr}},
	{"csc", "-csc(_1) * cot(_1)", ExprPtr{nullptr}},
	{"cot", "sec(_1)^2", ExprPtr{nullptr}},

	{"asin", "1 / sqrt(1 - _1^2)", ExprPtr{nullptr}},
	{"acos", "-1 / sqrt(1 - _1^2)", ExprPtr{nullptr}},
	{"atan", "1 / (1 + _1^2)", ExprPtr{nullptr}},

	{"asec", "1 / (_1 * sqrt(_1^2 - 1))", ExprPtr{nullptr}},
	{"acsc", "-1 / (_1 * sqrt(_1^2 - 1))", ExprPtr{nullptr}},
	{"acot", "-1 / (1 + _1^2)", ExprPtr{nullptr}},

	{"sinh", "cosh(_1)", ExprPtr{nullptr}},
	{"cosh", "sinh(_1)", ExprPtr{nullptr}},
	{"tanh", "(1 / cosh(_1))^2", ExprPtr{nullptr}},

	{"asinh", "1 / (sqrt(1 + _1^2))", ExprPtr{nullptr}},
	{"acosh", "1 / (sqrt(_1 - 1) * sqrt(_1 + 1))", ExprPtr{nullptr}},
	{"atanh", "1 / (1 - _1^2)", ExprPtr{nullptr}},
	{nullptr, nullptr, ExprPtr{nullptr}},
};

ExprPtr find_rule(std::string const &name) {
	const char *s = name.c_str();
	for (size_t p = 0; dx_rules_ary[p].name != nullptr; ++p) {
		const char *s1 = dx_rules_ary[p].name, *s2 = s;
		while (*s1 == *s2++) {
			if (*s1++ == 0) { // found
				if (dx_rules_ary[p].cached.get() == nullptr) {
					ErrorHandler eh(true, false);
					Parser parser(string(dx_rules_ary[p].impl), eh);
					ExprPtr eptr = parser.parse_expression();
					if ((eh.errors() != 0) || !eptr.get()) return nullptr;
					dx_rules_ary[p].cached = move(eptr);
				}
				return dx_rules_ary[p].cached->clone();
			}
		}
	}
	return nullptr;
}

/*
static map<string, string> dx_rules{
	{"log", "1 / _1"},
	{"exp", "exp(_1)"},
	{"abs", "sgn(_1)"},
	{"sqrt", "1 / (2 * sqrt(_1))"},

	{"sin", "cos(_1)"},
	{"cos", "-sin(_1)"},
	{"tan", "1 / (cos(_1)^2)"},

	{"sec", "sec(_1) * tan(_1)"},
	{"csc", "-csc(_1) * cot(_1)"},
	{"cot", "sec(_1)^2"},

	{"asin", "1 / sqrt(1 - _1^2)"},
	{"acos", "-1 / sqrt(1 - _1^2)"},
	{"atan", "1 / (1 + _1^2)"},

	{"asec", "1 / (_1 * sqrt(_1^2 - 1))"},
	{"acsc", "-1 / (_1 * sqrt(_1^2 - 1))"},
	{"acot", "-1 / (1 + _1^2)"},

	{"sinh", "cosh(_1)"},
	{"cosh", "sinh(_1)"},
	{"tanh", "(1 / cosh(_1))^2"},

	{"asinh", "1 / (sqrt(1 + _1^2))"},
	{"acosh", "1 / (sqrt(_1 - 1) * sqrt(_1 + 1))"},
	{"atanh", "1 / (1 - _1^2)"},
};*/

static vector<string> placeholder_vars{ "_1", "_2", "_3", "_4", "_5", "_6", "_7", "_8", "_9", "_10"};

static ExprPtr fill(ExprPtr e, vector<ExprPtr> const &to) {
	map<string, ExprPtr> repl;
	assert(to.size() < placeholder_vars.size());
	for (size_t i = 0; i < to.size(); ++i)
		repl[placeholder_vars.at(i)] = to.at(i)->clone();
	return Replacer(move(repl)).replace(*e);
}


void Derivator::visit(CallExpr &e) {
	if (e.args().size() > 1)
		throw iv_arithmetic_error("Not implemented: differentiating multivariate functions in expressions");
//	string const &name = e.name();
//	auto it = dx_rules.find(name);
	ExprPtr expr = find_rule(e.name());
	if (!expr.get())
		throw iv_arithmetic_error("Can't derive '"+e.name()+"'.");
//	ErrorHandler eh(true, false);
//	Parser p(it->second, eh);
//	ExprPtr eptr = p.parse_expression();
//	if ((eh.errors() != 0) || !eptr.get())
//		throw iv_arithmetic_error("Bug: failed parse.");
	ExprPtr df = fill(move(expr), e.args());
	ExprPtr dg = derive(*e.args().at(0));
	_derived = Expr::make_mul(move(df), move(dg));
}


void Derivator::partials(FuncExpr const &e, vector<pair<string, ExprPtr>> &dest) {
	dest.clear();
	for (auto const &param : e.params()) {
		Derivator dparam(param);
		dest.push_back(make_pair(param, dparam.derive(*e.impl())));
	}
}



}