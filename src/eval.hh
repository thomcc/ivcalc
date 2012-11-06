#ifndef __EVAL_HH__
#define __EVAL_HH__
#include "expr.hh"
#include "visitorbase.hh"
#include "interval.hh"
#include "functions.hh"
#include <map>


namespace calc {





class Env {
	Env const *_parent;
	std::map<std::string, interval> _vars;
	std::map<std::string, std::shared_ptr<BaseFunc>> _funcs;
	void add_builtin(std::string const &func);
	void add_builtins(std::initializer_list<std::string> const &funcs);
public:
	Env();
	Env(Env const &) = default;
	Env &operator=(Env const &o) = default;
	bool has(std::string const &s) const;
	bool get(std::string const &s, interval &i) const;
	interval apply(std::string const &s, std::vector<interval> const &args) const;
	void put(std::string const &s, interval e);
	void def(std::string const &name, std::vector<std::string> const &prams, ExprSPtr v);
	// was getting repetitive...
	template <typename T, typename... Args> void add_func(Args&&... args) {
		std::shared_ptr<T> eptr = std::make_shared<T>(std::forward<Args>(args)...);
		std::shared_ptr<BaseFunc> bf = std::static_pointer_cast<BaseFunc>(eptr);
		_funcs[bf->name()] = bf;
	}
	Env extend(std::vector<std::string> const &params, std::vector<interval> const &args) const;
	static Env global();
};

class PartialCalc;

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
, public Visitor<FuncExpr>
{
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
	void visit(AddExpr &e);
	void visit(SubExpr &e);
	void visit(NegExpr &e);
	void visit(MulExpr &e);
	void visit(DivExpr &e);
	void visit(VarExpr &e);
	void visit(ExptExpr &e);
	void visit(LitExpr &e);
	void visit(AssignExpr &e);
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
	std::vector<ExprSPtr> _fpartials; // funcexprs representing the partials
	void initialize(FuncExpr const &fe);
public:
	PartialCalc();
	PartialCalc(FuncExpr const &fe, Evaluator &ev);
	PartialCalc(FuncExpr const &fe);
	PartialCalc(PartialCalc const &) = default;
	PartialCalc&operator=(PartialCalc const &) = default;
	std::string const &func_name() const { return _fname; }
	void call_args(CallExpr const &ce, std::vector<interval> &parms) { _ctx.call_args(ce, parms); }
	std::vector<std::string> const &params() const { return _pnames; }
	std::vector<ExprSPtr> const &partials() const { return _fpartials; }
	size_t partial_count() const { return _fpartials.size() - 1; }
	size_t expr_count() const { return _fpartials.size(); }
	std::vector<interval> calculate(std::vector<interval> const &args);
	std::vector<interval> calculate(std::vector<ExprSPtr> const &args) {
		std::vector<interval> res;
		for (auto const &name : _pf_names)
			res.push_back(_ctx.eval(*Expr::make<CallExpr>(name, args)));
		return res;
	}
};


}
#endif