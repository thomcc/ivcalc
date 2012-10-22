#include "parser/lexer.hh"
#include "utilities.hh"

namespace numbers {



static bool
is_punct(char c) {
	switch (c) {
	case '(': case ')': case ',': case '=':
	case '+': case '-':	case '*': case '/':
	case '^': case '|':
		return true;
	default:
		return false;
	}
}

static inline bool
starts_var(char c) {
	if (('a' <= c) && (c <= 'z')) return true;
	if (('A' <= c) && (c <= 'Z')) return true;
	if ((c == '_') || (c == '$')) return true;
	return false;
}

static inline bool
is_number(char c) {
	return ('0' <= c) && (c <= '9');
}

static inline bool
is_var(char c) {
	return starts_var(c) || is_number(c);
}

static inline bool
is_wspace(char c) {
	return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r') || (c == '\v');
}


bool
Lexer::finished() const {
	return _index >= _text.size();
}

Token 
Lexer::next() {
	while (!finished()) {
		_start = _index;
		char c = forward();
		if (is_punct(c))
			return make_token(punct_type(c));
		if (starts_var(c))
			return scan_var();
		if (is_number(c))
			return scan_number();
		if (!is_wspace(c))
			return error(strprintf("Unknown character '%c'", c));
	}
	// keep returning EOF so the parser can fill it's lookahead.
	return make_token(T_EOF);
}

Token
Lexer::scan_var() {
	while (is_var(peek())) forward();
	return make_token(T_NAME);
}

Token
Lexer::scan_number() {
	while (is_number(peek()))
		forward();
	if (peek() == '.') {
		// read decimal
		do forward();
		while (is_number(peek()));
	}
	if ((peek() == 'e') || (peek() == 'E')) {
		// read exponent
		forward();
		if ((peek() == '+') || (peek() == '-')) {
			// read optional sign
			forward();
		}
		if (!is_number(peek()))
			return error("Bad exponent");
		while (is_number(peek()))
			forward();
	}
	// check that there isn't a suffix on the number
	if (is_var(peek()))
		return error("Bad number");
	return make_token(T_NUMBER);
}

Token
Lexer::error(std::string const &message) {
	return make_token(T_ERROR, message);
}

char
Lexer::forward() {
	char c = peek();
	if (!finished()) ++_index;
	return c;
}

void
Lexer::forward(size_t ahead) {
	while (ahead--) forward();
}

Token
Lexer::make_token(TokenType type) {
	return make_token(type, _text.substr(_start, _index-_start));
}

Token
Lexer::make_token(TokenType t, std::string const &txt) {
	return Token(t, txt, _start, _index);
}


char
Lexer::peek(int ahead) {
	if (ahead + _index >= _text.size()) return '\0';
	return _text[_index + ahead];
}




Pos
Lexer::get_pos(int index) {
	int line = 1, col = 1;
	if (index < _text.size() && index > 0) {
		for (char c : _text) {
			if (--index <= 0) {
				struct Pos p = {line, col};
				return p;
			}
			if (c == '\n') {
				++line;
				col = 1;
			} else ++col;
		}
	}
	struct Pos p = {-1, -1};
	return p;
}





}





