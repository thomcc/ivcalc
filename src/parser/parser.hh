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
namespace numbers {

enum class ELevel {
	EGeneric,
	EInfo,
	EWarn,
	EFatal,
	EDebug,
	EBug,
};

class ErrorHandler {
	bool _need_lines;
	bool _repl;
	int _errors;
public:
	ErrorHandler() : _need_lines(false), _repl(false), _errors(0) {}
	void error(std::string const &msg, ELevel lvl=ELevel::EGeneric);
	
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

};

// hand written recursive decent parser with 
// pratt-style operator precedence

class Parser {
public:

	Parser(std::string const &text, ErrorHandler &eh)
	: _lexer(text)
	, _lookahead()
	, _last()
	, _on_error(eh)
	{}

	Parser(std::string const &text)
	: _lexer(text)
	, _lookahead()
	, _last()
	, _on_error()
	{}

	ExprSPtr
	parse_expression() {
		return parse_expr();
	}


private:
	// parses an infix expression
	// led stands for "Left Denotation"
	typedef ExprSPtr (Parser::*Led)(ExprSPtr left, Token const &t);
	// parses a prefix or non-infix expression
	// Nud stands for "Null Denotation".
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

//	struct Sym {
//		Led infix;
//		Nud prefix;
//		int precedence;
//	};


	ExprSPtr parse_expr(int precedence = 0);
	int get_precedence();

	ExprSPtr var(Token const &t);
	ExprSPtr number(Token const &t);
	ExprSPtr group(Token const &t);
	ExprSPtr p_plus(Token const &t);
	ExprSPtr p_minus(Token const &t);


	ExprSPtr assign(ExprSPtr lhs, Token const &t);
	ExprSPtr plus(ExprSPtr lhs, Token const &t);
	ExprSPtr minus(ExprSPtr lhs, Token const &t);
	ExprSPtr times(ExprSPtr lhs, Token const &t);
	ExprSPtr divide(ExprSPtr lhs, Token const &t);
	ExprSPtr expt(ExprSPtr lhs, Token const &t);
	ExprSPtr call(ExprSPtr lhs, Token const &t);

	bool look_ahead(TokenType t);
	bool look_ahead(TokenType t, TokenType t2);

	bool match(TokenType t);
	void expect(TokenType expected, std::string const &error_msg);

	Token consume();
	Token consume(TokenType expected, std::string const &msg);
	void fill_look_ahead(size_t size);
	void check_line();

	Lexer _lexer;
	Queue<Token, 2> _lookahead;
	Token _last;
	ErrorHandler _on_error;

//	static Sym _syms[NUM_TOKEN_TYPES];


	DISALLOW_COPY_AND_SWAP(Parser);
};



}














#endif