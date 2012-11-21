#ifndef __SEXP_PARSER_HH__
#define __SEXP_PARSER_HH__
#include "common.hh"
#include "errors.hh"
#include "expr.hh"
#include "parser/lexer.hh"
#include "parser/queue.hh"
#include <deque>
namespace calc {

class SexpParser {
public:
	SexpParser(std::string const &text, std::unique_ptr<ErrorHandler> eh)
		: _lexer(text), _on_error(std::move(eh)) {}
	SexpParser(std::string const &text, bool at_repl=false)
		: SexpParser(text, ErrorHandler::make_cerr(at_repl)) {}
	ExprPtr parse_expression();
	int errors() const { return _on_error->errors(); }
	bool need_lines() const { return _on_error->need_lines(); }
	explicit operator bool() const { return errors() == 0; }

private:
	Lexer _lexer;
	Token _last;
	Queue<Token, 1> _lookahead;
	std::unique_ptr<ErrorHandler> _on_error;

	ExprPtr read_iv_lit();
	ExprPtr read_list();
	ExprPtr read_add();
	ExprPtr read_sub();
	ExprPtr read_mul();
	ExprPtr read_div();
	ExprPtr read_expt();
	ExprPtr read_call(std::string const &s);
	ExprPtr read_func();
	real parse_real(std::string const &s);
	Token next_token();
	std::deque<ExprPtr> read_until_lp();
	bool look_ahead(TokenType t);
	bool match(TokenType t);

	Token consume();
	Token consume(TokenType expected, std::string const &msg);
	void fill_look_ahead(size_t size);

	DISALLOW_COPY_AND_ASSIGN(SexpParser);
};



}
#endif