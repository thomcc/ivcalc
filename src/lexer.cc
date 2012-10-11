#include "lexer.hh"



namespace numbers {

	Token Lexer::get_token() {
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

	

}





