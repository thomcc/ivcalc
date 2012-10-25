#include "test.hh"
#include "queue_test.hh"
#include "lexer_tests.hh"



int
main() {
	using namespace calc;
	QueueTest().run();
	LexerTest().run();
	Test::summary();
	return Test::failures();
}

