#ifndef __EXPR_HH__
#define __EXPR_HH__
#include <vector>
#include <memory>
#include "common.hh"
#include "interval.hh"
#include "parser/token.hh"
#include "visitorbase.hh"

namespace calc {
class Expr;
class AddExpr;
class SubExpr;
class NegExpr;
class MulExpr;
class DivExpr;
class VarExpr;
class ExptExpr;
class LitExpr;
class AssignExpr;
class CallExpr;
class EmptyExpr;



typedef std::shared_ptr<Expr> ExprSPtr;

class Expr : public BaseVisitable<> {
public:
	
	virtual ~Expr() {}

	virtual AddExpr*
	as_add_expr() { 
		return NULL; 
	}

	virtual SubExpr*
	as_sub_expr() { 
		return NULL; 
	}

	virtual NegExpr*
	as_neg_expr() { 
		return NULL; 
	}

	virtual MulExpr*
	as_mul_expr() { 
		return NULL; 
	}

	virtual DivExpr*
	as_div_expr() { 
		return NULL; 
	}

	virtual VarExpr*
	as_var_expr() { 
		return NULL; 
	}

	virtual ExptExpr*
	as_expt_expr() { 
		return NULL; 
	}

	virtual LitExpr*
	as_lit_expr() {
		return NULL;
	}

	virtual AssignExpr*
	as_assign_expr() {
		return NULL;
	}

	virtual CallExpr*
	as_call_expr() {
		return NULL;
	}

	virtual EmptyExpr*
	as_empty_expr() {
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
	AddExpr(ExprSPtr lhs, ExprSPtr rhs)
	: _lhs(lhs), _rhs(rhs) {}

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
	SubExpr(ExprSPtr lhs, ExprSPtr rhs)
	: _lhs(lhs), _rhs(rhs) {}

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
	NegExpr(ExprSPtr e)
	: _value(e) {}

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
	MulExpr(ExprSPtr lhs, ExprSPtr rhs)
	: _lhs(lhs), _rhs(rhs) {}

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
	DivExpr(ExprSPtr lhs, ExprSPtr rhs)
	: _lhs(lhs), _rhs(rhs) {}

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

	VarExpr(std::string const &name)
	: _name(name) {}

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
	ExptExpr(int power, ExprSPtr base)
	: _power(power), _base(base) {}

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
	LitExpr(number_type v)
	: _value(v) {}
	LitExpr(interval const &value)
	: _value(value) {}

	virtual LitExpr*
	as_lit_expr() {
		return this;
	}

	interval const&
	value() const {
		return _value;
	}

	virtual bool
	is_constant() const {
		return true;
	}

	VISITABLE()
private:
	DISALLOW_COPY_AND_SWAP(LitExpr);
};

class AssignExpr : public Expr {
	std::string const _name;
	ExprSPtr _value;
public:

	AssignExpr(std::string const &name, ExprSPtr value)
	: _name(name), _value(value) {}

	virtual AssignExpr *
	as_assign_expr() {
		return this;
	}

	std::string const&
	name() const {
		return _name;
	}

	ExprSPtr
	value() const {
		return _value;
	}

	virtual bool
	is_constant() const {
		return false;
	}

	VISITABLE()

private:
	DISALLOW_COPY_AND_SWAP(AssignExpr);

};

class CallExpr : public Expr {
	std::string const _name;
	std::vector<ExprSPtr> _args;
public:
	CallExpr(std::string const &name, std::vector<ExprSPtr> const &args)
	: _name(name), _args(args) {}

	virtual CallExpr *
	as_call_expr() {
		return this;
	}

	std::string const&
	name() const {
		return _name;
	}

	std::vector<ExprSPtr> const&
	args() const {
		return _args;
	}

	virtual bool
	is_constant() const {
		for (ExprSPtr arg : _args)
			if (!arg->is_constant()) return false;
		return true;
	}

	virtual ReturnType
	Accept(BaseVisitor &v) {

	}
	VISITABLE()

private:
	DISALLOW_COPY_AND_SWAP(CallExpr);

};

class EmptyExpr : public Expr {
public:
	EmptyExpr() {}

	virtual EmptyExpr*
	as_empty_expr() {
		return this;
	}

	VISITABLE()
private:
	DISALLOW_COPY_AND_SWAP(EmptyExpr);
};





}


#endif