#ifndef __VISITOR_BASE_HH__
#define __VISITOR_BASE_HH__

#include "common.hh"

namespace calc {

class BaseVisitor {
public:
	virtual ~BaseVisitor() {}
};

template <typename T, typename R = void>
class Visitor {
public:
	typedef R ReturnType;
	virtual ReturnType visit(T&) = 0;
};


template <typename R = void>
class BaseVisitable {
public:

	typedef R ReturnType;

	virtual ~BaseVisitable() {}
	
	virtual R accept(BaseVisitor&) = 0;

protected:

	template <typename T> 
	static ReturnType acceptVisitor(T &visited, BaseVisitor &guest) {
		if (Visitor<T>* p = dynamic_cast<Visitor<T>*>(&guest))
			return p->visit(visited);
		return ReturnType();
	}

};

#define VISITABLE() \
virtual ReturnType accept(BaseVisitor &guest) { \
	return acceptVisitor(*this, guest); \
}

}


#endif