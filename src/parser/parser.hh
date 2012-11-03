#ifndef __PARSER_HH__
#define __PARSER_HH__
#include <memory>
#include <unistd.h>
#include "common.hh"
#include "parser/token.hh"
#include "expr.hh"
#include "parser/lexer.hh"
#include "parser/queue.hh"
#include "colors.hh"

namespace calc {

enum ELevel {
	E_Error = 31,
	E_Warn = 33,
	E_Info = 32,
	E_Bug = 34,
};

class ErrorHandler {

	bool _need_lines;
	bool _repl;
	bool _silent;
	int _errors;

public:

	ErrorHandler(bool silent=false, bool repl=true)
	: _need_lines(false)
	, _repl(repl)
	, _silent(silent)
	, _errors(0)
	{}

	void error(std::string const &msg, int lvl=E_Error);

	bool
	at_repl() const {
		return _repl;
	}

	int
	errors() const {
		return _errors;
	}

	bool
	need_lines() const {
		return _need_lines;
	}

	void
	want_lines() {
		if (_repl)
			_need_lines = true;
	}

	friend std::ostream &operator<<(std::ostream &o, ErrorHandler const &e) {
		o << "<EH: repl=" << e.at_repl() << " need_lines?=";
		return o << e.need_lines() << " errors=" << e.errors() << ">";
	}
};

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
	: _lexer(text)
	, _lookahead()
	, _last()
	, _on_error(eh)
	{}

	ExprSPtr
	parse_expression() {
		return parse_expr();
	}


private:
	// parses an infix expression
	typedef ExprSPtr (Parser::*Led)(ExprSPtr left, Token const &t);
	// parses a prefix expression
	typedef ExprSPtr (Parser::*Nud)(Token const &t);

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

	long double
	parse_real(std::string const &s);

	ExprSPtr parse_expr(int precedence = 0);
	int get_precedence();

	ExprSPtr var(Token const &t);
	ExprSPtr number(Token const &t);
	ExprSPtr group(Token const &t);
	ExprSPtr p_plus(Token const &t);
	ExprSPtr p_minus(Token const &t);
	ExprSPtr ival_lit(Token const &t);

	ExprSPtr assign(ExprSPtr lhs, Token const &t);
	ExprSPtr plus(ExprSPtr lhs, Token const &t);
	ExprSPtr minus(ExprSPtr lhs, Token const &t);
	ExprSPtr times(ExprSPtr lhs, Token const &t);
	ExprSPtr divide(ExprSPtr lhs, Token const &t);
	ExprSPtr expt(ExprSPtr lhs, Token const &t);
	ExprSPtr call(ExprSPtr lhs, Token const &t);

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

//	static Sym _syms[NUM_TOKEN_TYPES];


	DISALLOW_COPY_AND_SWAP(Parser);
};

}

#endif