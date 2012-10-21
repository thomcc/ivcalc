#include "token.hh"



namespace numbers {

char const*
Token::type_string(TokenType t) {
	switch (t) {
	case T_LPAREN: 
		return "(";
	case T_RPAREN: 
		return ")";
	case T_LBRACKET:
		return "[";
	case T_RBRACKET:
		return "]";
	case T_COMMA:
		return ",";
	case T_EQ: 
		return "=";
	case T_IDENT: 
		return "variable";
	case T_NUM: 
		return "number";
	case T_CEQ: 
		return ":=";
	case T_POW:
		return "^";
	case T_PLUS: 
		return "+";
	case T_MINUS: 
		return "-";
	case T_TIMES: 
		return "*";
	case T_DIV: 
		return "/";
	case T_LINE: 
		return "line";
	case T_EOF: 
		return "eof";
	case T_ERROR: 
		return "error";
	default: 
		return "<unknown-token-type>";
	}
}
std::ostream &
operator<<(std::ostream &o, Token const &token) {
	switch (token.type()) {
	case T_IDENT:
	case T_NUM:
	case T_ERROR:
		o << token.text();
		break;
	default:
		o << Token::type_string(token.type());
		break;
	}
	return o;
}

Position
Position::span_to(Position const &end) const {
	return Position(_file, _start_line, _start_col, end._end_line, end._end_col);
}


std::ostream &
operator<<(std::ostream &o, Position const &pos) {
	o << "#<position: " << pos.file();
	o << ":: lines(" << pos.start_line() << " - " << pos.end_line() << ")";
	o << ", cols(" << pos.start_col() << " - " << pos.end_col() << ")>";
	return o;
}


}


