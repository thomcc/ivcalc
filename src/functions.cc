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

}