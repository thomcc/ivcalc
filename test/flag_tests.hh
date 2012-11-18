#ifndef __FLAG_TESTS_HH__
#define __FLAG_TESTS_HH__
#include "test.hh"
#include "frontend/flagparser.hh"

namespace calc {
class FlagTest : public Test {
public:
	virtual void run_tests() {
		test_parse();
	}
private:
	void test_parse();

};


}
#endif