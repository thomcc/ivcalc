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
	bool _color; // use color
	int _prec; // current precedence (for parens)
	int _precis; // initial ostream precision (reset afterwards)
	template <typename T> void style_out(T const &i, int t);

public:

	void print_interval(interval i);

	static std::string stringify(ExprSPtr e) {
		std::stringstream ss;
		Printer(ss, false).print(*e);
		return ss.str();
	}

	static void output(ExprSPtr e) { Printer(std::cout, true).print(*e); }

	Printer(std::ostream &os, bool color=false)
	: _os(os), _color(color), _prec(0), _precis(_os.precision()) {
		_os.precision(std::numeric_limits<real>::digits10 + 1);
		_os << std::fixed;
	}
	~Printer() { _os.precision(_precis); }

	void print(Expr &e, int prec=0) {
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

inline std::ostream &operator<<(std::ostream &o, Expr &e) {
	Printer(o, false).print(e);
	return o;
}




}












#endif