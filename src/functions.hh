#ifndef __FUNCTIONS_HH__
#define __FUNCTIONS_HH__

#include <functional>
#include <vector>
#include <string>
#include "common.hh"
#include "utilities.hh"
#include "interval.hh"
namespace calc {
class Env;

class BaseFunc {
private:
	static unsigned long gs_entropy;
protected:
	std::string _name;
	static std::string
	gensym(std::string prefix="UNNAMED") { return stringize() << prefix << "__" << gs_entropy++; }
	virtual interval apply(std::vector<interval> const &intervals, Env const &env) const = 0;
public:
	BaseFunc(std::string const &name) : _name(name) {}
	virtual ~BaseFunc() {}
	interval call(std::vector<interval> const &intervals, Env const &env) const {
		if (intervals.size() != arity()) throw iv_arithmetic_error("Wrong number of args provided to "+name());
		return apply(intervals, env);
	}
	virtual unsigned arity() const = 0;
	std::string const &name() const { return _name; }
};


// could probably use templates
class Fn1Arg : public BaseFunc {
public:
	typedef std::function<interval(interval const&)> impl_function;
	Fn1Arg(std::string const &name, impl_function func) : BaseFunc(name), _impl(func) {}
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
}
#endif