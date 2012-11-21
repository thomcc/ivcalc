#include "parser/sexp_parser.hh"
#include "visitors/simplifier.hh"
namespace calc {
using namespace std;

static inline bool is_kw(string const &t) {
	return (t == "func" || t == "add" || t == "sub" || t == "mul" || t == "div" || t == "expt");
}

real SexpParser::parse_real(std::string const &s) {
	real d;
	rmath::set_rnear(); // bug in sscanf?
	if (!sscanf(s.c_str(), REAL_FMT, &d)) {
		_on_error->error("Invalid number: '"+s+"'");
		d = 0.0;
	}
	return d;
}

ExprPtr SexpParser::read_iv_lit() {
	ExprPtr lo = parse_expression();
	ExprPtr hi = parse_expression();
	if (!lo || !hi) return Expr::make_empty();
	Simplifier s;
	lo = s.simplify(*lo);
	hi = s.simplify(*hi);
	real lr = 0, hr = 0;
	if (LitExpr const *le = lo->as_lit_expr()) {
		if (!le->value().is_singleton()) {
			_on_error->error(stringize() << "Expected singleton interval as lower bound in interval literal. got: " << le->value());
			return Expr::make_empty();
		} else lr = le->value().lo();
	} else {
		_on_error->error("Expected constant expression in lower bound of interval literal.");
		return Expr::make_empty();
	}

	if (LitExpr const *he = hi->as_lit_expr()) {
		if (!he->value().is_singleton()) {
			_on_error->error(stringize() << "Expected singleton interval as upper bound in interval literal. got: " << he->value());
			return Expr::make_empty();
		} else hr = he->value().lo();
	} else {
		_on_error->error("Expected constant expression in upper bound of interval literal.");
		return Expr::make_empty();
	}

	if (lr > hr) {
		_on_error->error("Inverted interval literal", E_Warn);
		real t = lr;
		lr = hr;
		hr = t;
	}
	consume(T_RBRACKET, "Missing ']' after interval literal");
	return Expr::make_lit(lr, hr);
}

deque<ExprPtr> SexpParser::read_until_lp() {
	deque<ExprPtr> ve;
	while (!match(T_RPAREN))
		ve.push_back(parse_expression());
	return std::move(ve);
}
// todo: factor out repetition.
ExprPtr SexpParser::read_add() {
	deque<ExprPtr> ve = read_until_lp();
	if (ve.size() == 0) {
		_on_error->error("Missing arguments to add expr.");
		return Expr::make_lit(0);
	} else if (ve.size() == 1) return std::move(ve[0]);
	ExprPtr res = move(ve.front());
	ve.pop_front();
	do {
		res = Expr::make_add(move(res), move(ve.front()));
		ve.pop_front();
	} while (ve.size());
	return std::move(res);
}

ExprPtr SexpParser::read_sub() {
	deque<ExprPtr> ve = read_until_lp();
	if (ve.size() == 0) {
		_on_error->error("Missing arguments to sub/neg expr.");
		// implicitly treat as identity
		return Expr::make_lit(0);
	} else if (ve.size() == 1) return Expr::make_neg(std::move(ve[0]));
	ExprPtr res = move(ve.front());
	ve.pop_front();
	do {
		res = Expr::make_sub(std::move(res), move(ve.front()));
		ve.pop_front();
	} while (ve.size());
	return std::move(res);
}

ExprPtr SexpParser::read_mul() {
	deque<ExprPtr> ve = read_until_lp();
	if (ve.size() == 0) {
		_on_error->error("Missing arguments to mul expr.");
		return Expr::make_lit(1); // identity
	} else if (ve.size() == 1) return std::move(ve[0]);
	ExprPtr res = move(ve.front());
	ve.pop_front();
	do {
		res = Expr::make_mul(std::move(res), move(ve.front()));
		ve.pop_front();
	} while (ve.size());
	return std::move(res);
}

ExprPtr SexpParser::read_div() {
	deque<ExprPtr> ve = read_until_lp();
	if (ve.size() == 0) {
		_on_error->error("Missing arguments to div expr.");
		return Expr::make_lit(1); // identity
	} else if (ve.size() == 1) return Expr::make_div(Expr::make_lit(1), std::move(ve[0])); // invert
	ExprPtr res = move(ve.front());
	ve.pop_front();
	do {
		res = Expr::make_div(std::move(res), move(ve.front()));
		ve.pop_front();
	} while (ve.size());
	return std::move(res);
}

ExprPtr SexpParser::read_expt() {
	deque<ExprPtr> ve = read_until_lp();
	if (ve.size() != 2) {
		_on_error->error("Bad arguments to expt expr.");
		return Expr::make_empty();
	}
	int power = 0;
	if (LitExpr const *le = ve[1]->as_lit_expr()) {
		if (!le->value().is_singleton()) {
			_on_error->error(stringize() << "Bad power in expt expression. got: " << le->value());
			return Expr::make_empty();
		} else {
			if (le->value().lo() != std::rint(le->value().lo()))
				_on_error->error(stringize() << "Rounding exponent from " << le->value().lo() << " to " << std::rint(le->value().lo()));
			power = std::rint(le->value().lo());
		}
	} else {
		_on_error->error("Expected constant integer as power of interval.");
		return Expr::make_empty();
	}
	return Expr::make_expt(std::move(ve[0]), power);
}

ExprPtr SexpParser::read_call(string const &s) {
	deque<ExprPtr> de = read_until_lp();
	vector<ExprPtr> ve;
	for (auto &e : de) ve.push_back(move(e));
	return Expr::make_call(s, move(ve));
}

ExprPtr SexpParser::read_func() {
	ExprPtr lhs = parse_expression();
	ExprPtr rhs = parse_expression();
	if (CallExpr *e = lhs->as_call_expr()) {
		std::string const &name = e->name();
		std::vector<std::string> prams;
		for (auto const &a : e->args())
			if (VarExpr const *ve = a->as_var_expr()) prams.push_back(ve->name());
			else _on_error->error("non-identifier in function parameter list.");
		consume(T_RPAREN, "Missing closing paren after FuncExpr.");
		return Expr::make_func(name, prams, std::move(rhs));
	} else _on_error->error("Bad func expression.");
	return Expr::make_empty();
}


ExprPtr SexpParser::read_list() {
	Token t = consume();
	if (t.is_a(T_EOF)) _on_error->set_need_lines();
	if (t.is_a(T_PLUS) || t.text() == "add") return read_add();
	if (t.is_a(T_MINUS) || t.text() == "sub") return read_sub();
	if (t.is_a(T_SLASH) || t.text() == "div") return read_div();
	if (t.is_a(T_ASTERISK) || t.text() == "mul") return read_mul();
	if (t.is_a(T_CARET) || t.text() == "expt") return read_expt();
	if (t.is_a(T_ASSIGN) || t.text() == "func") return read_func();
	return read_call(t.text());
}

ExprPtr SexpParser::parse_expression() {
	for (;;) {
		Token t = consume();
		switch (t.type()) {
		case T_COMMA: // treat comma as whitespace
			continue;
		case T_CARET: case T_PLUS:   case T_MINUS:
		case T_SLASH: case T_ASSIGN: case T_ASTERISK:
			_on_error->error("Expected List, Identifier, or Number. got \""+t.text()+"\".");
			return Expr::make_empty();
		case T_NAME:
			if (is_kw(t.text())) {
				_on_error->error("Expected List, Identifier, or Number. got \""+t.text()+"\".");
				return Expr::make_empty();
			}
			return Expr::make_var(t.text());
		case T_NUMBER:
			return Expr::make_lit(parse_real(t.text()));
		case T_RBRACKET: case T_RPAREN:
			_on_error->error("Unexpected '"+t.text()+"'");
			return Expr::make_empty();
		case T_LBRACKET: return read_iv_lit();
		case T_LPAREN: return read_list();
		case T_EOF:
			_on_error->set_need_lines();
			return Expr::make_empty();
		case T_ERROR:
			_on_error->error(t.text());
			return Expr::make_empty();
		default:
			_on_error->error("Unknown token: '"+t.text()+"'");
			return Expr::make_empty();
		}
	}
}

bool SexpParser::look_ahead(TokenType t) {
	fill_look_ahead(1);
	return _lookahead[0].is_a(t);
}

bool SexpParser::match(TokenType t) {
	if (look_ahead(t)) { consume(); return true; }
	return false;
}

Token SexpParser::consume() {
	fill_look_ahead(1);
	_last = _lookahead.dequeue();
	if (_last.is_a(T_EOF)) _on_error->set_need_lines();
	return _last;
}

Token SexpParser::consume(TokenType expected, std::string const &msg) {
	if (look_ahead(expected)) return consume();
	_on_error->error(msg);
	return consume();
}

void SexpParser::fill_look_ahead(size_t size) {
	while (_lookahead.count() < size) {
		Token t = _lexer.next();
		if (t.is_a(T_ERROR)) _on_error->error(t.text());
		else _lookahead.enqueue(t);
	}
}








}
