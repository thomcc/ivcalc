#include "derivator.hh"
#include <map>
#include <parser/parser.hh>
#include "replacer.hh"

namespace calc {
using namespace std;
Derivator::Derivator(string const &v)
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

void
Derivator::visit(FuncExpr &e) {
	ExprSPtr dimpl = derive(*e.impl());
	_derived = Expr::make<FuncExpr>(e.name(), e.params(), dimpl);
}

static map<string, string> dx_rules{
	{"log", "1 / _1"},
	{"exp", "exp(_1)"},
	{"abs", "sgn(_1)"},
	{"square", "2 * _1"},
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
};

static vector<string> placeholder_vars{
	"_1", "_2", "_3", "_4", "_5", "_6", "_7", "_8", "_9", "_10"
};

static ExprSPtr
fill(ExprSPtr e, vector<ExprSPtr> const &to) {
	map<string, ExprSPtr> repl;
	assert(to.size() < placeholder_vars.size());
	for (size_t i = 0; i < to.size(); ++i)
		repl[placeholder_vars.at(i)] = to.at(i);
	return Replacer(repl).replace(*e);
}

void
Derivator::visit(CallExpr &e) {
	string const &name = e.name();
	auto it = dx_rules.find(name);
	if (it == dx_rules.end())
		throw iv_arithmetic_error("Can't derive '"+name+"'.");
	ErrorHandler eh(true, false);
	Parser p(it->second, eh);
	ExprSPtr eptr = p.parse_expression();
	if ((eh.errors() != 0) || !eptr.get()) throw iv_arithmetic_error("Bug: failed parse.");
	_derived = fill(eptr, e.args());
}


void
Derivator::partials(FuncExpr const &e, std::vector<std::pair<std::string, ExprSPtr>> &dest) {
	dest.clear();
	for (auto const &param : e.params()) {
		Derivator dparam(param);
		dest.push_back(make_pair(param, dparam.derive(*e.impl())));
	}
}



}