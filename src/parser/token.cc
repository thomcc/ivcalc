#include "parser/token.hh"



namespace calc {

int
punct(TokenType t) {
	switch (t) {
	case T_LPAREN:
		return '(';
	case T_RPAREN:
		return ')';
	case T_COMMA:
		return ',';
	case T_ASSIGN:
		return '=';
	case T_PLUS:
		return '+';
	case T_MINUS:
		return '-';
	case T_ASTERISK:
		return '*';
	case T_SLASH:
		return '/';
	case T_CARET:
		return '^';
//	case T_PIPE:
//		return '|';
//	case T_TILDE:
//		return '~';
//	case T_BANG:
//		return '!';
	default:
		return -1;
	}
}

TokenType
punct_type(char c) {
	switch (c) {
	case '(':
		return T_LPAREN;
	case ')':
		return T_RPAREN;
	case ',':
		return T_COMMA;
	case '=':
		return T_ASSIGN;
	case '-':
		return T_MINUS;
	case '+':
		return T_PLUS;
	case '*':
		return T_ASTERISK;
	case '/':
		return T_SLASH;
	case '^':
		return T_CARET;
//	case '|':
//		return T_PIPE;
	default:
		return T_ERROR;
	}
}

}


