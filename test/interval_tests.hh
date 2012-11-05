#ifndef __INTERVAL_TESTS_HH__
#define __INTERVAL_TESTS_HH__

#include "test.hh"

namespace calc {

class IntervalTest : public Test {
public:
	virtual void run_tests() {
		add();
		neg();
		sub();
		mul();
		div();
		floats();
	}
private:
	void add();
	void neg();
	void sub();
	void mul();
	void div();
	void floats();
	template <typename T> void dofloatb();
	template <typename T> void dofloatu();
};



}


#endif