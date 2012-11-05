#include "eval.hh"
#include <parser/parser.hh>
#include <iostream>
namespace calc {

unsigned long BaseFunc::gs_entropy = 0;

using namespace std;

Evaluator::Evaluator(Env &env) : _env(env), _res(0) {}
Env::Env() : _parent(nullptr) {}

interval UserDefinedFn::apply(vector<interval> const &args, Env const &e) const {
	Env env = (e.extend(_params, args));
	Evaluator eval(env);
	return eval.eval(*_impl);
}

bool Env::has(string const &s) const { interval dummy; return get(s, dummy); }
bool Env::get(string const &s, interval &i) const {
	auto it = _vars.find(s);
	if (it != _vars.end()) { i = it->second; return true; }
	else if (_parent) return _parent->get(s, i);
	else return false;
}

interval Env::apply(string const &s, vector<interval> const &args) const {
	BaseFunc *bf = nullptr;
	Env const *p = this;
	do {
		auto it = p->_funcs.find(s);
		if (it != p->_funcs.end()) {
			bf = it->second.get();
			if (bf) break;
		}
	} while ((p = p->_parent));
	if (!bf) throw iv_arithmetic_error("Unknown function: '" + s + "'");
	return bf->call(args, *this);
}

void Env::put(string const &s, interval e) { _vars[s] = e; }

Env Env::extend(vector<string> const &params, vector<interval> const &args) const {
	Env e;
	e._parent = this;
	assert(params.size() == args.size()); // should be checked before we get here
	for (size_t i = 0; i < params.size(); ++i) e._vars[params.at(i)] = args.at(i);
	return e;
}

Env Env::global() {
	Env e;

#define FN1(NAME) e.add_func<Fn1Arg>(#NAME, NAME)
#define FN2(NAME) e.add_func<Fn2Args>(#NAME, NAME)

	FN1(exp);   FN1(log);   FN1(sin);   FN1(cos);   FN1(tan);
	FN1(asin);  FN1(acos);  FN1(atan);  FN1(sinh);  FN1(cosh);
	FN1(tanh);  FN1(acosh); FN1(asinh); FN1(atanh); FN2(power);
	FN2(hull);  FN1(abs);   FN1(sgn);   FN1(sqrt);  FN1(square);

#undef FN2
#undef FN1

	e.add_func<Fn2Args>("max", iv_max);
	e.add_func<Fn2Args>("min", iv_min);
	e.add_func<Fn1Arg>("mid", midpoint);
	e.add_func<Fn1Arg>("left", leftendpoint);
	e.add_func<Fn1Arg>("right", rightendpoint);
	e.add_builtins({
		"sec(x) = 1 / cos(x)",
		"csc(x) = 1 / sin(x)",
		"cot(x) = 1 / tan(x)",

		"asec(x) = acos(1/x)",
		"acsc(x) = asin(1/x)",
		"acot(x) = atan(1/x)",


		"sech(x) = 1 / cosh(x)",
		"csch(x) = 1 / sinh(x)",
		"coth(x) = cosh(x) / sinh(x)",

		"asech(x) = acosh(1/x)",
		"acsch(x) = asinh(1/x)",
		"acoth(x) = atanh(1/x)"
	});

	e.put("pi", interval::pi());
	e.put("two_pi", interval::two_pi());
	return e;

}

void Env::add_builtin(string const &text) {
	ErrorHandler eh(true, false);
	Parser p(text, eh);
	ExprSPtr eptr = p.parse_expression();
	if ((eh.errors() != 0) || !eptr.get())
		throw iv_arithmetic_error("Bug: error adding builtin: parser error");
	FuncExpr const *fe = eptr->as_func_expr();
	if (!fe)
		throw iv_arithmetic_error("Bug: error adding builtin: incorrectly parsed.");
	def(fe->name(), fe->params(), fe->impl());
}

void Env::add_builtins(initializer_list<string> const &funcs) {
	for (auto const &func : funcs) {
		try {
			add_builtin(func);
		} catch (...) {
			cerr << "Warning: failed to add builtin `" << func << "`." << endl;
		}
	}
}

void Env::def(string const &name, vector<string> const &prams, ExprSPtr v) { add_func<UserDefinedFn>(name, prams, v); }


void Evaluator::visit(AddExpr &e) {
	interval lhs = eval(*e.lhs());
	interval rhs = eval(*e.rhs());
	_res = lhs + rhs;
}

void Evaluator::visit(SubExpr &e) {
	interval lhs = eval(*e.lhs());
	interval rhs = eval(*e.rhs());
	_res = lhs - rhs;
}

void Evaluator::visit(NegExpr &e) {
	interval vv = eval(*e.value());
	_res = -vv;
}

void Evaluator::visit(MulExpr &e) {
	interval lhs = eval(*e.lhs());
	interval rhs = eval(*e.rhs());
	_res = lhs * rhs;
}

void Evaluator::visit(DivExpr &e) {
	interval lhs = eval(*e.lhs());
	interval rhs = eval(*e.rhs());
	_res = lhs / rhs;
}

void Evaluator::visit(VarExpr &e) {
	if (!_env.has(e.name()) || !_env.get(e.name(), _res))
		throw iv_arithmetic_error("Error: unbound variable '" + e.name() + "'");
}

void Evaluator::visit(ExptExpr &e) {
	interval v = eval(*e.base());
	_res = int_pow(v, e.power());
}

void Evaluator::visit(LitExpr &e) { _res = e.value(); }
void Evaluator::visit(AssignExpr &e) {
	interval v = eval(*e.value());
	_env.put(e.name(), v);
	_res = v;
}

void Evaluator::visit(CallExpr &e) {
	std::vector<interval> parms;
	for (auto const &eptr : e.args()) parms.push_back(eval(*eptr));
	_res = _env.apply(e.name(), parms);
}

void Evaluator::visit(EmptyExpr &e) {}


void Evaluator::visit(FuncExpr &e) { _env.def(e.name(), e.params(), e.impl()); _res = interval::empty(); }

}