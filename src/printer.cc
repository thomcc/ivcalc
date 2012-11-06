#include "printer.hh"
#include <parser/parser.hh>
namespace calc {

enum Style {
	Bold = 0, Italic, Underline, Inverse,
	White, Grey, Black, Blue, Cyan, Green,
	Magenta, Red, Yellow, NSTYLES
};

enum Thing { Var, Func, Number, Empty, Operator, NTHINGS };

static const std::string
style_lookup[NSTYLES][2] = {
	{"1",  "22"}, // Bold
	{"3",  "23"}, // Italic
	{"4",  "24"}, // Underline
	{"7",  "27"}, // Inverse
	{"37", "39"}, // White
	{"90", "39"}, // Grey
	{"30", "39"}, // Black
	{"34", "39"}, // Blue
	{"36", "39"}, // Cyan
	{"32", "39"}, // Green
	{"35", "39"}, // Magenta
	{"31", "39"}, // Red
	{"33", "39"}, // Yellow
};

const Style things[NTHINGS] = { Magenta, Blue, Yellow, Grey, White, };

template <typename T> void Printer::style_out(T const &i, int t) {
	if ((!_color) || (t >= NTHINGS)) { _os << i; return; }
	_os << "\x1b[" << style_lookup[things[t]][0] << "m";
	_os << i;
	_os << "\x1b[" << style_lookup[things[t]][1] << "m";
}

void Printer::print_interval(interval i) {
	if (i.is_empty())
		style_out("[]", Empty);
	else if (i.is_singleton()) {
//		_os << "[";
		style_out(i.lo(), Number);
//		_os << "]";
	} else {
		_os << "[";
		style_out(i.lo(), Number);
		_os << ", ";
		style_out(i.hi(), Number);
		_os << "]";
	}
}


void Printer::visit(AddExpr &e) {
	if (_prec >= P_Term) _os << "(";
	print(*e.lhs(), P_Term);
	style_out(" + ", Operator);
	print(*e.rhs(), P_Term);
	if (_prec >= P_Term) _os << ")";
}

void Printer::visit(SubExpr &e) {
	if (_prec >= P_Term) _os << "(";
	print(*e.lhs(), P_Term);
	style_out(" - ", Operator);
	print(*e.rhs(), P_Term);
	if (_prec >= P_Term) _os << ")";
}

void Printer::visit(NegExpr &e) {
	if (_prec >= P_Prefix) _os << "(";
	style_out("-", Operator);
	print(*e.value(), P_Prefix);
	if (_prec >= P_Prefix) _os << ")";
}

void Printer::visit(MulExpr &e) {
	if (_prec >= P_Prod) _os << "(";
	print(*e.lhs(), P_Prod);
	style_out("*", Operator);
	print(*e.rhs(), P_Prod);
	if (_prec >= P_Prod) _os << ")";
}

void Printer::visit(DivExpr &e) {
	if (_prec >= P_Prod) _os << "(";
	print(*e.lhs(), P_Prod);
	style_out("/", Operator);
	print(*e.rhs(), P_Prod);
	if (_prec >= P_Prod) _os << ")";
}

void Printer::visit(VarExpr &e) { style_out(e.name(), Var); }
void Printer::visit(LitExpr &e) { print_interval(e.value()); }
void Printer::visit(EmptyExpr &e) { style_out("(#empty)", Empty); }
void Printer::visit(ExptExpr &e) {
	if (_prec >= P_Expt) _os << "(";
	print(*e.base(), P_Expt);
	style_out("^", Operator);
	style_out(e.power(), Number);
	if (_prec >= P_Expt) _os << ")";
}

void Printer::visit(AssignExpr &e) {
	style_out(e.name(), Var);
	style_out(" = ", Operator);
	print(*e.value());
}

void Printer::visit(CallExpr &e) {
	style_out(e.name(), Func);
	_os << "(";
	bool fst = true;
	for (auto const &ex : e.args()) {
		if (fst) fst = false;
		else _os << ", ";
		print(*ex);
	}
	_os << ")";
}

void Printer::visit(FuncExpr &e) {
	style_out(e.name(), Func);
	_os << "(";
	bool fst = true;
	for (auto const &p : e.params()) {
		if (fst) fst = false;
		else _os << ", ";
		style_out(p, Var);
	}
	_os << ") = ";
	print(*e.impl());
}





}
