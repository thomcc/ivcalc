#ifndef __EVAL_HH__
#define __EVAL_HH__
#include "expr.hh"
#include "visitorbase.hh"
#include "interval.hh"
#include <map>
#include <functional>

namespace calc {

class Env;

class BaseFunc {
private:
	static unsigned long gs_entropy;
protected:

	std::string _name;

	static std::string
	gensym(std::string prefix="UNNAMED") {
		return stringize() << prefix << "__" << gs_entropy++;
	}

	virtual interval apply(std::vector<interval> const &intervals, Env const &env) const = 0;

public:

	BaseFunc(std::string const &name) : _name(name) {}

	virtual ~BaseFunc() {}

	interval
	call(std::vector<interval> const &intervals, Env const &env) const {
		if (intervals.size() != arity())
			throw iv_arithmetic_error("Wrong number of args provided to "+name());
		return apply(intervals, env);
	}

	virtual unsigned arity() const = 0;

	std::string const &name() const { return _name; }
};


// could probably use templates
class Fn1Arg : public BaseFunc {
public:
	typedef std::function<interval(interval const&)> impl_function;
	Fn1Arg(std::string const &name, impl_function func)
		: BaseFunc(name), _impl(func) {}
	unsigned arity() const { return 1; }
protected:
	interval apply(std::vector<interval> const &args, Env const&) const { return _impl(args[0]); }
private:
	impl_function _impl;
};

class Fn2Args : public BaseFunc {
public:
	typedef std::function<interval(interval const&, interval const&)> impl_function;
	Fn2Args(std::string const &name, impl_function func)
		: BaseFunc(name), _impl(func) {}
	unsigned arity() const { return 2; }
protected:
	interval apply(std::vector<interval> const &args, Env const&) const { return _impl(args[0], args[1]); }
private:
	impl_function _impl;
};



class UserDefinedFn : public BaseFunc {
public:
	UserDefinedFn(std::vector<std::string> const &param_names, ExprSPtr expr)
		: BaseFunc(gensym()) , _params(param_names) , _impl(expr) {}
	UserDefinedFn(std::string const &name, std::vector<std::string> const &param_names, ExprSPtr expr)
		: BaseFunc(name) , _params(param_names) , _impl(expr) {}
	unsigned arity() const { return _params.size(); }
protected:
	interval apply(std::vector<interval> const &args, Env const &) const;
private:
	std::vector<std::string> _params;
	ExprSPtr _impl;
};



class Env {
	Env const *_parent;
	std::map<std::string, interval> _vars;
	std::map<std::string, std::shared_ptr<BaseFunc>> _funcs;
public:
	Env();
	Env(Env const &) = default;
	Env &operator=(Env const &o) = default;
	bool has(std::string const &s) const;
	bool get(std::string const &s, interval &i) const;
	interval apply(std::string const &s, std::vector<interval> const &args) const;
	void put(std::string const &s, interval e);
	void def(std::string const &name, std::vector<std::string> const &prams, ExprSPtr v);
	// was getting repetitive
	template <typename T, typename... Args>
	void add_func(Args&&... args) {
		std::shared_ptr<T> eptr = std::make_shared<T>(std::forward<Args>(args)...);
		std::shared_ptr<BaseFunc> bf = std::static_pointer_cast<BaseFunc>(eptr);
		_funcs[bf->name()] = bf;
	}

	Env extend(std::vector<std::string> const &params, std::vector<interval> const &args) const;
	static Env global();
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
, public Visitor<FuncExpr>
{
	Env _env;
	interval _res;

public:

	Evaluator() : _env(Env::global()), _res(0) {}
	Evaluator(Env &env);

	interval eval(Expr &e);
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
};

inline interval
Evaluator::eval(Expr &e)  {
	_res = 0;
	e.accept(*this);
	return _res;
}

inline void
Evaluator::visit(AddExpr &e) {
	interval lhs = eval(*e.lhs());
	interval rhs = eval(*e.rhs());
	_res = lhs + rhs;
}

inline void
Evaluator::visit(SubExpr &e) {
	interval lhs = eval(*e.lhs());
	interval rhs = eval(*e.rhs());
	_res = lhs - rhs;
}

inline void
Evaluator::visit(NegExpr &e) {
	interval vv = eval(*e.value());
	_res = -vv;
}

inline void
Evaluator::visit(MulExpr &e) {
	interval lhs = eval(*e.lhs());
	interval rhs = eval(*e.rhs());
	_res = lhs * rhs;
}

inline void
Evaluator::visit(DivExpr &e) {
	interval lhs = eval(*e.lhs());
	interval rhs = eval(*e.rhs());
	_res = lhs / rhs;
}

inline void
Evaluator::visit(VarExpr &e) {
	if (!_env.has(e.name()) || !_env.get(e.name(), _res))
		throw iv_arithmetic_error("Error: unbound variable '" + e.name() + "'");
}

inline void
Evaluator::visit(ExptExpr &e) {
	interval v = eval(*e.base());
	_res = int_pow(v, e.power());
}

inline void
Evaluator::visit(LitExpr &e) {
	_res = e.value();
}

inline void
Evaluator::visit(AssignExpr &e) {
	interval v = eval(*e.value());
	_env.put(e.name(), v);
	_res = v;
}

inline void
Evaluator::visit(CallExpr &e) {
	std::vector<interval> parms;
	for (auto const &eptr : e.args())
		parms.push_back(eval(*eptr));
	_res = _env.apply(e.name(), parms);
}

inline void
Evaluator::visit(EmptyExpr &e) {}


inline void
Evaluator::visit(FuncExpr &e) {
	_env.def(e.name(), e.params(), e.impl());
	_res = interval::empty();
}




}
#endif