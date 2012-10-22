#ifndef __TOKEN_HH__
#define __TOKEN_HH__
#include <cstdio>
#include <iostream>
#include <string>

namespace calc {

// Must be sequential and have no holes
enum TokenType {
	T_LPAREN = 0,
	T_RPAREN,
	T_COMMA,
	T_ASSIGN,
	T_PLUS,
	T_MINUS,
	T_ASTERISK,
	T_SLASH,
	T_CARET,
//	T_PIPE,
//	T_TILDE,
//	T_BANG,
	T_NAME,
	T_NUMBER,
	T_EOF,
	T_ERROR,
	NUM_TOKEN_TYPES
};

class Token {
	
	TokenType _type;
	std::string _text;
	int _start, _end;
public:
	Token(TokenType t, std::string const &s, int start, int end) 
	: _type(t)
	, _text(s)
	, _start(start)
	, _end(end)
	{}

	Token() 
	: _type(T_ERROR)
	, _text("")
	, _start(-1)
	, _end(-1)
	{}

	TokenType
	type() const {
		return _type;
	}

	std::string const&
	text() const {
		return _text;
	}

	bool
	is_a(TokenType tt) const {
		return _type == tt;
	}

};

inline std::ostream&
operator<<(std::ostream &o, Token const &token) {
	return o << token.text();
}

int
punct(TokenType t);

TokenType
punct_type(char c);


}






#endif