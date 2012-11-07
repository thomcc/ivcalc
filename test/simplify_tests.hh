#ifndef __SIMPLIFY_TESTS_HH__
#define __SIMPLIFY_TESTS_HH__
#include "test.hh"
#include "expr.hh"

namespace calc {
class SimplifyTest : public Test {
public:
	virtual void run_tests() {
		add_zero();
		sub_zero();
		mul_zero();
		zero_div();
		pow_zero();

		mul_one();
		div_one();
		pow_one();
		constant_fold();


	}
private:

	void add_zero();
	void sub_zero();
	void mul_zero();
	void zero_div();
	void pow_zero();
	void mul_one();
	void div_one();
	void pow_one();
	void constant_fold();

	ExprSPtr get_expr(std::string const &, bool simplify=true) const;
};


}















#endif