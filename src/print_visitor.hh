#ifndef __PRINT_VISITOR_HH__
#define __PRINT_VISITOR_HH__

#include "expr.hh"
#include "visitorbase.hh"
//#include "colors.h"
#include <sstream>
#include <iomanip>
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
	bool _color;

	enum Style {
		Bold = 0,
		Italic,
		Underline,
		Inverse,
		White,
		Grey,
		Black,
		Blue,
		Cyan,
		Green,
		Magenta,
		Red,
		Yellow,
		NSTYLES
	};

	enum Thing {
		Var,
		Func,
		Number,
		Empty,
		Operator,
		NTHINGS
	};

	const std::string
	style_lookup[NSTYLES][2] = {
		{"1", "22"},//  Bold
		{"3", "23"},//  Italic
		{"4", "24"},//  Underline
		{"7", "27"},//  Inverse
		{"37", "39"},// White
		{"90", "39"},// Grey
		{"30", "39"},// Black
		{"34", "39"},// Blue
		{"36", "39"},// Cyan
		{"32", "39"},// Green
		{"35", "39"},// Magenta
		{"31", "39"},// Red
		{"33", "39"},// Yellow
	};

	const Style things[NTHINGS] = {
		Magenta,
		Blue,
		Yellow,
		Grey,
		White,
	};

	template <typename T>
	void
	style_out(T const &i, Thing t) {
		if ((!_color) || (t >= NTHINGS)) { _os << i; return; }
		_os << "\x1b[" << style_lookup[things[t]][0] << "m";
		_os << i;
		_os << "\x1b[" << style_lookup[things[t]][1] << "m";
	}

public:

	void
	print_interval(interval i) {
		if (i.is_empty())
			style_out("[]", Empty);
		else if (i.is_singleton()) {
			_os << "[";
			style_out(i.lo(), Number);
			_os << "]";
		} else {
			_os << "[";
			style_out(i.mid(), Number);
			_os << " ±";
			style_out(i.uncertainty(), Number);
			//style_out(i.lo(), Number);
			//_os << ", ";
			//style_out(i.hi(), Number);
			_os << "]";
		}
	}

	static std::string
	stringify(ExprSPtr e) {
		std::stringstream ss;
		PrintVisitor p(ss);
		e->accept(p);
		return ss.str();
	}

	static void
	output(ExprSPtr e) {
		PrintVisitor p(std::cout, true);
		e->accept(p);
	}

	PrintVisitor(std::ostream &os, bool color=false)
	: _os(os), _color(color) {
		_os << std::setiosflags(std::ios::scientific);
	}

	void
	print(ExprSPtr e) {
		e->accept(*this);
	}

	void
	visit(AddExpr &e) {
		_os << "(";
		print(e.lhs());
		style_out(" + ", Operator);
		print(e.rhs());
		_os << ")";	}

	void
	visit(SubExpr &e) {
		_os << "(";
		print(e.lhs());
		style_out(" - ", Operator);
		print(e.rhs());
		_os << ")";
	}

	void
	visit(NegExpr &e) {
		_os << "(";
		style_out("-", Operator);
		print(e.value());
		_os << ")";
	}

	void
	visit(MulExpr &e) {
		_os << "(";
		print(e.lhs());
		style_out(" * ", Operator);
		print(e.rhs());
		_os << ")";
	}

	void
	visit(DivExpr &e) {
		_os << "(";
		print(e.lhs());
		style_out(" / ", Operator);
		print(e.rhs());
		_os << ")";
	}
	
	void
	visit(VarExpr &e) {
		style_out(e.name(), Var);
	}

	void
	visit(ExptExpr &e) {
		print(e.base());
		style_out("^", Operator);
		style_out(e.power(), Number);
	}

	void
	visit(LitExpr &e) {
		print_interval(e.value());
	}
	
	void
	visit(AssignExpr &e) {
		style_out(e.name(), Var);
		style_out(" = ", Operator);
		print(e.value());
	}
	
	void
	visit(CallExpr &e) {
		style_out(e.name(), Func);
		_os << "(";
		bool fst = true;
		for (ExprSPtr ex : e.args()) {
			if (fst) fst = false;
			else _os << ", ";
			print(ex);
		}
		_os << ")";
	}
	
	void
	visit(EmptyExpr &e) {
		style_out("(#empty)", Empty);
	}

};






}












#endif