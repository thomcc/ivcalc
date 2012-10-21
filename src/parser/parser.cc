#include "parser.hh"
#include <sstream>
#include "expr.hh"
#include "lexer.hh"
#include "errorlog.hh"

namespace numbers {

enum precedence {
	P_ASSIGN = 1,
	P_PLUS_MINUS = 2,
	P_TIMES_DIV = 3,
	P_NEGATE = 4,
	P_MAX = 5,
};

Parser::ParseInfo Parser::_parsers[] {
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
	{ NULL, NULL, -1 },
};




ExprSPtr
parse_expr() {

}


ExprSPtr
Parser::parse_precedence(int precedence) {
	Token token = consume();
	PrefixParseCB p = _parsers[token.type()].prefix;
	if (p == NULL) {

	}
}

ExprSPtr
Parser::name(Token &t) {

}

ExprSPtr
Parser::number_lit(Token &t) {

}

ExprSPtr
Parser::interval_lit(Token &t) {

}



ExprSPtr
Parser::decl(ExprSPtr lhs, Token &t) {

}

ExprSPtr
Parser::plus(ExprSPtr lhs, Token &t) {

}

ExprSPtr
Parser::minus(ExprSPtr lhs, Token &t) {

}

ExprSPtr
Parser::times(ExprSPtr lhs, Token &t) {

}

ExprSPtr
Parser::divide(ExprSPtr lhs, Token &t) {

}

ExprSPtr
Parser::pow(ExprSPtr lhs, Token &t) {

}

const Token&
Parser::current() {
	fill_look_ahead(1);
	return _read[0];
}


bool
Parser::match(TokenType type) {
	if (look_ahead(type)) {
		consume();
		return true;
	} else return false;
}

void
Parser::expect(TokenType expected, std::string const &errormsg) {
	if (!look_ahead(expected)) _logger.error(current().pos(), errormsg);
}

Token&
Parser::consume() {
	fill_look_ahead(1);
	_last = _read.dequeue();
	return _last;
}

Token&
Parser::consume(TokenType e, std::string const &errormsg) {
	if (!look_ahead(e)) {
		if (e == T_LINE) check_line();
		_logger.error(current().pos(), errormsg);
	}
	return consume();
}

bool
Parser::look_ahead(TokenType t) {
	fill_look_ahead(1);
	return _read[0].type() == t;
}

bool
Parser::look_ahead(TokenType c, TokenType n) {
	fill_look_ahead(2);
	return _read[0].is(c) && _read[1].is(n);
}

void
Parser::check_line() {
	if (look_ahead(T_EOF) && !_logger.nerrors())
		_logger.want_lines();
}

void
Parser::fill_look_ahead(int count) {
	while (_read.count() < count) {
		Token token = _lexer.get_token();
		if (token.is(T_ERROR))
			_logger.error(token.pos(), token.text().c_str());
		else _read.enqueue(token);
	}
}



}