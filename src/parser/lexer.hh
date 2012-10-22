#ifndef __LEXER_HH__
#define __LEXER_HH__
#include "common.hh"
#include "parser/token.hh"
#include <memory>

namespace calc {

struct Pos {
	int line, column;
};

class Lexer {
public:
	Lexer(std::string const &text)
	: _text(text), _index(0), _start(0) {}

	bool finished() const;
	Token next();

	Pos get_pos(int index);

private:
	
	Token scan_var();
	Token scan_number();
//	Token scan_operator();
	Token error(std::string const &message);

	Token make_token(TokenType type);
	Token make_token(TokenType t, std::string const &txt);

	char peek(int ahead = 0);
	char forward();
	void forward(size_t ahead);
	std::string _text;
	int _index;
	int _start;
	

	DISALLOW_COPY_AND_SWAP(Lexer);

};


}






#endif