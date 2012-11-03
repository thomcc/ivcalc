#include "eval.hh"

namespace calc {

unsigned long BaseFunc::gs_entropy = 0;

using namespace std;

Evaluator::Evaluator(Env &env) : _env(env), _res(0) {}

Env::Env() : _parent(nullptr) {}

interval
UserDefinedFn::apply(std::vector<interval> const &args, Env const &e) const {
	Env env = (e.extend(_params, args));
	Evaluator eval(env);
	return eval.eval(*_impl);
}

bool
Env::has(string const &s) const {
	interval dummy;
	return get(s, dummy);
}

bool
Env::get(string const &s, interval &i) const {
	auto it = _vars.find(s);
	if (it != _vars.end()) { i = it->second; return true; }
	else if (_parent) return _parent->get(s, i);
	else return false;
}

interval
Env::apply(string const &s, vector<interval> const &args) const {
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

void
Env::put(string const &s, interval e) {
	_vars[s] = e;
}

Env
Env::extend(vector<string> const &params, vector<interval> const &args) const {
	Env e;
	e._parent = this;
	assert(params.size() == args.size()); // should be checked before we get here
	for (size_t i = 0; i < params.size(); ++i)
		e._vars[params.at(i)] = args.at(i);
	return e;
}

Env
Env::global() {
	Env e;

#define FN1(NAME) e.add_func<Fn1Arg>(#NAME, NAME)
#define FN2(NAME) e.add_func<Fn2Args>(#NAME, NAME)

	FN1(exp);
	FN1(log);
	FN1(sin);
	FN1(cos);
	FN1(tan);
	FN1(asin);
	FN1(acos);
	FN1(atan);
	FN2(power);
	FN2(hull);
	FN1(abs);
	FN1(sgn);
	FN1(sqrt);
	FN1(square);

#undef FN2
#undef FN1

	e.add_func<Fn2Args>("iroot", integral_root);
	e.add_func<Fn2Args>("max", iv_max);
	e.add_func<Fn2Args>("min", iv_min);
	e.add_func<Fn1Arg>("mid", midpoint);
	e.add_func<Fn1Arg>("left", leftendpoint);
	e.add_func<Fn1Arg>("right", rightendpoint);

	e.put("pi", interval::pi());
	e.put("two_pi", interval::two_pi());
	return e;

}

void
Env::def(std::string const &name, std::vector<std::string> const &prams, ExprSPtr v) {
	add_func<UserDefinedFn>(name, prams, v);
}



}