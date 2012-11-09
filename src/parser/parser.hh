#ifndef __PARSER_HH__
#define __PARSER_HH__
#include <memory>
#include "common.hh"
#include "parser/lexer.hh"
#include "parser/queue.hh"
#include "expr.hh"
namespace calc {

enum ELevel {
	E_Error = 31,
	E_Warn = 33,
	E_Info = 32,
	E_Bug = 34,
};


class ErrorHandler {
	bool _need_lines, _repl, _silent;
	int _errors;
public:
	ErrorHandler(bool silent=false, bool repl=true) : _need_lines(false), _repl(repl), _silent(silent), _errors(0) {}

	void error(std::string const &msg, int lvl=E_Error);
	bool at_repl() const { return _repl; }
	int errors() const { return _errors; }
	bool need_lines() const { return _need_lines; }
	void want_lines() { if (_repl) _need_lines = true; }
};
std::ostream &operator<<(std::ostream &o, ErrorHandler const &e);

enum Precedence {
	P_Assign  = 1,
	P_Term    = 2,
	P_Prod    = 3,
	P_Expt    = 4,
	P_Prefix  = 5,
	P_Call    = 7
};


class Parser {
public:
	Parser(std::string const &text, ErrorHandler &eh)
		: _lexer(text), _lookahead(), _last(), _on_error(eh) {}

	ExprPtr parse_expression() { return parse_expr(); }
private:
	// parses an infix expression
	typedef ExprPtr (Parser::*Led)(ExprPtr left, Token const &t);
	// parses a prefix expression
	typedef ExprPtr (Parser::*Nud)(Token const &t);

	struct Prefix {
		Nud prefix;
		int precedence;
	};

	struct Infix {
		Led infix;
		int precedence;
	};

	static Prefix _prefixes[NUM_TOKEN_TYPES];
	static Infix _infixes[NUM_TOKEN_TYPES];

	real parse_real(std::string const &s);

	ExprPtr parse_expr(int precedence = 0);
	int get_precedence();

	ExprPtr var(Token const &t);
	ExprPtr number(Token const &t);
	ExprPtr group(Token const &t);
	ExprPtr p_plus(Token const &t);
	ExprPtr p_minus(Token const &t);
	ExprPtr ival_lit(Token const &t);

	ExprPtr assign(ExprPtr lhs, Token const &t);
	ExprPtr plus(ExprPtr lhs, Token const &t);
	ExprPtr minus(ExprPtr lhs, Token const &t);
	ExprPtr times(ExprPtr lhs, Token const &t);
	ExprPtr divide(ExprPtr lhs, Token const &t);
	ExprPtr expt(ExprPtr lhs, Token const &t);
	ExprPtr call(ExprPtr lhs, Token const &t);

	bool look_ahead(TokenType t);
	bool match(TokenType t);

	Token consume();
	Token consume(TokenType expected, std::string const &msg);
	void fill_look_ahead(size_t size);
	void check_line();

	Lexer _lexer;
	Queue<Token, 1> _lookahead;
	Token _last;
	ErrorHandler &_on_error;

	DISALLOW_COPY_AND_SWAP(Parser);
};

}

#endif