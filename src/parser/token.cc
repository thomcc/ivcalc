#include "parser/token.hh"
#include <sstream>


namespace calc {

int punct(TokenType t) {
	switch (t) {
	case T_LPAREN:   return '(';
	case T_RPAREN:   return ')';
	case T_LBRACKET: return '[';
	case T_RBRACKET: return ']';
	case T_COMMA:    return ',';
	case T_ASSIGN:   return '=';
	case T_PLUS:     return '+';
	case T_MINUS:    return '-';
	case T_ASTERISK: return '*';
	case T_SLASH:    return '/';
	case T_CARET:    return '^';
	default:         return -1;
	}
}

TokenType punct_type(char c) {
	switch (c) {
	case '(': return T_LPAREN;
	case ')': return T_RPAREN;
	case '[': return T_LBRACKET;
	case ']': return T_RBRACKET;
	case ',': return T_COMMA;
	case '=': return T_ASSIGN;
	case '-': return T_MINUS;
	case '+': return T_PLUS;
	case '*': return T_ASTERISK;
	case '/': return T_SLASH;
	case '^': return T_CARET;
	default:  return T_ERROR;
	}
}

std::string type_name(TokenType tt) {
	switch (tt) {
	case T_LPAREN:   return "T_LPAREN";
	case T_RPAREN:   return "T_RPAREN";
	case T_LBRACKET: return "T_LBRACKET";
	case T_RBRACKET: return "T_RBRACKET";
	case T_COMMA:    return "T_COMMA";
	case T_ASSIGN:   return "T_ASSIGN";
	case T_PLUS:     return "T_PLUS";
	case T_MINUS:    return "T_MINUS";
	case T_ASTERISK: return "T_ASTERISK";
	case T_SLASH:    return "T_SLASH";
	case T_CARET:    return "T_CARET";
	case T_NAME:     return "T_NAME";
	case T_NUMBER:   return "T_NUMBER";
	case T_EOF:      return "T_EOF";
	case T_ERROR:    return "T_ERROR";
	default:         return "UNKNOWN";
	}
}

std::string Token::describe() const {
	std::stringstream ss;
	ss << "<Token [" << type_name(_type) << "] '" << _text << "' (" << _start << " -> " << _end << ")>";
	return ss.str();
}

std::ostream& operator<<(std::ostream &o, Token const &token) {
	return o << token.describe();
}


}


