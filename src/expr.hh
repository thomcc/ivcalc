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

	virtual AddExpr const*
	as_add_expr() const {
		return NULL;
	}

	virtual SubExpr const*
	as_sub_expr() const {
		return NULL;
	}

	virtual NegExpr const*
	as_neg_expr() const {
		return NULL;
	}

	virtual MulExpr const*
	as_mul_expr() const {
		return NULL;
	}

	virtual DivExpr const*
	as_div_expr() const {
		return NULL;
	}

	virtual VarExpr const*
	as_var_expr() const {
		return NULL;
	}

	virtual ExptExpr const*
	as_expt_expr() const {
		return NULL;
	}

	virtual LitExpr const*
	as_lit_expr() const {
		return NULL;
	}

	virtual AssignExpr const*
	as_assign_expr() const {
		return NULL;
	}

	virtual CallExpr const*
	as_call_expr() const {
		return NULL;
	}

	virtual EmptyExpr const*
	as_empty_expr() const {
		return NULL;
	}

	virtual bool
	is_constant() const {
		return false;
	}

	virtual bool
	is_lit_zero() const {
		return false;
	}

	virtual bool
	is_lit_one() const {
		return false;
	}

	virtual bool
	operator==(Expr const &other) const {
		return false;
	}

	VISITABLE()
};

class AddExpr : public Expr {
	ExprSPtr _lhs, _rhs;
public:
	AddExpr(ExprSPtr lhs, ExprSPtr rhs)
	: _lhs(lhs), _rhs(rhs) {}

	AddExpr const*
	as_add_expr() const {
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

	bool
	is_constant() const {
		return _lhs->is_constant() && _rhs->is_constant();
	}

	bool
	operator==(Expr const &other) const {
		if (AddExpr const *e = other.as_add_expr())
			return (*_lhs == *e->lhs()) && (*_rhs == *e->rhs());
		return false;
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

	SubExpr const*
	as_sub_expr() const {
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

	bool
	is_constant() const {
		return _lhs->is_constant() && _rhs->is_constant();
	}

	bool
	operator==(Expr const &other) const {
		if (SubExpr const *e = other.as_sub_expr())
			return (*_lhs == *e->lhs()) && (*_rhs == *e->rhs());
		return false;
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

	NegExpr const*
	as_neg_expr() const {
		return this;
	}

	bool
	is_constant() const {
		return _value->is_constant();
	}

	bool
	operator==(Expr const &other) const {
		if (NegExpr const *e = other.as_neg_expr()) 
			return *_value == *e->value();
		return false;
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

	MulExpr const*
	as_mul_expr() const {
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

	bool
	is_constant() const {
		return _lhs->is_constant() && _rhs->is_constant();
	}

	bool
	operator==(Expr const &other) const {
		if (MulExpr const *e = other.as_mul_expr())
			return (*_lhs == *e->lhs()) && (*_rhs == *e->rhs());
		return false;
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

	DivExpr const*
	as_div_expr() const {
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

	bool
	is_constant() const {
		return _lhs->is_constant() && _rhs->is_constant();
	}

	bool
	operator==(Expr const &other) const {
		if (DivExpr const *e = other.as_div_expr())
			return (*_lhs == *e->lhs()) && (*_rhs == *e->rhs());
		else return false;
	}


	VISITABLE()
};

class VarExpr : public Expr {
	std::string const _name;
public:

	VarExpr(std::string const &name)
	: _name(name) {}

	VarExpr const *
	as_var_expr() const {
		return this;
	}

	std::string const&
	name() const {
		return _name;
	}

	bool
	is_constant() const {
		return false;
	}

	bool
	operator==(Expr const &other) const {
		if (VarExpr const *e = other.as_var_expr()) return _name == e->name();
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

	ExptExpr const*
	as_expt_expr() const {
		return this;
	}

	ExprSPtr
	base() const {
		return _base;
	}

	bool
	is_constant() const {
		return _base->is_constant();
	}

	bool
	operator==(Expr const &other) const {
		if (ExptExpr const *e = other.as_expt_expr())
			return (_power == e->power()) && (*_base == *e->base());
		return false;
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
	LitExpr(interval &&value)
	: _value(value) {}

	LitExpr const*
	as_lit_expr() const {
		return this;
	}

	interval const&
	value() const {
		return _value;
	}

	bool
	is_constant() const {
		return true;
	}

	bool
	is_lit_zero() const {
		return _value.is_zero();
	}

	bool
	is_lit_one() const {
		return _value.is_one();
	}

	bool
	operator==(Expr const &other) const {
		if (LitExpr const *e = other.as_lit_expr()) return _value == e->value();
		return false;
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

	AssignExpr const*
	as_assign_expr() const {
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

	bool
	is_constant() const {
		return false;
	}

	bool
	operator==(Expr const &other) const {
		if (AssignExpr const *e = other.as_assign_expr()) 
			return _name == e->name() && (*_value == *e->value());
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

	CallExpr const*
	as_call_expr() const {
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

	bool
	is_constant() const {
		for (ExprSPtr arg : _args)
			if (!arg->is_constant()) return false;
		return true;
	}

	bool
	operator==(Expr const &other) const {
		if (CallExpr const*e = other.as_call_expr()) {
			if (_name != e->name()) return false;
			if (_args.size() != e->args().size()) return false;
			std::vector<ExprSPtr> const &eargs = e->args();
			for (size_t i = 0; i < _args.size(); ++i)
				if (!(*_args.at(i) == *eargs.at(i)))
					return false;
			return true;
		} else return false;
	}


	VISITABLE()

private:
	DISALLOW_COPY_AND_SWAP(CallExpr);

};

class EmptyExpr : public Expr {
public:
	EmptyExpr() {}

	EmptyExpr const*
	as_empty_expr() const {
		return this;
	}

	bool
	operator==(Expr const &other) const {
		return other.as_empty_expr() ? true : false;
	}

	VISITABLE()
private:
	DISALLOW_COPY_AND_SWAP(EmptyExpr);
};





}


#endif