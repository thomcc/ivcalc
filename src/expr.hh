#ifndef __EXPR_HH__
#define __EXPR_HH__
#include <vector>
#include <memory>
#include "common.hh"
#include "interval.hh"
#include <visitors/visitorbase.hh>

namespace calc {

class Expr : public BaseVisitable<> {
public:

	virtual ~Expr() {}



	virtual AddExpr const *as_add_expr() const { return nullptr; }
	virtual SubExpr const *as_sub_expr() const { return nullptr; }
	virtual NegExpr const *as_neg_expr() const { return nullptr; }
	virtual MulExpr const *as_mul_expr() const { return nullptr; }
	virtual DivExpr const *as_div_expr() const { return nullptr; }
	virtual VarExpr const *as_var_expr() const { return nullptr; }
	virtual ExptExpr const *as_expt_expr() const { return nullptr; }
	virtual LitExpr const *as_lit_expr() const { return nullptr; }
	virtual CallExpr const *as_call_expr() const { return nullptr; }
	virtual FuncExpr const *as_func_expr() const { return nullptr; }
	virtual EmptyExpr const *as_empty_expr() const { return nullptr; }
	virtual AddExpr *as_add_expr() { return nullptr; }
	virtual SubExpr *as_sub_expr() { return nullptr; }
	virtual NegExpr *as_neg_expr() { return nullptr; }
	virtual MulExpr *as_mul_expr() { return nullptr; }
	virtual DivExpr *as_div_expr() { return nullptr; }
	virtual VarExpr *as_var_expr() { return nullptr; }
	virtual ExptExpr *as_expt_expr() { return nullptr; }
	virtual LitExpr *as_lit_expr() { return nullptr; }
	virtual CallExpr *as_call_expr() { return nullptr; }
	virtual FuncExpr *as_func_expr() { return nullptr; }
	virtual EmptyExpr *as_empty_expr() { return nullptr; }

	virtual bool is_constant() const { return false; }
	virtual bool is_lit_zero() const { return false; }
	virtual bool is_lit_one() const { return false; }

	virtual bool operator==(Expr const &other) const = 0;
	virtual ExprPtr clone() const = 0;
	virtual ExprVisitor::ReturnType accept(ExprVisitor &g) = 0;

	// factory methods to create subclasses
	static ExprPtr make_add(ExprPtr l, ExprPtr r);
	static ExprPtr make_sub(ExprPtr l, ExprPtr r);
	static ExprPtr make_neg(ExprPtr v);
	static ExprPtr make_mul(ExprPtr l, ExprPtr r);
	static ExprPtr make_div(ExprPtr l, ExprPtr r);
	static ExprPtr make_var(std::string const &name, int i=-1);
	static ExprPtr make_expt(ExprPtr b, int e);
	static ExprPtr make_lit(real v);
	static ExprPtr make_lit(interval v);
	static ExprPtr make_lit(real min, real max);
	static ExprPtr make_call(std::string const &name, std::vector<ExprPtr> args);
	static ExprPtr make_func(std::string const &name, std::vector<std::string> const &params, ExprPtr impl);
	static ExprPtr make_func(ExprPtr impl);
	static ExprPtr make_empty();
	static void stats();

//	EXPR_VISITABLE()
};

//#define GET_SHARED() ExprSPtr get_shared() { return std::static_pointer_cast<Expr>(shared_from_this()); }

class AddExpr : public Expr {
	ExprPtr _lhs, _rhs;
public:
	AddExpr(ExprPtr lhs, ExprPtr rhs) : _lhs(std::move(lhs)), _rhs(std::move(rhs)) {
		assert(_lhs.get());
		assert(_rhs.get());
	}

	AddExpr const *as_add_expr() const { return this; }
	AddExpr *as_add_expr() { return this; }
	ExprPtr const &rhs() const { return _rhs; }
	ExprPtr const &lhs() const { return _lhs; }
	ExprPtr &rhs() { return _rhs; }
	ExprPtr &lhs() { return _lhs; }
	ExprPtr clone() const { return Expr::make_add(_lhs->clone(), _rhs->clone()); }
	bool is_constant() const { return _lhs->is_constant() && _rhs->is_constant(); }

	bool operator==(Expr const &other) const {
		if (AddExpr const *e = other.as_add_expr()) return (*_lhs == *e->lhs()) && (*_rhs == *e->rhs());
		return false;
	}


	EXPR_VISITABLE()

	static void *operator new(size_t size) { return _pool.allocate(size); }
	static void operator delete(void *p) { _pool.deallocate(p); }
	static void stats() { _pool.stats(); }
private:
	static MemPool _pool;
};

class SubExpr : public Expr {
	ExprPtr _lhs, _rhs;
public:
	SubExpr(ExprPtr lhs, ExprPtr rhs) : _lhs(std::move(lhs)), _rhs(std::move(rhs)) {
		assert(_lhs.get());
		assert(_rhs.get());
	}

	SubExpr const *as_sub_expr() const { return this; }
	SubExpr *as_sub_expr() { return this; }
	ExprPtr const &rhs() const { return _rhs; }
	ExprPtr const &lhs() const { return _lhs; }
	ExprPtr &rhs() { return _rhs; }
	ExprPtr &lhs() { return _lhs; }
	ExprPtr clone() const { return Expr::make_sub(_lhs->clone(), _rhs->clone()); }
	bool is_constant() const { return _lhs->is_constant() && _rhs->is_constant(); }
	bool operator==(Expr const &other) const {
		if (SubExpr const *e = other.as_sub_expr()) return (*_lhs == *e->lhs()) && (*_rhs == *e->rhs());
		return false;
	}

	EXPR_VISITABLE()
	static void *operator new(size_t size) { return _pool.allocate(size); }
	static void operator delete(void *p) { _pool.deallocate(p); }
	static void stats() { _pool.stats(); }
private:
	static MemPool _pool;
};

class NegExpr : public Expr {
	ExprPtr _value;
public:
	NegExpr(ExprPtr e) : _value(std::move(e)) {
		assert(_value.get());
	}

	ExprPtr const &value() const { return _value; }
	ExprPtr &value() { return _value; }
	NegExpr const *as_neg_expr() const { return this; }
	NegExpr *as_neg_expr() { return this; }
	bool is_constant() const { return _value->is_constant(); }
	ExprPtr clone() const { return Expr::make_neg(_value->clone()); }
	bool operator==(Expr const &other) const {
		if (NegExpr const *e = other.as_neg_expr()) return *_value == *e->value();
		return false;
	}



	EXPR_VISITABLE()
	static void *operator new(size_t size) { return _pool.allocate(size); }
	static void operator delete(void *p) { _pool.deallocate(p); }
	static void stats() { _pool.stats(); }
private:
	static MemPool _pool;
};

class MulExpr : public Expr {
	ExprPtr _lhs, _rhs;
public:
	MulExpr(ExprPtr lhs, ExprPtr rhs) : _lhs(std::move(lhs)), _rhs(std::move(rhs)) {
		assert(_lhs.get());
		assert(_rhs.get());
	}

	MulExpr const *as_mul_expr() const { return this; }
	MulExpr *as_mul_expr() { return this; }
	ExprPtr const &rhs() const { return _rhs; }
	ExprPtr const &lhs() const { return _lhs; }
	ExprPtr &rhs() { return _rhs; }
	ExprPtr &lhs() { return _lhs; }
	bool is_constant() const { return _lhs->is_constant() && _rhs->is_constant(); }
	ExprPtr clone() const { return Expr::make_mul(_lhs->clone(), _rhs->clone()); }
	bool operator==(Expr const &other) const {
		if (MulExpr const *e = other.as_mul_expr()) return (*_lhs == *e->lhs()) && (*_rhs == *e->rhs());
		return false;
	}



	EXPR_VISITABLE()

	static void *operator new(size_t size) { return _pool.allocate(size); }
	static void operator delete(void *p) { _pool.deallocate(p); }

	static void stats() { _pool.stats(); }
private:
	static MemPool _pool;
};

class DivExpr : public Expr {
	ExprPtr _lhs, _rhs;
public:
	DivExpr(ExprPtr lhs, ExprPtr rhs) : _lhs(std::move(lhs)), _rhs(std::move(rhs)) {
		assert(_lhs.get());
		assert(_rhs.get());
	}

	DivExpr const *as_div_expr() const { return this; }
	DivExpr *as_div_expr() { return this; }
	ExprPtr const &rhs() const { return _rhs; }
	ExprPtr const &lhs() const { return _lhs; }
	ExprPtr &rhs() { return _rhs; }
	ExprPtr &lhs() { return _lhs; }
	bool is_constant() const { return _lhs->is_constant() && _rhs->is_constant() && !_rhs->is_lit_zero(); }
	ExprPtr clone() const { return Expr::make_div(_lhs->clone(), _rhs->clone()); }
	bool operator==(Expr const &other) const {
		if (DivExpr const *e = other.as_div_expr()) return (*_lhs == *e->lhs()) && (*_rhs == *e->rhs());
		else return false;
	}

	EXPR_VISITABLE()
	static void *operator new(size_t size) { return _pool.allocate(size); }
	static void operator delete(void *p) { _pool.deallocate(p); }
	static void stats() { _pool.stats(); }
private:
	static MemPool _pool;
};

class VarExpr : public Expr {
	std::string const _name;
	int _param_no;
public:
	VarExpr(std::string const &name) : _name(name), _param_no(-1) {}
	VarExpr(std::string const &name, int param_no) : _name(name), _param_no(param_no) {}
	VarExpr const *as_var_expr() const { return this; }
	VarExpr *as_var_expr() { return this; }
	std::string const &name() const { return _name; }
	ExprPtr clone() const { return Expr::make_var(_name, _param_no); }
	bool has_param_no() const { return _param_no >= 0; }
	int param_no() const { return _param_no; }
	void set_param_no(unsigned param_no) { _param_no = param_no; }
	bool is_constant() const { return false; }
	bool operator==(Expr const &other) const {
		if (VarExpr const *e = other.as_var_expr()) return _name == e->name();
		return false;
	}

	EXPR_VISITABLE()

	static void *operator new(size_t size) { return _pool.allocate(size); }
	static void operator delete(void *p) { _pool.deallocate(p); }
	static void stats() { _pool.stats(); }
private:
	static MemPool _pool;
};


// only supports constant exponents
class ExptExpr : public Expr {
	int _power;
	ExprPtr _base;
public:
	ExptExpr(ExprPtr base, int power) : _power(power), _base(std::move(base)) {
		assert(_base.get());
	}

	int power() const { return _power; }
	ExprPtr clone() const { return Expr::make_expt(_base->clone(), _power); }
	ExptExpr const *as_expt_expr() const { return this; }
	ExptExpr *as_expt_expr() { return this; }
	ExprPtr const &base() const { return _base; }
	ExprPtr &base() { return _base; }
	bool is_constant() const { return _base->is_constant(); }
	bool operator==(Expr const &other) const {
		if (ExptExpr const *e = other.as_expt_expr()) return (_power == e->power()) && (*_base == *e->base());
		return false;
	}

	EXPR_VISITABLE()
	static void *operator new(size_t size) { return _pool.allocate(size); }
	static void operator delete(void *p) { _pool.deallocate(p); }
	static void stats() { _pool.stats(); }
private:
	static MemPool _pool;
};

class LitExpr : public Expr {
	interval _value;
public:
	LitExpr(real v) : _value(v) {}
	LitExpr(interval value) : _value(value) {}
	LitExpr(real min, real max) : _value(min, max) {}
	ExprPtr clone() const { return Expr::make_lit(_value); }
	LitExpr const *as_lit_expr() const { return this; }
	LitExpr *as_lit_expr() { return this; }
	interval const &value() const { return _value; }
	bool is_constant() const { return true; }
	bool is_lit_zero() const { return _value.is_zero(); }
	bool is_lit_one() const { return _value.is_one(); }
	bool operator==(Expr const &other) const {
		if (LitExpr const *e = other.as_lit_expr()) return _value == e->value();
		return false;
	}

	EXPR_VISITABLE()
	static void *operator new(size_t size) { return _pool.allocate(size); }
	static void operator delete(void *p) { _pool.deallocate(p); }
	static void stats() { _pool.stats(); }
private:
	static MemPool _pool;
};

class CallExpr : public Expr {
	std::string const _name;
	std::vector<ExprPtr> _args;
public:
	CallExpr(std::string const &name, std::vector<ExprPtr> args)
		: _name(name), _args(std::move(args)) {
#ifndef NDEBUG
			for (auto const &a : _args) assert(a.get());
#endif
		}
	ExprPtr clone() const {
		std::vector<ExprPtr> a2;
		for (auto const &arg : _args) a2.push_back(arg->clone());
		return Expr::make_call(_name, std::move(a2));
	}
	CallExpr const *as_call_expr() const { return this; }
	CallExpr *as_call_expr() { return this; }
	std::string const &name() const { return _name; }
	std::vector<ExprPtr> const &args() const { return _args; }
	std::vector<ExprPtr> &args() { return _args; }
// don't know how to evaluate the function w/o definition
	bool is_constant() const { return false; }
	bool operator==(Expr const &other) const {
		if (CallExpr const*e = other.as_call_expr()) return (_name == e->name()) && (_args == e->args());
		return false;
	}

	EXPR_VISITABLE()
	static void *operator new(size_t size) { return _pool.allocate(size); }
	static void operator delete(void *p) { _pool.deallocate(p); }
	static void stats() { _pool.stats(); }
private:
	static MemPool _pool;
};

class FuncExpr : public Expr {
	std::string const _name;
	std::vector<std::string> _params;
	ExprPtr _impl;
public:

	FuncExpr(std::string const &name, std::vector<std::string> const &params, ExprPtr impl)
		: _name(name), _params(params), _impl(std::move(impl)) {
			assert(_impl.get());
		}
	FuncExpr(ExprPtr impl) : _name(""), _params(), _impl(std::move(impl)) {
		assert(_impl.get());
		assert(!_impl->as_func_expr());
	}
	ExprPtr clone() const { return Expr::make_func(_name, _params, _impl->clone()); }
	std::string const &name() const { return _name; }
	std::vector<std::string> const &params() const { return _params; }
	ExprPtr const &impl() const { return _impl; }
	ExprPtr &impl() { return _impl; }
	FuncExpr const *as_func_expr() const { return this; }
	FuncExpr *as_func_expr() { return this; }
	bool is_constant() const { return _impl->is_constant(); }
	bool operator==(Expr const &other) const {
		if (FuncExpr const *e = other.as_func_expr())
			return (_name == e->name()) && (_params == e->params()) && (*_impl == *e->impl());
		return false;
	}

	EXPR_VISITABLE()

	static void *operator new(size_t size) { return _pool.allocate(size); }
	static void operator delete(void *p) { _pool.deallocate(p); }
	static void stats() { _pool.stats(); }
private:
	static MemPool _pool;
};


class EmptyExpr : public Expr {
public:
	EmptyExpr() {}
	EmptyExpr const *as_empty_expr() const { return this; }
	EmptyExpr *as_empty_expr() { return this; }
	bool operator==(Expr const &other) const { return other.as_empty_expr(); }
	ExprPtr clone() const { return Expr::make_empty(); }
	EXPR_VISITABLE()

	static void *operator new(size_t size) { return _pool.allocate(size); }
	static void operator delete(void *p) { _pool.deallocate(p); }
	static void stats() { _pool.stats(); }
private:
	static MemPool _pool;
};


inline ExprPtr Expr::make_add(ExprPtr l, ExprPtr r) { return ExprPtr(new AddExpr(std::move(l), std::move(r))); }
inline ExprPtr Expr::make_sub(ExprPtr l, ExprPtr r) { return ExprPtr(new SubExpr(std::move(l), std::move(r))); }
inline ExprPtr Expr::make_div(ExprPtr l, ExprPtr r) { return ExprPtr(new DivExpr(std::move(l), std::move(r))); }
inline ExprPtr Expr::make_mul(ExprPtr l, ExprPtr r) { return ExprPtr(new MulExpr(std::move(l), std::move(r))); }
inline ExprPtr Expr::make_neg(ExprPtr v) { return ExprPtr(new NegExpr(std::move(v))); }
inline ExprPtr Expr::make_var(std::string const &name, int i) { return ExprPtr(new VarExpr(name, i)); }
inline ExprPtr Expr::make_expt(ExprPtr b, int e) { return ExprPtr(new ExptExpr(std::move(b), e)); }
inline ExprPtr Expr::make_lit(real v) { return ExprPtr(new LitExpr(v)); }
inline ExprPtr Expr::make_lit(interval v) { return ExprPtr(new LitExpr(v)); }
inline ExprPtr Expr::make_lit(real min, real max) { return ExprPtr(new LitExpr(min, max)); }
inline ExprPtr Expr::make_call(std::string const &name, std::vector<ExprPtr> args) { return ExprPtr(new CallExpr(name, std::move(args))); }
inline ExprPtr Expr::make_func(std::string const &name, std::vector<std::string> const &params, ExprPtr impl) { return ExprPtr(new FuncExpr(name, params, std::move(impl))); }
inline ExprPtr Expr::make_func(ExprPtr impl) { return ExprPtr(new FuncExpr(std::move(impl))); }

inline ExprPtr Expr::make_empty() { return ExprPtr(new EmptyExpr()); }

inline void copy_eptrs(std::vector<ExprPtr> const &e, std::vector<ExprPtr> &dst) {
	for (auto const &p : e) dst.push_back(p->clone());
}


inline std::vector<ExprPtr> copy_eptrs(std::vector<ExprPtr> const &e) {
	std::vector<ExprPtr> v;
	copy_eptrs(e, v);
	return v;
}


}


#endif


