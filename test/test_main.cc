#include "test.hh"
#include "queue_tests.hh"
#include "lexer_tests.hh"
#include "parser_tests.hh"
#include "simplify_tests.hh"
#include "derive_tests.hh"
#include "interval_tests.hh"
int
main() {
	using namespace calc;
//	std::cout << "sizeof(Expr) = " << sizeof(Expr) << std::endl;
//	std::cout << "sizeof(AddExpr) = " << sizeof(AddExpr) << std::endl;
//	std::cout << "sizeof(SubExpr) = " << sizeof(SubExpr) << std::endl;
//	std::cout << "sizeof(MulExpr) = " << sizeof(MulExpr) << std::endl;
//	std::cout << "sizeof(DivExpr) = " << sizeof(DivExpr) << std::endl;
//	std::cout << "sizeof(NegExpr) = " << sizeof(NegExpr) << std::endl;
//	std::cout << "sizeof(VarExpr) = " << sizeof(VarExpr) << std::endl;
//	std::cout << "sizeof(CallExpr) = " << sizeof(CallExpr) << std::endl;
//	std::cout << "sizeof(LitExpr) = " << sizeof(LitExpr) << std::endl;
//	std::cout << "sizeof(ExptExpr) = " << sizeof(ExptExpr) << std::endl;
//	std::cout << "sizeof(FuncExpr) = " << sizeof(FuncExpr) << std::endl;
//	std::cout << "sizeof(EmptyExpr) = " << sizeof(EmptyExpr) << std::endl;

	QueueTest().run();
	LexerTest().run();
	ParserTest().run();
	SimplifyTest().run();
	DeriveTest().run();
	Test::summary();
	Test::reset();
	std::cout << "running interval tests..." << std::endl;
	IntervalTest().run();
	Test::summary();
	return Test::failures();
}

