#include "parser/lexer.hh"
#include "utilities.hh"

namespace calc {
/*


static bool
is_punct(char c) {
	switch (c) {
	case '(': case ')': case ',': case '=':
	case '+': case '-':	case '*': case '/':
	case '^': case '|': case '[': case ']':
		return true;
	default:
		return false;
	}
}
*/
static inline bool
starts_ident(char c) {
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
is_ident(char c) {
	return starts_ident(c) || is_number(c);
}

static inline bool
is_wspace(char c) {
	return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}


bool
Lexer::finished() const {
	return _index >= _text.size();
}

Token 
Lexer::next() {
	for (;;) {
		if (finished()) return make_token(T_EOF, "");
		_start = _index;
		char c = forward();
		switch (c) {
		case ' ': case '\t': case '\r': case '\n':
			while (is_wspace(peek())) forward();
			break;
		case '(': return make_token(T_LPAREN);
		case ')': return make_token(T_RPAREN);
		case '[': return make_token(T_LBRACKET);
		case ']': return make_token(T_RBRACKET);
		case '=': return make_token(T_ASSIGN);
		case ',': return make_token(T_COMMA);
		case '+': return make_token(T_PLUS);
		case '-':
			if (is_number(peek())) return scan_number();
			return make_token(T_MINUS);
		case '*': return make_token(T_ASTERISK);
		case '/': return make_token(T_SLASH);
		case '^': return make_token(T_CARET);
		case '.': 
			if (is_number(peek())) return scan_number();
			return error("Expected digit after decimal.");
		default:
			if (starts_ident(c)) return scan_ident();
			if (is_number(c)) return scan_number();
			return error(std::string("Unknown character '")+c+"'");
		}
	}
}

Token
Lexer::scan_ident() {
	while (is_ident(peek())) forward();
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
	if (is_ident(peek()))
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

}





