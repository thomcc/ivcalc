#include "parser/parser.hh"
#include <sstream>
#include "expr.hh"
#include "parser/lexer.hh"
#include "utilities.hh"
#include <cstdio>
namespace numbers {

void
ErrorHandler::error(std::string const &msg, ELevel lvl) {
	if (_need_lines) return;
	++_errors;
	switch (lvl) {
	case ELevel::EGeneric:
		if (isatty(STDERR_FILENO)) 
			std::cerr << "[" M_BOLD("Error") "]";
		else std::cerr << "[Error]";
		break;
	case ELevel::EWarn:
		if (isatty(STDERR_FILENO)) 
			std::cerr << "[" Y_BOLD("Warn") "]";
		else std::cerr << "[Warn]";
		break;
	case ELevel::EFatal:
		if (isatty(STDERR_FILENO))
			std::cerr << "[" R_BOLD("Fatal") "]";
		else std::cerr << "[Fatal]";
		break;
	case ELevel::EInfo:
		if (isatty(STDERR_FILENO))
			std::cerr << "[" B_BOLD("Info") "]";
		else std::cerr << "[Info]";
		break;
	case ELevel::EBug:
		if (isatty(STDERR_FILENO))
			std::cerr << "[" R_BOLD("Bug") "]";
		else std::cerr << "[Bug]";
		break;
	default:
		if (isatty(STDERR_FILENO)) 
			std::cerr << "[" K_BOLD("???") "]";
		else std::cerr << "[???]";
		break;
	}
	std::cerr << msg << std::endl;
}

enum Precedence {
	P_Assign  = 1,
	P_Term    = 2,
	P_Prod    = 3,
	P_Expt    = 4,
	P_Prefix  = 5,
	P_Postfix = 6,
	P_Call    = 7     
};

Parser::Infix Parser::_infixes[] = {
	{ &Parser::call, P_Call },// T_LPAREN,
	{ NULL, -1 },// T_RPAREN,
	{ NULL, -1 },// T_COMMA,
	{ &Parser::assign, P_Assign },// T_ASSIGN,
	{ &Parser::plus, P_Term },// T_PLUS
	{ &Parser::minus, P_Term },// T_MINUS,
	{ &Parser::times, P_Prod }, // T_ASTERISK
	{ &Parser::divide, P_Prod }, // T_SLASH
	{ &Parser::expt, P_Expt }, // T_CARET
	{ NULL, -1 },// T_NAME
	{ NULL, -1 },// T_NUMBER
	{ NULL, -1 },// T_EOF,
	{ NULL, -1 },// T_ERROR,
};

Parser::Prefix Parser::_prefixes[] = {
	{ &Parser::group, -1 },// T_LPAREN,
	{ NULL, -1 },// T_RPAREN,
	{ NULL, -1 },// T_COMMA,
	{ NULL, -1 },// T_ASSIGN,
	{ &Parser::p_plus, P_Prefix },// T_PLUS
	{ &Parser::p_minus, P_Prefix },// T_MINUS,
	{ NULL, -1 },// T_ASTERISK
	{ NULL, -1 },// T_SLASH
	{ NULL, -1 },// T_CARET
	{ &Parser::var, -1 },// T_NAME
	{ &Parser::number, -1 },// T_NUMBER
	{ NULL, -1 },// T_EOF,
	{ NULL, -1 },// T_ERROR,

};

ExprSPtr
Parser::var(Token const &t) {
	return ExprSPtr(new VarExpr(t.text()));
}

ExprSPtr
Parser::number(Token const &t) {
	long double d;
	if (!sscanf(t.text().c_str(), "%Lf", &d)) {
		_on_error.error(strprintf("Invalid number: %s", t.text().c_str()));
		d = 0.0;
	}
	return ExprSPtr(new LitExpr(d));
}

ExprSPtr
Parser::call(ExprSPtr lhs, Token const &t) {
	std::vector<ExprSPtr> args;
	std::string name;
	if (VarExpr *e = lhs->as_var_expr()) {
		name = e->name();
	} else {
		_on_error.error("invalid function!");
		throw "Bad function";
	}
	if (!match(T_RPAREN)) {
		do args.push_back(parse_expression());
		while (match(T_COMMA));
		consume(T_RPAREN, "Expected ')' after call");
	}
	return ExprSPtr(new CallExpr(name, args));
}

ExprSPtr
Parser::group(Token const &t) {
	ExprSPtr e = parse_expression();
	consume(T_RPAREN, "Expected ')' in grouping");
	return e;
}

ExprSPtr
Parser::assign(ExprSPtr lhs, Token const &t) {
	ExprSPtr rhs = parse_expr(P_Assign);
	std::string name("");
	if (VarExpr *e = lhs->as_var_expr())
		name = e->name();
	else {
		_on_error.error(strprintf("'%s' is not assignable!", t.text().c_str()), ELevel::EFatal);
		throw "Bad assignment";
	}
	return ExprSPtr(new AssignExpr(name, rhs));
}

ExprSPtr 
Parser::p_plus(Token const &t) {
	return parse_expr(P_Prefix);
}

ExprSPtr 
Parser::p_minus(Token const &t) {
	ExprSPtr rhs = parse_expr(P_Prefix);
	return ExprSPtr(new NegExpr(rhs));
}


ExprSPtr
Parser::plus(ExprSPtr lhs, Token const &t) {
	ExprSPtr rhs = parse_expr(P_Term);
	return ExprSPtr(new AddExpr(lhs, rhs));
}

ExprSPtr
Parser::minus(ExprSPtr lhs, Token const &t) {
	ExprSPtr rhs = parse_expr(P_Term);
	return ExprSPtr(new SubExpr(lhs, rhs));
}

ExprSPtr
Parser::times(ExprSPtr lhs, Token const &t) {
	ExprSPtr rhs = parse_expr(P_Prod);
	return ExprSPtr(new MulExpr(lhs, rhs));
}

ExprSPtr
Parser::divide(ExprSPtr lhs, Token const &t) {
	ExprSPtr rhs = parse_expr(P_Prod);
	return ExprSPtr(new DivExpr(lhs, rhs));
}

ExprSPtr
Parser::expt(ExprSPtr lhs, Token const &t) {
	ExprSPtr rhs = parse_expr(P_Expt-1);
	int i = 0;
	if (LitExpr *e = rhs->as_lit_expr()) {
		number_type n = e->value().mid();
		if (n != std::rint(n))
			_on_error.error("Warning: truncating exponent literal", ELevel::EWarn);
		i = std::rint(n);
	} else {
		_on_error.error("Exponent must be literal. using 0", ELevel::EFatal);
	}
	return ExprSPtr(new ExptExpr(i, lhs));
}

//ExprSPtr
//Parser::parse_expression() {
//	return parse_expr();
//}
//
ExprSPtr
Parser::parse_expr(int precedence) {
	Token t = consume();
	if (look_ahead(T_EOF)) check_line();
	Prefix p = _prefixes[t.type()];
	if (!p.prefix) {
		_on_error.error(strprintf("Couldn't parse '%s'.", t.text().c_str()));
		return ExprSPtr(new EmptyExpr);
	}
	check_line();
	ExprSPtr left = (this->*p.prefix)(t);
	while (precedence < get_precedence()) {
		t = consume();
		check_line();
		Infix s2 = _infixes[t.type()];
		if (s2.infix) {
			left = (this->*s2.infix)(left, t);
		} else _on_error.error(strprintf("Couldn't parse '%s'.", t.text().c_str()));
	}
	return left;
}


bool
Parser::look_ahead(TokenType t) {
	fill_look_ahead(1);
	return _lookahead[0].type() == t;
}

bool
Parser::look_ahead(TokenType t, TokenType t2) {
	fill_look_ahead(2);
	return _lookahead[0].type() == t && _lookahead[1].type() == t2;
}

bool
Parser::match(TokenType t) {
	if (look_ahead(t)) {
		consume();
		return true;
	} else return false;
}

void
Parser::expect(TokenType expected, std::string const &error_msg) {
	if (!look_ahead(expected)) {
		_on_error.error(error_msg);
	}
}


Token
Parser::consume() {
	fill_look_ahead(1);
	return _last = _lookahead.dequeue();
}

Token
Parser::consume(TokenType expected, std::string const &msg) {
	if (look_ahead(expected)) return consume();
	_on_error.error(msg);
	// keep going, catch more errors
	return consume();
}

int
Parser::get_precedence() {
	fill_look_ahead(1);
//	if (_infixes[_lookahead[0].type()].precedence >= 0)
		return _infixes[_lookahead[0].type()].precedence;
//	return 0;
}

void 
Parser::check_line() {
	if (look_ahead(T_EOF) && !_on_error.errors()) {
		_on_error.want_lines();
	}
}



void
Parser::fill_look_ahead(size_t size) {
	while (_lookahead.count() < size) {
		Token t = _lexer.next();
		if (t.is_a(T_ERROR))
			_on_error.error(t.text());
		else _lookahead.enqueue(t);
	}
}


}