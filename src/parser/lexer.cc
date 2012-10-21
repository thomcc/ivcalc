#include "lexer.hh"
#include "utilities.hh"

namespace numbers {

Token
Lexer::get_token() {
	while (true) {
		Token t = raw_get_token();
		switch (t.type()) {
		case T_LPAREN:
		case T_LBRACKET:
		case T_EQ:
		case T_PLUS:
		case T_MINUS:
		case T_DIV:
		case T_CEQ:
			_skipline = true;
			break;
		case T_LINE:
			if (_skipline) continue;
			_skipline = true;
			break;
		default:
			_skipline = false;
			break;
		}
		return t;
	}
}

Token
Lexer::raw_get_token() {
	while (true) {
		if (finished()) return make_tok(T_EOF, "");
		_start = _pos;
		_start_row = _cur_row;
		_start_col = _cur_col;
		char c = forward();

		switch (c) {
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			while (is_space(peek())) forward();
			break;
		case '(': return make_tok(T_LPAREN);
		case ')': return make_tok(T_RPAREN);
		case '[': return make_tok(T_LBRACKET);
		case ']': return make_tok(T_RBRACKET);
		case ',': return make_tok(T_COMMA);
		case ':':
			if (peek() == '=') {
				forward();
				return make_tok(T_CEQ);
			} else return error(strprintf("Unknown character '%c'", c));
		case '+': return make_tok(T_PLUS);
		case '-': 
			if (is_digit(peek()))
				return read_number();
			else return make_tok(T_MINUS);
		case '*':
			return make_tok(T_TIMES);
		case '/':
			if (peek() == '/') skip_line_comment();
			else if (peek() == '*') skip_block_comment();
			else return make_tok(T_DIV);
		case '^':
			return make_tok(T_POW);
		default:
			if (is_ident_start(c)) return read_var();
			if (is_digit(c)) return read_number();
			return error(strprintf("Unknown character '%c'.", c));
		}
	}
}

bool
Lexer::finished() const {
	return _pos == _source.size();
}

bool
Lexer::is_space(char c) const {
	return (c == ' ') || (c == '\t') || (c == '\r') || (c == '\v');
}

bool
Lexer::is_digit(char c) const {
	return ('0' <= c) && (c <= '9');
}

bool
Lexer::is_ident_start(char c) const {
	return (c == '_') || (('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z'));
}

bool
Lexer::is_ident(char c) const {
	return is_ident_start(c) || is_digit(c);
}


char
Lexer::peek(int ahead) const {
	if (_pos + ahead >= _source.size()) return '\0';
	return _source.c_str()[_pos + ahead];
}


char
Lexer::forward() {
	char c = peek();
	if (!finished()) {
		++_pos;
		if (c == '\n') {
			++_cur_row;
			_cur_col = 1;
		} else ++_cur_col;
	}
	return c;
}

void
Lexer::skip_line_comment() {
	while (!finished() && peek() != '\n') forward();
}

void
Lexer::skip_block_comment() {
	forward();
	forward();
	int nest = 1;
	while (nest > 0) {
		if (finished()) return;
		if ((peek() == '/') && peek(1) == '*') {
			forward(); forward(); ++nest;
		} else if ((peek() == '*') && peek(1) == '/') {
			forward(); forward(); --nest;
		} else forward();
	}
}

Token
Lexer::make_tok(TokenType t) {
	return make_tok(t, _source.substr(_start, _pos - _start));
}

Token
Lexer::make_tok(TokenType t, std::string s) {
	Position pos(_file, _start_row, _start_col, _cur_row, _cur_col);
	return Token(t, s, pos);
}

Token
Lexer::error(std::string const &msg) {
	return make_tok(T_ERROR, msg);
}


Token
Lexer::read_number() {
	while (is_digit(peek())) forward();
	if (peek() == '.') {
		// decimal part
		do forward();
		while (is_digit(peek()));
	}
	if (peek() == 'e' || peek() == 'E') {
		forward();
		if (peek() == '+' || peek() == '-')
			forward();
		if (!is_digit(peek()))
			return error("Bad exponent");
		while (is_digit(peek())) forward();
	}
	if (is_ident(peek())) 
		return error("bad number");
	return make_tok(T_NUM);
}

Token
Lexer::read_var() {
	while (is_ident(peek())) forward();
	return make_tok(T_IDENT);
}



}





