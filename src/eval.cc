#include "eval.hh"
namespace calc {

typedef interval (*fn1)(interval const&);
typedef interval (*fn2)(interval const&,interval const&);
//typedef interval (*fn3)(interval,interval,interval);

struct func {
	std::string name;
	int argc;
	union {
		fn1 fn1;
		fn2 fn2;
//		fn3 fn3;
	};
};

static const func funcs[] = {
#define F1(NAME) {#NAME, 1, {.fn1 = NAME}}
	F1(sin),
	F1(cos),
	F1(tan),
	F1(exp),
	F1(log),
#undef F1
};


interval
Env::apply_function(std::string const &name, std::vector<interval> const &args) {
	for (func f : funcs) {
		if (name == f.name) {
			if (f.argc != args.size())
				throw "wrong number of arguments!";
			switch (f.argc) {
			case 1: 
				return f.fn1(args[0]);
			case 2:
				return f.fn2(args[0], args[1]);
			}
		}
	}
	throw "Unknown function: "+name;
}




}