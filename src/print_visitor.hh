#ifndef __PRINT_VISITOR_HH__
#define __PRINT_VISITOR_HH__
#include "expr.hh"
#include "visitorbase.hh"
#include <sstream>
namespace calc {

class PrintVisitor
: public BaseVisitor
, public Visitor<AddExpr>
, public Visitor<SubExpr>
, public Visitor<NegExpr>
, public Visitor<MulExpr>
, public Visitor<DivExpr>
, public Visitor<VarExpr>
, public Visitor<ExptExpr>
, public Visitor<LitExpr>
, public Visitor<AssignExpr>
, public Visitor<CallExpr>
, public Visitor<EmptyExpr> {

	std::ostream &_os;

public:

	static std::string
	stringify(ExprSPtr e) {
		std::stringstream ss;
		PrintVisitor p(ss);
		e->accept(p);
		return ss.str();
	}

	static void
	output(ExprSPtr e) {
		PrintVisitor p(std::cout);
		e->accept(p);
	}

	PrintVisitor(std::ostream &os)
	: _os(os) {}

	void
	visit(AddExpr &e) {
		_os << "(";
		e.lhs()->accept(*this);
		_os << " + ";
		e.rhs()->accept(*this);
		_os << ")";	}

	void
	visit(SubExpr &e) {
		_os << "(";
		e.lhs()->accept(*this);
		_os << " - ";
		e.rhs()->accept(*this);
		_os << ")";
	}

	void
	visit(NegExpr &e) {
		_os << "(-";
		e.value()->accept(*this);
		_os << ")";
	}

	void
	visit(MulExpr &e) {
		_os << "(";
		e.lhs()->accept(*this);
		_os << " * ";
		e.rhs()->accept(*this);
		_os << ")";
	}

	void
	visit(DivExpr &e) {
		_os << "(";
		e.lhs()->accept(*this);
		_os << " / ";
		e.rhs()->accept(*this);
		_os << ")";
	}
	
	void
	visit(VarExpr &e) {
		_os << e.name();
	}

	void
	visit(ExptExpr &e) {
		e.base()->accept(*this);
		_os << "^" << e.power();
	}

	void
	visit(LitExpr &e) {
		_os << e.value();
	}
	
	void
	visit(AssignExpr &e) {
		_os << e.name() << " = ";
		e.value()->accept(*this);
	}
	
	void
	visit(CallExpr &e) {
		_os << e.name() << "(";
		bool fst = true;
		for (ExprSPtr ex : e.args()) {
			if (fst) fst = false;
			else _os << ", ";
			ex->accept(*this);
		}
		_os << ")";
	}
	
	void
	visit(EmptyExpr &e) {
		_os << "(#empty)";
	}

};






}












#endif