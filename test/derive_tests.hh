#ifndef __DERIVE_TESTS_HH__
#define __DERIVE_TESTS_HH__

#include "test.hh"
#include "expr.hh"

namespace calc {
class DeriveTest : public Test {
public:
	virtual void run_tests() {
		var();
		expt();
		lit();
		plus();
		minus();
		mul();
		div();
		partials();
	}
private:
	void var();
	void expt();
	void lit();
	void plus();
	void minus();
	void mul();
	void div();
	void partials();
	ExprSPtr get_expr(std::string const &) const;
};


}


















#endif