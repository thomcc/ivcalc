#ifndef __LEXER_TESTS_HH__
#define __LEXER_TESTS_HH__

#include "test.hh"

namespace calc {

class LexerTest : public Test {
public:
	virtual void
	run_tests() {
		number();
		tokens();
	}
private:
	void number();
	void tokens();
};





}


#endif