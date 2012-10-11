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
		T_IDENT,
		T_NUM,
		T_CEQ,
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
		const std::string _file;
		int _start_line;
		int _start_col;
		int _end_line;
		int _end_col;
	public:
		Position(const std::string &file, int sl, int sc, int el, int ec)
		: _file(file), _start_line(sl), _start_col(sc), _end_line(el), _end_col(ec) {}

		const std::string &file() const { return _file; }
		int start_line() const { return _start_line; }
		int start_col() const { return _start_col; }
		int end_line() const { return _end_line; }
		int end_col() const { return _end_col; }
		Position span_to(const Position &end) const;
	};

	std::ostream &operator<<(std::ostream &o, const Position &pos);


	class Token {
		TokenType _type;
		const std::string _text;
		Position _pos;
	public:
		Token(TokenType type, std::string text, Position pos) 
		: _type(type), _text(text), _pos(pos) {}

		static const char *type_string(TokenType t);
		
		TokenType type() const { return _type; }
		const Position &pos() const { return _pos; }
		const std::string &text() const { return _text; }
		bool is(TokenType t) const { return _type == t; }
	};

	std::ostream &operator<<(std::ostream &o, const Token &token);

}






#endif