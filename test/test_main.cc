#include "test.hh"
#include "queue_tests.hh"
#include "lexer_tests.hh"
#include "parser_tests.hh"


int
main() {
	using namespace calc;
	QueueTest().run();
	LexerTest().run();
	ParserTest().run();
	Test::summary();
	return Test::failures();
}

