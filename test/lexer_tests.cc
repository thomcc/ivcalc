#include "lexer_tests.hh"
#include "parser/lexer.hh"

namespace calc {



void
LexerTest::number() {
#define CHECK_NUM(str) do {      \
	std::string src(str);        \
	Lexer lexer(src);            \
	Token t = lexer.next();      \
	CheckEq(T_NUMBER, t.type()); \
	CheckEq(src, t.text());      \
	Check(lexer.finished());     \
} while (0)

	CHECK_NUM("431");
	CHECK_NUM("-431");
	
	CHECK_NUM("44.2");
	CHECK_NUM("-44.4");
	
	CHECK_NUM("24e5");
	CHECK_NUM("-24e5");
	CHECK_NUM("24e+5");

	CHECK_NUM("24e-5");
	CHECK_NUM("-24e-5");
	CHECK_NUM("-24e+5");

	CHECK_NUM("1.2e4");
	CHECK_NUM("1.2e+4");
	CHECK_NUM("1.2e-4");

	CHECK_NUM("-1.2e4");
	CHECK_NUM("-1.2e+4");
	CHECK_NUM("-1.2e-4");

	CHECK_NUM("-0e4");
	CHECK_NUM("0e+0");
	CHECK_NUM("0001000");

#define CHECK_ERROR(str) do {    \
	std::string src(str);        \
	Lexer lexer(src);            \
	Token t = lexer.next();      \
	CheckEq(T_ERROR, t.type());  \
} while (0)

	CHECK_ERROR("1.2a");
	CHECK_ERROR("1e+");
	CHECK_ERROR("1e-");
	CHECK_ERROR("1e");

#undef CHECK_ERROR
#undef CHECK_NUM

}

void
LexerTest::tokens() {
	std::string src("()[]foo+3-/*^=,");
	Lexer lexer(src);
	
	Token t = lexer.next();
	CheckEq(T_LPAREN, t.type());
	
	t = lexer.next();
	CheckEq(T_RPAREN, t.type());
	
	t = lexer.next();
	CheckEq(T_LBRACKET, t.type());
	
	t = lexer.next();
	CheckEq(T_RBRACKET, t.type());
	
	t = lexer.next();
	CheckEq(T_NAME, t.type());
	
	t = lexer.next();
	CheckEq(T_PLUS, t.type());
	
	t = lexer.next();
	CheckEq(T_NUMBER, t.type());
	
	t = lexer.next();
	CheckEq(T_MINUS, t.type());

	t = lexer.next();
	CheckEq(T_SLASH, t.type());

	t = lexer.next();
	CheckEq(T_ASTERISK, t.type());

	t = lexer.next();
	CheckEq(T_CARET, t.type());

	t = lexer.next();
	CheckEq(T_ASSIGN, t.type());

	t = lexer.next();
	CheckEq(T_COMMA, t.type());

	t = lexer.next();
	CheckEq(T_EOF, t.type());

	Check(lexer.finished());

}

}


