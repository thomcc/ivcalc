#ifndef __TOKEN_HH__
#define __TOKEN_HH__
#include <cstdio>
#include <iostream>
#include <string>

namespace numbers {
enum TokenType {
	T_LPAREN,
	T_RPAREN,
	T_LBRACKET,
	T_RBRACKET,
	T_EQ,
	T_COMMA,
	T_IDENT,
	T_NUM,
	T_CEQ,
	T_POW,
	T_PLUS,
	T_MINUS,
	T_TIMES,
	T_DIV,
	T_LINE,
	T_ERROR,
	T_EOF,
	N_TOKEN_TYPES
};

class Position {
	std::string const _file;
	int _start_line;
	int _start_col;
	int _end_line;
	int _end_col;
public:

	Position(std::string const &file, int sl, int sc, int el, int ec)
	: _file(file), _start_line(sl), _start_col(sc), _end_line(el), _end_col(ec) {}

	Position() : _file(), _start_line(-1), _start_col(-1), _end_line(-1), _end_col(-1) {}

	std::string const &
	file() const {
		return _file; 
	}

	int 
	start_line() const {
		return _start_line; 
	}

	int 
	start_col() const {
		return _start_col; 
	}

	int 
	end_line() const {
		return _end_line; 
	}

	int 
	end_col() const {
		return _end_col; 
	}

	Position 
	span_to(Position const &end) const;
};

std::ostream&
operator<<(std::ostream &o, Position const &pos);


class Token {
	TokenType _type;
	const std::string _text;
	Position _pos;
public:
	Token(TokenType type, std::string const &text, Position const &pos)
	: _type(type), _text(text), _pos(pos) {}

	Token() : _type(T_ERROR), _text(), _pos() {}

	Token(Token const &t) = default;
	Token& operator=(Token const &rhs) = default;

	static char const *type_string(TokenType t);
	
	TokenType
	type() const { 
		return _type;
	}

	Position const&
	pos() const {
		return _pos;
	}

	std::string const&
	text() const {
		return _text;
	}

	bool
	is(TokenType t) const {
		return _type == t;
	}

	bool
	is_integral() const {
		if (!is(T_NUM)) return false;
		for (char c : _text)
			if (c == '.' || c == 'e' || c == 'E')
				return false;
		return true;
	}

};

std::ostream&
operator<<(std::ostream &o, Token const &token);

}






#endif