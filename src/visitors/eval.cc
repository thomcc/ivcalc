#include <iostream>
#include "visitors/eval.hh"
#include "parser/parser.hh"
#include "visitors/derivator.hh"
#include "visitors/simplifier.hh"
#include "visitors/replacer.hh"

namespace calc {

using namespace std;

Evaluator::Evaluator(Env &env) : _env(env), _res(0) {}
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
	if (!_env.get(e, _res))
		throw iv_arithmetic_error("Error: unbound variable '" + e.name() + "'");
}

void Evaluator::visit(ExptExpr &e) {
	interval v = eval(*e.base());
	_res = int_pow(v, e.power());
}

void Evaluator::visit(LitExpr &e) { _res = e.value(); }

void Evaluator::visit(CallExpr &e) {
	vector<interval> parms;
	call_args(e, parms);
	_res = _env.apply(e.name(), parms);
}


void Evaluator::call_args(CallExpr const &e, vector<interval> &parms) {
	for (auto const &eptr : e.args()) parms.push_back(eval(*eptr));
}

void Evaluator::visit(EmptyExpr &e) {}

void Evaluator::visit(FuncExpr &e) { _env.def(e.name(), e.params(), e.impl()); _res = interval::empty(); }

vector<interval> PartialCalc::calculate(vector<interval> const &args) {
	vector<ExprSPtr> es;
	for (auto const &iv : args) es.push_back(Expr::make<LitExpr>(iv));
	return calculate(es);
}

PartialCalc::PartialCalc(FuncExpr const &fe) : _ctx(), _fname(fe.name()), _pnames(fe.params()) {
	initialize(fe);
}

PartialCalc::PartialCalc(FuncExpr const &fe, Evaluator &ev)
	: _ctx(ev), _fname(fe.name()), _pnames(fe.params()) { initialize(fe); }

void PartialCalc::initialize(FuncExpr const &fe) {
	string deriv_prefix = stringize() << "∂" << _fname << "/∂";
	_pf_names.push_back(fe.name());
	ExprSPtr ffe = Expr::make<FuncExpr>(fe.name(), fe.params(), Simplifier::simplified(fe.impl()));
	_fpartials.push_back(ffe);
	_ctx.eval(*ffe);
	for (auto const &i : Derivator::partials(fe)) {
		_pf_names.push_back(deriv_prefix+i.first);
		ExprSPtr pfe = Expr::make<FuncExpr>(_pf_names.back(), _pnames, Simplifier::simplified(i.second));
		_fpartials.push_back(pfe);
		_ctx.eval(*pfe);
	}
}


vector<interval>
PartialCalc::calculate(vector<ExprSPtr> const &args) {
	vector<interval> res(_fpartials.size(), interval::empty());
	size_t as = args.size();
	if (as != _pnames.size()) throw iv_arithmetic_error("Wrong number of arguments");

	for (auto const &name : _pf_names)
		res.push_back(_ctx.eval(*Expr::make<CallExpr>(name, args)));
	return res;
}
PartialCalc::PartialCalc() {}


}