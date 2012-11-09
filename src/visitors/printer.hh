#ifndef __PRINTER_HH__
#define __PRINTER_HH__

#include <sstream>
#include <limits>

#include "expr.hh"
#include "visitors/visitorbase.hh"

namespace calc {

class Printer : ExprVisitor {
private:
	std::ostream &_os;
	bool _color; // use color
	int _prec; // current precedence (for parens)
	int _precis; // initial ostream precision (reset afterwards)
	template <typename T> void style_out(T const &i, int t);

public:

	void print_interval(interval i);

	static std::string stringify(Expr &e) {
		std::stringstream ss;
		Printer(ss, false).print(e);
		return ss.str();
	}

	static void output(ExprPtr &e) { Printer(std::cout, true).print(*e); }

	Printer(std::ostream &os, bool color=false)
	: _os(os), _color(color), _prec(0), _precis(_os.precision()) {
		_os.precision(std::numeric_limits<real>::digits10 + 1);
//		_os << std::fixed;
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
	void visit(CallExpr &e);
	void visit(EmptyExpr &e);
	void visit(FuncExpr &e);
};

inline std::ostream &operator<<(std::ostream &o, Expr &e) {
	Printer(o, false).print(e);
	return o;
}

inline void dump(Expr &e, std::ostream &o=std::cout) {
	Printer(o, false).print(e);
}

inline void dumpln(Expr &e, std::ostream &o=std::cout) {
	Printer(o, false).print(e);
	o << std::endl;
}




}












#endif