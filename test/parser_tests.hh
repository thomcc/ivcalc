#ifndef __PARSER_TESTS_HH__
#define __PARSER_TESTS_HH__

#include "test.hh"

namespace calc {

class ParserTest : public Test {
public:
	virtual void
	run_tests() {
		add();
		sub();
		mul();
		div();
		pow();
		lit();
		assign();
		group();
		precedence();
	}
private:
	void add();
	void sub();
	void mul();
	void div();
	void pow();
	void lit();
	void assign();
	void group();
	void call();
	void precedence();
};





}


#endif