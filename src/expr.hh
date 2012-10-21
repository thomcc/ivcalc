#ifndef __EXPR_HH__
#define __EXPR_HH__
#include <vector>
#include <memory>
#include "common.hh"
#include "interval.hh"
#include "token.hh"
#include "visitorbase.hh"

namespace numbers {
class Expr;
class AddExpr;
class SubExpr;
class NegExpr;
class MulExpr;
class DivExpr;
class VarExpr;
class ExptExpr;
class LitExpr;

typedef std::shared_ptr<Expr> ExprSPtr;

class Expr : public BaseVisitable<> {
	Position _pos;
public:
	
	Expr(Position const &pos) : _pos(pos) {}

	Position const&
	pos() const {
		return _pos;
	}

	virtual ~Expr() {}

	virtual AddExpr*
	to_add_expr() { 
		return NULL; 
	}

	virtual SubExpr*
	to_sub_expr() { 
		return NULL; 
	}

	virtual NegExpr*
	to_neg_expr() { 
		return NULL; 
	}

	virtual MulExpr*
	to_mul_expr() { 
		return NULL; 
	}

	virtual DivExpr*
	to_div_expr() { 
		return NULL; 
	}

	virtual VarExpr*
	to_var_expr() { 
		return NULL; 
	}

	virtual ExptExpr*
	to_expt_expr() { 
		return NULL; 
	}

	virtual LitExpr*
	to_lit_expr() {
		return NULL;
	}

	virtual bool
	is_constant() const {
		return false;
	}

	VISITABLE()
};

class AddExpr : public Expr {
	ExprSPtr _lhs, _rhs;
public:
	AddExpr(Position const &pos, ExprSPtr lhs, ExprSPtr rhs)
	: Expr(pos), _lhs(lhs), _rhs(rhs) {}

	virtual AddExpr *
	as_add_expr() {
		return this;
	}

	ExprSPtr 
	rhs() const {
		return _rhs;
	}

	ExprSPtr
	lhs() const {
		return _lhs;
	}

	virtual bool
	is_constant() const {
		return _lhs->is_constant() && _rhs->is_constant();
	}

	VISITABLE()
private:
	DISALLOW_COPY_AND_SWAP(AddExpr);
};

class SubExpr : public Expr {
	ExprSPtr _lhs, _rhs;
public:
	SubExpr(Position const &pos, ExprSPtr lhs, ExprSPtr rhs)
	: Expr(pos), _lhs(lhs), _rhs(rhs) {}

	virtual SubExpr*
	as_sub_expr() {
		return this;
	}

	ExprSPtr
	rhs() const {
		return _rhs;
	}

	ExprSPtr
	lhs() const {
		return _lhs;
	}

	virtual bool
	is_constant() const {
		return _lhs->is_constant() && _rhs->is_constant();
	}

	VISITABLE()
private:
	DISALLOW_COPY_AND_SWAP(SubExpr);
};

class NegExpr : public Expr {
	ExprSPtr _value;
public:
	NegExpr(Position const &pos, ExprSPtr e)
	: Expr(pos), _value(e) {}

	ExprSPtr
	value() const {
		return _value;
	}

	virtual NegExpr *
	as_neg_expr() {
		return this;
	}

	virtual bool
	is_constant() const {
		return _value->is_constant();
	}

	VISITABLE()
private:
	DISALLOW_COPY_AND_SWAP(NegExpr);
};

class MulExpr : public Expr {
	ExprSPtr _lhs, _rhs;
public:
	MulExpr(Position const &pos, ExprSPtr lhs, ExprSPtr rhs)
	: Expr(pos), _lhs(lhs), _rhs(rhs) {}

	virtual MulExpr *
	as_mul_expr() {
		return this;
	}
	
	ExprSPtr 
	rhs() const {
		return _rhs;
	}

	ExprSPtr
	lhs() const {
		return _lhs;
	}

	virtual bool
	is_constant() const {
		return _lhs->is_constant() && _rhs->is_constant();
	}

	VISITABLE()
private:
	DISALLOW_COPY_AND_SWAP(MulExpr);
};

class DivExpr : public Expr {
	ExprSPtr _lhs, _rhs;
public:
	DivExpr(Position const &pos, ExprSPtr lhs, ExprSPtr rhs)
	: Expr(pos), _lhs(lhs), _rhs(rhs) {}

	virtual DivExpr *
	as_div_expr() {
		return this;
	}
	
	ExprSPtr 
	rhs() const {
		return _rhs;
	}

	ExprSPtr
	lhs() const {
		return _lhs;
	}

	virtual bool
	is_constant() const {
		return _lhs->is_constant() && _rhs->is_constant();
	}


	VISITABLE()
};

class VarExpr : public Expr {
	std::string const _name;
public:

	VarExpr(Position const &pos, std::string const &name)
	: Expr(pos), _name(name) {}

	virtual VarExpr *
	as_var_expr() {
		return this;
	}

	std::string const&
	name() const {
		return _name;
	}

	virtual bool
	is_constant() const {
		return false;
	}

	VISITABLE()
private:
	DISALLOW_COPY_AND_SWAP(VarExpr);
};


// only supports constant exponents
class ExptExpr : public Expr {
	int _power;
	ExprSPtr _base;
public:
	ExptExpr(Position const &pos, int power, ExprSPtr base)
	: Expr(pos), _power(power), _base(base) {}

	int
	power() const {
		return _power;
	}

	virtual ExptExpr*
	as_expt_expr() {
		return this;
	}

	ExprSPtr
	base() const {
		return _base;
	}

	virtual bool
	is_constant() const {
		return _base->is_constant();
	}

	VISITABLE()
private:
	DISALLOW_COPY_AND_SWAP(ExptExpr);
};

class LitExpr : public Expr {
	interval _value;
public:
	LitExpr(Position const &pos, interval const &value)
	: Expr(pos), _value(value) {}

	virtual LitExpr*
	as_lit_expr() {
		return this;
	}

	virtual bool
	is_constant() const {
		return true;
	}

	VISITABLE()
private:
	DISALLOW_COPY_AND_SWAP(LitExpr);
};



}


#endif