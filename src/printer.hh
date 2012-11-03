#ifndef __PRINTER_HH__
#define __PRINTER_HH__

#include "expr.hh"
#include "visitorbase.hh"
#include <sstream>
#include <limits>
namespace calc {

class Printer
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
, public Visitor<FuncExpr>
, public Visitor<EmptyExpr> {

private:
	std::ostream &_os;
	bool _color;
	int _prec;

	template <typename T>
	void style_out(T const &i, int t);

public:

	void print_interval(interval i);

	static std::string
	stringify(ExprSPtr e) {
		std::stringstream ss;
		Printer p(ss, false);
		p.print(*e);
//		e->accept(p);
		return ss.str();
	}

	static void
	output(ExprSPtr e) {
		Printer p(std::cout, true);
		p.print(*e);
//		e->accept(p);
	}

	Printer(std::ostream &os, bool color=false)
	: _os(os), _color(color), _prec(0) {
		_os.precision(std::numeric_limits<real>::digits10 + 1);
	}

	void
	print(Expr &e, int prec=0) {
		int op = _prec;
		_prec = prec;
		e.accept(*this);
		_prec = op;
	}

	void visit(AddExpr &e);
	void visit(SubExpr &e);
	void visit(NegExpr &e);
	void visit(MulExpr &e);
	void visit(DivExpr &e);
	void visit(VarExpr &e);
	void visit(ExptExpr &e);
	void visit(LitExpr &e);
	void visit(AssignExpr &e);
	void visit(CallExpr &e);
	void visit(EmptyExpr &e);
	void visit(FuncExpr &e);

};

inline std::ostream&
operator<<(std::ostream &o, Expr &e) {
	Printer p(o);
	e.accept(p);
	return o;
}




}












#endif