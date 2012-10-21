#ifndef __PARSER_HH__
#define __PARSER_HH__
#include <memory>
#include "common.hh"
#include "token.hh"
#include "expr.hh"
#include "lexer.hh"
#include "queue.hh"

// pretty simple recursive decent parser using Pratt operator precedence// pretty simple recursive decent parser using Pratt operator precedence

namespace numbers {


class ErrorLog;
class Lexer;

class Parser {
public:
	Parser(std::string const &filename, std::string &src, ErrorLog &elog)
	: _logger(elog)
	, _lexer(filename, src)
	, _read()
	, _last() 
	{}

	ExprSPtr parse_expr();



private:

	typedef ExprSPtr(Parser::*PrefixParseCB)(Token &t);
	typedef ExprSPtr(Parser::*InfixParseCB)(ExprSPtr lhs, Token &t);

	struct ParseInfo {
		PrefixParseCB prefix;
		InfixParseCB infix;
		int precedence;
	};

	ExprSPtr parse_precedence(int precedence = 0);

	ExprSPtr group(Token &t);
	ExprSPtr name(Token &t);
	ExprSPtr number_lit(Token &t);
	ExprSPtr interval_lit(Token &t);

	ExprSPtr decl(ExprSPtr lhs, Token &t);
	ExprSPtr plus(ExprSPtr lhs, Token &t);
	ExprSPtr minus(ExprSPtr lhs, Token &t);
	ExprSPtr times(ExprSPtr lhs, Token &t);
	ExprSPtr divide(ExprSPtr lhs, Token &t);
	ExprSPtr pow(ExprSPtr lhs, Token &t);


	Token const &current();

	Token const&
	last() const {
		return _last;
	}

	bool look_ahead(TokenType t);
	bool look_ahead(TokenType c, TokenType n);


	void check_line();

	void fill_look_ahead(int count);

	bool match(TokenType type);

	void expect(TokenType expected, std::string const &errormsg);
	
	Token &consume();
	
	Token &consume(TokenType e, std::string const &errormsg);



	static ParseInfo _parsers[N_TOKEN_TYPES];



	ErrorLog &_logger;
	Lexer _lexer;
	Queue<Token, 2> _read;
	Token _last;

	DISALLOW_COPY_AND_SWAP(Parser);
};



}














#endif