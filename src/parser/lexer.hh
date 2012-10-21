#ifndef __LEXER_HH__
#define __LEXER_HH__
#include "common.hh"
#include "token.hh"
#include <memory>

namespace numbers {

class Lexer {
public:
	Lexer(std::string const &file, std::string const &source)
	: _file(file)
	, _source(source)
	, _pos(0)
	, _start(0)
	, _skipline(true)
	, _start_row(1)
	, _start_col(1)
	, _cur_row(1)
	, _cur_col(1) {}
	Token get_token();
private:

	Token raw_get_token();
	bool finished() const;
	bool is_space(char c) const;
	bool is_ident_start(char c) const;
	bool is_ident(char c) const;
	bool is_digit(char c) const;
	char peek(int ahead=0) const;
	Token read_number();
	Token read_var();
	void skip_line_comment();
	void skip_block_comment();
	char forward();
	Token make_tok(TokenType t);
	Token make_tok(TokenType t, std::string text);
	Token error(std::string const &message);

	const std::string _file;
	std::string _source;
	int _pos;
	int _start;
	bool _skipline;
	int _start_row;
	int _start_col;
	int _cur_row;
	int _cur_col;
	DISALLOW_COPY_AND_SWAP(Lexer);
};


}






#endif