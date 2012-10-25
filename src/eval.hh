#ifndef __EVAL_HH__
#define __EVAL_HH__
#include "expr.hh"
#include "visitorbase.hh"
#include "interval.hh"
#include <map>
namespace calc {

class Env {
	std::map<std::string, interval> _impl;
public:
	Env() {}

	bool
	has(std::string const &s) {
		return _impl.find(s) != _impl.end();
	}

	bool
	get(std::string const &s, interval &i) {
		try {
			interval r = _impl.at(s);
			i = r;
		} catch (...) {
			return false;
		}
		return true;
	}

	void
	put(std::string const &s, interval v) {
		_impl[s] = v;
	}

	interval
	apply_function(std::string const &name, std::vector<interval> const &args);
	
};



class Evaluator
: public BaseVisitor
, public Visitor<AddExpr>
, public Visitor<SubExpr>
, public Visitor<NegExpr>
, public Visitor<MulExpr>
, public Visitor<DivExpr>
, public Visitor<VarExpr>
, public Visitor<ExptExpr>
, public Visitor<LitExpr>
, public Visitor<AssignExpr>
, public Visitor<CallExpr>
, public Visitor<EmptyExpr> 
{

	Env _env;
	interval _res;
public:

	Evaluator() : _env(), _res(0) {}

	interval
	eval(ExprSPtr e)  {
		_res = 0;
		e->accept(*this);
		return _res;
	}

	void
	visit(AddExpr &e) {
		interval lhs = eval(e.lhs());
		interval rhs = eval(e.rhs());
		_res = lhs + rhs;
	}

	void
	visit(SubExpr &e) {
		interval lhs = eval(e.lhs());
		interval rhs = eval(e.rhs());
		_res = lhs - rhs;
	}

	void
	visit(NegExpr &e) {
		interval vv = eval(e.value());
		_res = -vv;
	}

	void
	visit(MulExpr &e) {
		interval lhs = eval(e.lhs());
		interval rhs = eval(e.rhs());
		_res = lhs * rhs;
	}

	void
	visit(DivExpr &e) {
		interval lhs = eval(e.lhs());
		interval rhs = eval(e.rhs());
		_res = lhs / rhs;
	}
	
	void
	visit(VarExpr &e) {
		if (!_env.has(e.name()) || !_env.get(e.name(), _res))
			throw "Error: unbound variable '" + e.name() + "'";
	}

	void
	visit(ExptExpr &e) {
		interval v = eval(e.base());
		_res = v.expt(e.power());
	}

	void
	visit(LitExpr &e) {
		_res = e.value();
	}
	
	void
	visit(AssignExpr &e) {
		interval v = eval(e.value());
		_env.put(e.name(), v);
		_res = v;
	}

	void
	visit(CallExpr &e) {
		std::vector<interval> parms;
		for (ExprSPtr expr : e.args())
			parms.push_back(eval(expr));
		_res = _env.apply_function(e.name(), parms);
	}

	void
	visit(EmptyExpr &e) {}


};






}












#endif