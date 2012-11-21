#include "parser/parser.hh"
#include <sstream>
#include "expr.hh"
#include "utilities.hh"
#include "visitors/printer.hh"
#include <cstdio>
#include <iostream>
#include <unistd.h>

namespace calc {

Parser::Infix Parser::_infixes[] = {
	{ &Parser::call,    P_Call   }, // T_LPAREN,
	{ NULL,             -1       }, // T_RPAREN,
	{ NULL,             -1       }, // T_LBRACKET
	{ NULL,             -1       }, // T_RBRACKET
	{ NULL,             -1       }, // T_COMMA,
	{ &Parser::assign,  P_Assign }, // T_ASSIGN,
	{ &Parser::plus,    P_Term   }, // T_PLUS
	{ &Parser::minus,   P_Term   }, // T_MINUS,
	{ &Parser::times,   P_Prod   }, // T_ASTERISK
	{ &Parser::divide,  P_Prod   }, // T_SLASH
	{ &Parser::expt,    P_Expt   }, // T_CARET
	{ NULL,             -1       }, // T_NAME
	{ NULL,             -1       }, // T_NUMBER
	{ NULL,             -1       }, // T_EOF,
	{ NULL,             -1       }, // T_ERROR,
};

Parser::Prefix Parser::_prefixes[] = {
	{ &Parser::group,    -1       }, // T_LPAREN,
	{ NULL,              -1       }, // T_RPAREN
	{ &Parser::ival_lit, -1       }, // T_LBRACKET
	{ NULL,              -1       }, // T_RBRACKET
	{ NULL,              -1       }, // T_COMMA
	{ NULL,              -1       }, // T_ASSIGN
	{ &Parser::p_plus,   P_Prefix }, // T_PLUS
	{ &Parser::p_minus,  P_Prefix }, // T_MINUS
	{ NULL,              -1       }, // T_ASTERISK
	{ NULL,              -1       }, // T_SLASH
	{ NULL,              -1       }, // T_CARET
	{ &Parser::var,      -1       }, // T_NAME
	{ &Parser::number,   -1       }, // T_NUMBER
	{ NULL,              -1       }, // T_EOF
	{ NULL,              -1       }, // T_ERROR
};

real Parser::parse_real(std::string const &s) {
	real d;
	if (!sscanf(s.c_str(), REAL_FMT, &d)) {
		_on_error->error("Invalid number: '"+s+"'");
		d = 0.0;
	}
	return d;
}

// `var`
ExprPtr Parser::var(Token const &t) { return Expr::make_var(t.text()); }

// `num`
ExprPtr Parser::number(Token const &t) { return Expr::make_lit(parse_real(t.text())); }

// `[(+|-)?num, (+|-)?num]`
// todo: support negative numbers
ExprPtr Parser::ival_lit(Token const &t) {
	if (match(T_RBRACKET)) return Expr::make_lit(interval::empty());
	bool neglo = false, neghi = false;

	if (match(T_MINUS)) neglo = true;
	else if (match(T_PLUS)) neglo = false;

	Token lo = consume(T_NUMBER, "Expected number in interval literal.");
	consume(T_COMMA, "Expected comma in interval literal.");

	if (match(T_MINUS)) neghi = true;
	else if (match(T_PLUS)) neghi = false;

	Token hi = consume(T_NUMBER, "Expected number in interval literal.");
	consume(T_RBRACKET, "Missing closing ']' in interval literal");
	real l = (neglo ? -1 : 1) * Parser::parse_real(lo.text());
	real h = (neghi ? -1 : 1) * Parser::parse_real(hi.text());

	return Expr::make_lit(l, h);
}

// `name(expr, ...)`
ExprPtr Parser::call(ExprPtr lhs, Token const &t) {
	std::vector<ExprPtr> args;
	std::string name;
	if (VarExpr const *e = lhs->as_var_expr()) name = e->name();
	else _on_error->error("invalid function!");
	if (!match(T_RPAREN)) {
		do args.push_back(parse_expression());
		while (match(T_COMMA));
		consume(T_RPAREN, "Expected ')' after call");
	}
	return Expr::make_call(name, std::move(args));
}

// `(expr)`
ExprPtr Parser::group(Token const &t) {
	ExprPtr e = parse_expression();
	consume(T_RPAREN, "Expected ')' in grouping");
	return std::move(e);
}


// `func_name(arg1, arg2, ...) = expr`
ExprPtr Parser::assign(ExprPtr lhs, Token const &t) {
	ExprPtr rhs = parse_expr(P_Assign);
	if (CallExpr const *e = lhs->as_call_expr()) {
		std::string const &name = e->name();
		std::vector<std::string> prams;
		for (auto const &a : e->args())
			if (VarExpr const *ve = a->as_var_expr()) prams.push_back(ve->name());
			else _on_error->error("Error: non-identifier in function parameter list: '" + Printer::stringify(*a) + "'.");
		return Expr::make_func(name, prams, std::move(rhs));
	}
	_on_error->error(strprintf("bad syntax: '%s'. expected `<name>(<arg>, ...) = <expr>", t.text().c_str()));
	return Expr::make_empty();
}

// `+expr`
ExprPtr Parser::p_plus(Token const &t) { return parse_expr(P_Prefix); }

// `-expr`
ExprPtr Parser::p_minus(Token const &t) { return Expr::make_neg(parse_expr(P_Prefix)); }

// todo: combine `+`, `-`, `*`, `/`?

// `lhs_expr + rhs_expr`
ExprPtr Parser::plus(ExprPtr lhs, Token const &t) {
	return Expr::make_add(std::move(lhs), parse_expr(P_Term));
}

// `lhs_expr - rhs_expr`
ExprPtr Parser::minus(ExprPtr lhs, Token const &t) {
	return Expr::make_sub(std::move(lhs), parse_expr(P_Term));
}

// `lhs_expr * rhs_expr`
ExprPtr Parser::times(ExprPtr lhs, Token const &t) {
	return Expr::make_mul(std::move(lhs), parse_expr(P_Prod));
}

// `lhs_expr / rhs_expr`
ExprPtr Parser::divide(ExprPtr lhs, Token const &t) {
	return Expr::make_div(std::move(lhs), parse_expr(P_Prod));
}

// `lhs_expr ^ (+|-)? an_integer`
// TODO: support lhs ^ arbitrary expr evaluating to integer
// and move check to runtime ?
ExprPtr Parser::expt(ExprPtr lhs, Token const &t) {
	bool neg_pow = false;
	if (match(T_MINUS)) neg_pow = true;
	else if (match(T_PLUS)) neg_pow = false;
	Token tt = consume(T_NUMBER, "Expected number in exponent");
	real d = parse_real(tt.text());
	if (d != std::rint(d)) {
		std::string s = stringize() << "Expected integer in exponent. rounding " << d << " to int.";
		_on_error->error(s);
	}
	return Expr::make_expt(std::move(lhs), (int)((neg_pow ? -1 : 1) * std::rint(d)));
}

// actually parse an expression
ExprPtr Parser::parse_expr(int precedence) {
	// parse a prefix expression (includes vars, numbers)
	Token t = consume();
	Prefix p = _prefixes[t.type()];
	if (!p.prefix) {
		_on_error->error(strprintf("Couldn't parse (prefix) '%s'.", t.describe().c_str()));
		return Expr::make_empty();
	}
	ExprPtr left = (this->*p.prefix)(t);
	// if the next token's precedence is high enough,
	// we're parsing an infix expression, and what we
	// just parsed is the left hand side
	while (precedence < get_precedence()) {
		t = consume();
		Infix s2 = _infixes[t.type()];
		if (s2.infix) left = (this->*s2.infix)(std::move(left), t);
		else _on_error->error(strprintf("Couldn't parse (infix) '%s'.", t.describe().c_str()));
	}
	return std::move(left);
}

// check if the next token has the given type
bool Parser::look_ahead(TokenType t) {
	fill_look_ahead(1);
	return _lookahead[0].is_a(t);
}

// consume the next token if it has the given type
bool Parser::match(TokenType t) {
	if (look_ahead(t)) { consume(); return true; }
	return false;
}

// get the next token
Token Parser::consume() {
	fill_look_ahead(1);
	_last = _lookahead.dequeue();
	if (_last.is_a(T_EOF)) _on_error->set_need_lines();
	return _last;
}

// get the next token, register an error if its not the given type.
Token Parser::consume(TokenType expected, std::string const &msg) {
	if (look_ahead(expected)) return consume();
	_on_error->error(msg);
	return consume();
}

// get the precedence of the next token
int Parser::get_precedence() {
	fill_look_ahead(1);
	int p = _infixes[_lookahead[0].type()].precedence;
	return (p < 0) ? 0 : p;
}

// if we're at the end of a line, tell the error handler we want more lines
void Parser::check_line() {
	if (look_ahead(T_EOF)) _on_error->set_need_lines();
}

// fill the look ahead buffer
void Parser::fill_look_ahead(size_t size) {
	while (_lookahead.count() < size) {
		Token t = _lexer.next();
		if (t.is_a(T_ERROR)) _on_error->error(t.text());
		else _lookahead.enqueue(t);
	}
}


}