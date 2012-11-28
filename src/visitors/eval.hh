#ifndef __EVAL_HH__
#define __EVAL_HH__

#include "expr.hh"
#include "visitors/visitorbase.hh"
#include "interval.hh"
#include "visitors/env.hh"

namespace calc {


class PartialCalc;

class Evaluator : public ExprVisitor {
	Env _env;
	interval _res;
	friend class PartialCalc;
public:

	Evaluator() : _env(Env::global()), _res(0) {}
	Evaluator(Env &env);

	interval eval(Expr &e) {
		_res = 0;
		e.accept(*this);
		return _res;
	}
	void visit(Expr &e) { assert(0); } // impossible
	void visit(AddExpr &e);
	void visit(SubExpr &e);
	void visit(NegExpr &e);
	void visit(MulExpr &e);
	void visit(DivExpr &e);
	void visit(VarExpr &e);
	void visit(ExptExpr &e);
	void visit(LitExpr &e);
	void visit(CallExpr &e);
	void visit(EmptyExpr &e);
	void visit(FuncExpr &e);
	void call_args(CallExpr const &e, std::vector<interval> &argres);
};

// stores info necessary for repeated interval partial calculations

class PartialCalc {
	Evaluator _ctx; // env with partial functions defined
	std::string _fname; // original func name
	std::vector<std::string> _pnames, _pf_names; // param names and partial func names
	std::vector<ExprPtr> _fpartials; // funcexprs representing the partials
	void initialize(FuncExpr const &fe);
public:
	PartialCalc();
	PartialCalc(FuncExpr const &fe, Evaluator &ev);
	PartialCalc(FuncExpr const &fe);
	PartialCalc(PartialCalc const &) = default;
	PartialCalc&operator=(PartialCalc const &);
	PartialCalc&operator=(PartialCalc&&) = default;
	std::string const &func_name() const { return _fname; }
	void call_args(CallExpr const &ce, std::vector<interval> &parms) { _ctx.call_args(ce, parms); }
	std::vector<std::string> const &params() const { return _pnames; }
	std::vector<ExprPtr> const &partials() const { return _fpartials; }
	size_t partial_count() const { return _fpartials.size() - 1; }
	size_t nargs() const { return _pnames.size(); }
	size_t expr_count() const { return _fpartials.size(); }
	std::vector<interval> calculate(std::vector<interval> const &args);
	void apply(std::vector<pod_interval> &dst, std::vector<pod_interval> const &src);
	std::vector<interval> calculate(std::vector<ExprPtr> const &args);
};


}
#endif