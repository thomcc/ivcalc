#ifndef __LEXER_HH__
#define __LEXER_HH__
#include "common.hh"
#include "parser/token.hh"
#include <string>

namespace calc {

class Lexer {
public:
	Lexer(std::string const &text)
	: _text(text), _index(0), _start(0) {}

	bool finished() const;
	Token next();

private:

	Token scan_ident();
	Token scan_number();

	Token error(std::string const &message);

	Token make_token(TokenType type);
	Token make_token(TokenType t, std::string const &txt);

	char peek(int ahead = 0);
	char forward();
	void forward(size_t ahead);
	std::string _text;
	int _index;
	int _start;


	DISALLOW_COPY_AND_ASSIGN(Lexer);

};


}






#endif