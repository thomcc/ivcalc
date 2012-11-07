#ifndef __REPLACER_HH__
#define __REPLACER_HH__
#include <visitorbase.hh>
#include <expr.hh>
#include <map>

namespace calc {
// replaces a var in an expression with an expression.
// used within derivator, but seemed useful
class Replacer : ExprVisitor {
	std::map<std::string, ExprSPtr> const &_replacements;
	ExprSPtr _replaced;
public:
	Replacer(std::map<std::string, ExprSPtr> const &replacements) : _replacements(replacements) {}
	ExprSPtr replace(Expr &e) {
		_replaced = nullptr;
		e.accept(*this);
		return _replaced;
	}

	void visit(Expr &e) { assert(0); } // impossible
	void visit(AddExpr &e) {
		ExprSPtr rhs = replace(*e.rhs());
		ExprSPtr lhs = replace(*e.lhs());
		_replaced = Expr::make<AddExpr>(lhs, rhs);
	}

	void visit(SubExpr &e) {
		ExprSPtr rhs = replace(*e.rhs());
		ExprSPtr lhs = replace(*e.lhs());
		_replaced = Expr::make<SubExpr>(lhs, rhs);
	}

	void visit(NegExpr &e) {
		ExprSPtr v = replace(*e.value());
		_replaced = Expr::make<NegExpr>(v);
	}

	void visit(MulExpr &e) {
		ExprSPtr rhs = replace(*e.rhs());
		ExprSPtr lhs = replace(*e.lhs());
		_replaced = Expr::make<MulExpr>(lhs, rhs);
	}

	void visit(DivExpr &e) {
		ExprSPtr rhs = replace(*e.rhs());
		ExprSPtr lhs = replace(*e.lhs());
		_replaced = Expr::make<DivExpr>(lhs, rhs);
	}

	void visit(VarExpr &e) {
		ExprSPtr s;
		if (find(e.name(), s)) _replaced = s;
		else _replaced = Expr::make<VarExpr>(e.name());
	}

	void visit(ExptExpr &e) {
		ExprSPtr lhs = replace(*e.base());
		_replaced = Expr::make<ExptExpr>(lhs, e.power());
	}

	void visit(LitExpr &e) {
		_replaced = Expr::make<LitExpr>(e.value());
	}

	void visit(CallExpr &e) {
		std::vector<ExprSPtr> args;
		for (auto const &e : e.args()) args.push_back(replace(*e));
		_replaced = Expr::make<CallExpr>(e.name(), args);
	}

	void visit(EmptyExpr &e) { _replaced = Expr::make<EmptyExpr>(); }
	void visit(AssignExpr &e) {
		std::string name = e.name();
		ExprSPtr e2;
		if (find(e.name(), e2)) {
			if (VarExpr const *ve = e2->as_var_expr()) name = ve->name();
			else throw iv_arithmetic_error("Error: replacement variable on "
			                               "left-hand side of assignment expression "
			                               "may only be replaced with a VarExpr.");
		}
		ExprSPtr r = replace(*e.value());
		_replaced = Expr::make<AssignExpr>(name, r);
	}

	void visit(FuncExpr &e) {
		// not going to look for a name replacement.
		std::vector<std::string> nparams(e.params());
		for (auto &ee : nparams) {
			ExprSPtr res;
			if (find(ee, res)) {
				if (VarExpr const *ve = res->as_var_expr()) ee = ve->name();
				else throw iv_arithmetic_error("Error: replacement variable in function parameter list may only be replaced with a VarExpr.");
			}
		}
		ExprSPtr rimpl = replace(*e.impl());
		_replaced = Expr::make<FuncExpr>(e.name(), nparams, rimpl);
	}
private:
	bool find(std::string const &name, ExprSPtr &e) const {
		auto it = _replacements.find(name);
		if (it == _replacements.end()) return false;
		e = it->second;
		return true;
	}
};

}
#endif