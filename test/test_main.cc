#include "test.hh"
#include "queue_tests.hh"
#include "lexer_tests.hh"
#include "parser_tests.hh"
#include "simplify_tests.hh"
#include "derive_tests.hh"

int
main() {
	using namespace calc;
	QueueTest().run();
	LexerTest().run();
	ParserTest().run();
	SimplifyTest().run();
	DeriveTest().run();

	Test::summary();
	return Test::failures();
}

