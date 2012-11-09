#ifndef __REPLACER_HH__
#define __REPLACER_HH__
#include <map>
#include "visitors/visitorbase.hh"
#include "expr.hh"

namespace calc {
// replaces a var in an expression with an expression.
// used within derivator, but seemed useful
class Replacer : public ExprVisitor {
	std::map<std::string, ExprPtr> _replacements;
	ExprPtr _replaced;
public:
	Replacer(std::map<std::string, ExprPtr> replacements) : _replacements(std::move(replacements)), _replaced(nullptr) {}
	ExprPtr replace(Expr &e) {
		_replaced = nullptr;
		e.accept(*this);
		return std::move(_replaced);
	}

	void visit(AddExpr &e) {
		ExprPtr rhs = replace(*e.rhs());
		ExprPtr lhs = replace(*e.lhs());
		_replaced = Expr::make_add(std::move(lhs), std::move(rhs));
	}

	void visit(SubExpr &e) {
		ExprPtr rhs = replace(*e.rhs());
		ExprPtr lhs = replace(*e.lhs());
		_replaced = Expr::make_sub(std::move(lhs), std::move(rhs));
	}

	void visit(NegExpr &e) {
		ExprPtr v = replace(*e.value());
		_replaced = Expr::make_neg(std::move(v));
	}

	void visit(MulExpr &e) {
		ExprPtr rhs = replace(*e.rhs());
		ExprPtr lhs = replace(*e.lhs());
		_replaced = Expr::make_mul(std::move(lhs), std::move(rhs));
	}

	void visit(DivExpr &e) {
		ExprPtr rhs = replace(*e.rhs());
		ExprPtr lhs = replace(*e.lhs());
		_replaced = Expr::make_div(std::move(lhs), std::move(rhs));
	}

	void visit(VarExpr &e) {
		ExprPtr s = nullptr;
		if (find(e.name(), s)) _replaced = std::move(s);
		else _replaced = Expr::make_var(e.name());
	}

	void visit(ExptExpr &e) {
		_replaced = Expr::make_expt(replace(*e.base()), e.power());
	}

	void visit(LitExpr &e) {
		_replaced = Expr::make_lit(e.value());
	}

	void visit(CallExpr &e) {
		std::vector<ExprPtr> args;
		for (auto const &e : e.args()) args.push_back(replace(*e));
		_replaced = Expr::make_call(e.name(), std::move(args));
	}

	void visit(EmptyExpr &e) { _replaced = Expr::make_empty(); }

	void visit(FuncExpr &e) {
		// not going to look for a name replacement.
		std::vector<std::string> nparams(e.params());
		for (auto &ee : nparams) {
			ExprPtr res;
			if (find(ee, res)) {
				if (VarExpr const *ve = res->as_var_expr()) ee = ve->name();
				else throw iv_arithmetic_error("Error: replacement variable in function parameter list may only be replaced with a VarExpr.");
			}
		}
		ExprPtr rimpl = replace(*e.impl());
		_replaced = Expr::make_func(e.name(), nparams, std::move(rimpl));
	}
private:
	bool find(std::string const &name, ExprPtr &e) const {
		auto it = _replacements.find(name);
		if (it == _replacements.end()) return false;
		e = it->second->clone();
		return true;
	}
};

}
#endif