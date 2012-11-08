#ifndef __VISITOR_BASE_HH__
#define __VISITOR_BASE_HH__

#include "common.hh"
#include "utilities.hh"

namespace calc {

// visitor based on implementation in chapter 10 of
// Modern C++ Design: Generic Programming and Design Patterns Applied
// by Andrei Alexandrescu


class BaseVisitor {
public:
	virtual ~BaseVisitor() {}
};

template <typename T, typename R = void> class Visitor {
public:
	typedef R ReturnType;
	typedef T ArgType;
	virtual ReturnType visit(ArgType&) = 0;
};

template <typename A, typename D, typename R> class Visitor<Typelist<A, D>, R>
: public Visitor<A, R>, public Visitor<D, R> {
public:
	typedef R ReturnType;
};

template <typename A, typename R> class Visitor<Typelist<A, NullType>, R> : public Visitor<A, R> {
public:
	typedef R ReturnType;
	using Visitor<A, R>::visit;
};

template <typename R = void> class BaseVisitable {
public:
	typedef R ReturnType;
	virtual ~BaseVisitable() {}
//	virtual R accept(BaseVisitor&) = 0;
protected:
	template <typename T> static ReturnType acceptVisitor(T &visited, BaseVisitor &guest) {
		if (Visitor<T>* p = dynamic_cast<Visitor<T>*>(&guest)) return p->visit(visited);
		return ReturnType();
	}

};

#define VISITABLE() \
virtual ReturnType accept(BaseVisitor &guest) { \
	return acceptVisitor(*this, guest); \
}

// this version avoids dynamic_cast at the cost of a cyclic dependancy
// (good god, there are already so many of these it's absurd to think i cared
// at the beginning)


template <typename R, typename List> class CyclicVisitor : public Visitor<List, R> {
public:
	typedef R ReturnType;
	template <typename T> ReturnType generic_visit(T &visited) {
		Visitor<T, ReturnType> &self = *this;
		return self.visit(visited);
	}
};

#define CYCLIC_VISITABLE(VisitorType) \
virtual VisitorType::ReturnType accept(VisitorType &guest) {\
	return guest.generic_visit(*this); \
}

typedef CyclicVisitor<
	void,
	Seq<
		Expr,
		AddExpr,
		SubExpr,
		NegExpr,
		MulExpr,
		DivExpr,
		VarExpr,
		ExptExpr,
		LitExpr,
		CallExpr,
		FuncExpr,
		EmptyExpr
	>::Type
> ExprVisitor;

#define EXPR_VISITABLE() CYCLIC_VISITABLE(ExprVisitor)

}


#endif