#include "parser/parser.hh"
#include <sstream>
#include "expr.hh"
#include "parser/lexer.hh"
#include "utilities.hh"
#include <cstdio>
namespace calc {

void
ErrorHandler::error(std::string const &msg, int lvl) {
	if (_need_lines) return;
	++_errors;
	if (_silent) return;
	std::string prefix = "[", suffix = "]";
	if (isatty(STDERR_FILENO)) { // ansi escape code magic
		std::string level = stringize() << lvl;
		prefix = prefix + "\x1b[" + level + "m";
		suffix = "\x1b[39m" + suffix;
	}
	switch (lvl) {
	case E_Warn: std::cerr << prefix << "Warning" << suffix << " "; break;
	case E_Error: std::cerr << prefix << "Error" << suffix << " "; break;
	case E_Info: std::cerr << prefix << "Info" << suffix << " "; break;
	case E_Bug: 
	default: std::cerr << prefix << "Bug" << suffix << " "; break;
	}
	std::cerr << msg << std::endl;
}

enum Precedence {
	P_Assign  = 1,
	P_Term    = 2,
	P_Prod    = 3,
	P_Expt    = 4,
	P_Prefix  = 5,
	P_Call    = 7
};

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

long double
Parser::parse_real(std::string const &s) {
	long double d;
	if (!sscanf(s.c_str(), "%Lf", &d)) {
		_on_error.error("Invalid number: '"+s+"'");
		d = 0.0;
	}
	return d;
}

// `var`
ExprSPtr
Parser::var(Token const &t) {
	return ExprSPtr(new VarExpr(t.text()));
}

// `num`
ExprSPtr
Parser::number(Token const &t) {
	long double d = parse_real(t.text());
	return ExprSPtr(new LitExpr(d));
}

// `[num, num]`
ExprSPtr
Parser::ival_lit(Token const &t) {
	Token lo = consume(T_NUMBER, "Expected number in interval literal.");
	consume(T_COMMA, "Expected comma in interval literal.");
	Token hi = consume(T_NUMBER, "Expected number in interval literal.");
	long double l = Parser::parse_real(lo.text());
	long double h = Parser::parse_real(hi.text());
	interval i(l, h);
	return ExprSPtr(new LitExpr(i));
}

// `name(expr, ...)`
ExprSPtr
Parser::call(ExprSPtr lhs, Token const &t) {
	std::vector<ExprSPtr> args;
	std::string name;
	if (VarExpr const *e = lhs->as_var_expr()) name = e->name();
	else _on_error.error("invalid function!");
	if (!match(T_RPAREN)) {
		do args.push_back(parse_expression());
		while (match(T_COMMA));
		consume(T_RPAREN, "Expected ')' after call");
	}
	return ExprSPtr(new CallExpr(name, args));
}

// `(expr)`
ExprSPtr
Parser::group(Token const &t) {
	ExprSPtr e = parse_expression();
	consume(T_RPAREN, "Expected ')' in grouping");
	return e;
}

// `var = expr`
ExprSPtr
Parser::assign(ExprSPtr lhs, Token const &t) {
	ExprSPtr rhs = parse_expr(P_Assign);
	std::string name("");
	if (VarExpr const *e = lhs->as_var_expr()) name = e->name();
	else _on_error.error(strprintf("'%s' is not assignable!", t.text().c_str()));
	return ExprSPtr(new AssignExpr(name, rhs));
}

// `+expr`
ExprSPtr 
Parser::p_plus(Token const &t) {
	return parse_expr(P_Prefix);
}

// `-expr`
ExprSPtr 
Parser::p_minus(Token const &t) {
	ExprSPtr rhs = parse_expr(P_Prefix);
	return ExprSPtr(new NegExpr(rhs));
}

// todo: combine +, -, *, /

// `lhs_expr + rhs_expr`
ExprSPtr
Parser::plus(ExprSPtr lhs, Token const &t) {
	ExprSPtr rhs = parse_expr(P_Term);
	return ExprSPtr(new AddExpr(lhs, rhs));
}

// `lhs_expr - rhs_expr`
ExprSPtr
Parser::minus(ExprSPtr lhs, Token const &t) {
	ExprSPtr rhs = parse_expr(P_Term);
	return ExprSPtr(new SubExpr(lhs, rhs));
}

// `lhs_expr * rhs_expr`
ExprSPtr
Parser::times(ExprSPtr lhs, Token const &t) {
	ExprSPtr rhs = parse_expr(P_Prod);
	return ExprSPtr(new MulExpr(lhs, rhs));
}

// `lhs_expr / rhs_expr`
ExprSPtr
Parser::divide(ExprSPtr lhs, Token const &t) {
	ExprSPtr rhs = parse_expr(P_Prod);
	return ExprSPtr(new DivExpr(lhs, rhs));
}

// `lhs_expr ^ an_integer`
ExprSPtr
Parser::expt(ExprSPtr lhs, Token const &t) {
	Token tt = consume(T_NUMBER, "Expected number in exponent");
	long double d = parse_real(tt.text());
	if (d != std::rint(d)) {
		std::string s = stringize() << "Expected integer in exponent. rounding " << d << " to int.";
		_on_error.error(s, 0);
	}
	return ExprSPtr(new ExptExpr((int)std::rint(d), lhs));
}

// actually parse an expression
ExprSPtr
Parser::parse_expr(int precedence) {
	// parse a prefix expression (includes vars, numbers)
	Token t = consume();
	Prefix p = _prefixes[t.type()];
	if (!p.prefix) {
		_on_error.error(strprintf("Couldn't parse (prefix) '%s'.", t.describe().c_str()));
		return ExprSPtr(new EmptyExpr);
	}
	ExprSPtr left = (this->*p.prefix)(t);
	// if the next token's precedence is high enough,
	// we're parsing an infix expression, and what we
	// just parsed is the left hand side
	while (precedence < get_precedence()) {
		t = consume();
		Infix s2 = _infixes[t.type()];
		if (s2.infix) left = (this->*s2.infix)(left, t);
		else {
			_on_error.error(strprintf("Couldn't parse (infix) '%s'.", t.describe().c_str()));
		}
	}
	return left;
}

// check if the next token has the given type
bool
Parser::look_ahead(TokenType t) {
	fill_look_ahead(1);
	return _lookahead[0].is_a(t);
}

// consume the next token if it has the given type
bool
Parser::match(TokenType t) {
	if (look_ahead(t)) {
		consume();
		return true;
	} else return false;
}

// get the next token
Token
Parser::consume() {
	fill_look_ahead(1);
	_last = _lookahead.dequeue();
	if (_last.is_a(T_EOF)) _on_error.want_lines();
	return _last;
}

// get the next token, register an error if its not the given type.
Token
Parser::consume(TokenType expected, std::string const &msg) {
	if (look_ahead(expected)) return consume();
	_on_error.error(msg);
	return consume();
}

// get the precedence of the next token
int
Parser::get_precedence() {
	fill_look_ahead(1);
	int p = _infixes[_lookahead[0].type()].precedence;
	return (p < 0) ? 0 : p;
}

// if we're at the end of a line, tell the error handler we want more lines
void 
Parser::check_line() {
	if (look_ahead(T_EOF)) _on_error.want_lines();
}

// fill the look ahead buffer
void
Parser::fill_look_ahead(size_t size) {
	while (_lookahead.count() < size) {
		Token t = _lexer.next();
		if (t.is_a(T_ERROR)) _on_error.error(t.text());
		else _lookahead.enqueue(t);
	}
}


}