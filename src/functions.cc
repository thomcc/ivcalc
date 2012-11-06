#include "functions.hh"
#include "eval.hh"
namespace calc {
using namespace std;

unsigned long BaseFunc::gs_entropy = 0;
interval UserDefinedFn::apply(vector<interval> const &args, Env const &e) const {
	Env env = (e.extend(_params, args));
	Evaluator eval(env);
	return eval.eval(*_impl);
}


UserDefinedFn::UserDefinedFn(std::vector<std::string> const &param_names, ExprSPtr expr)
	: BaseFunc(gensym()) , _params(param_names) , _impl(expr) {}

UserDefinedFn::UserDefinedFn(std::string const &name, std::vector<std::string> const &param_names, ExprSPtr expr)
	: BaseFunc(name) , _params(param_names) , _impl(expr) {}



}