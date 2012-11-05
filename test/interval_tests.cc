#include "interval_tests.hh"
#include "interval.hh"
namespace calc {

using namespace std;

static inline real dmin(real a, real b, real c, real d) {
	return std::min(std::min(a, b), std::min(c, d));
}

static inline real dmax(real a, real b, real c, real d) {
	return std::max(std::max(a, b), std::max(c, d));
}

static inline real mulmax(real al, real ah, real bl, real bh) {
	using rmath::mul_hi;
	return dmax(mul_hi(al, bl), mul_hi(al, bh), mul_hi(ah, bl), mul_hi(ah, bh));
}

static inline real mulmin(real al, real ah, real bl, real bh) {
	using rmath::mul_lo;
	return dmin(mul_lo(al, bl), mul_lo(al, bh), mul_lo(ah, bl), mul_lo(ah, bh));
}

static inline real divmax(real al, real ah, real bl, real bh) {
	using rmath::div_hi;
	return dmax(div_hi(al, bl), div_hi(al, bh), div_hi(ah, bl), div_hi(ah, bh));
}

static inline real divmin(real al, real ah, real bl, real bh) {
	using rmath::div_lo;
	return dmin(div_lo(al, bl), div_lo(al, bh), div_lo(ah, bl), div_lo(ah, bh));
}

void IntervalTest::add() {
	CheckEq(interval(4, 6), interval(1, 2)+interval(3, 4));
	CheckEq(interval(1, 3), interval(0, 1)+interval(1, 2));
	CheckEq(interval(2, 3), interval(0, 1)+2);
	CheckEq(interval(2, 3), 2+interval(0, 1));
}

void IntervalTest::neg() {
	CheckEq(interval(-1, -0), -interval(0, 1));
}

void IntervalTest::sub() {
	CheckEq(interval(-3, -1), interval(0, 1) - interval(2, 3));
	CheckEq(interval(-2, -1), interval(0, 1) - 2);
	CheckEq(interval(1, 2), 2 - interval(0, 1));
}

#define CHECK_MUL_II(al, ah, bl, bh) do { \
	interval _a(al, ah), _b(bl, bh); \
	interval _c = _a*_b; \
	CheckEq(mulmin(al, ah, bl, bh), _c.lo()); \
	CheckEq(mulmax(al, ah, bl, bh), _c.hi()); \
} while (0)

#define CHECK_MUL_RI(r, bl, bh) do {\
	interval _a(r), _b(bl, bh); \
	interval _c = r * _b; \
	interval _d = _b * r; \
	interval _e = _a * _b; \
	CheckEq(_c, _d); \
	CheckEq(_d, _e); \
} while (0)


void IntervalTest::mul() {
	CHECK_MUL_II(2, 3, 5, 7);
	CHECK_MUL_II(2, 3, -5, 7);
	CHECK_MUL_II(2, 3, -7, -5);
	CHECK_MUL_II(-2, 3, 5, 7);
	CHECK_MUL_II(-2, 3, -5, 7);
	CHECK_MUL_II(-2, 3, -7, -5);
	CHECK_MUL_II(-3, -2, 5, 7);
	CHECK_MUL_II(-3, -2, -5, 7);
	CHECK_MUL_II(-3, -2, -7, -5);
	CHECK_MUL_RI(3, 5, 7);
	CHECK_MUL_RI(3, -5, 7);
	CHECK_MUL_RI(3, -7, -5);
	CHECK_MUL_RI(-3, 5, 7);
	CHECK_MUL_RI(-3, -5, 7);
	CHECK_MUL_RI(-3, -7, -5);
}

#define CHECK_DIV_II(al, ah, bl, bh) do { \
	interval _a(al, ah), _b(bl, bh); \
	interval _c = _a / _b; \
	CheckEq(divmin(al, ah, bl, bh), _c.lo()); \
	CheckEq(divmax(al, ah, bl, bh), _c.hi()); \
} while (0)

#define CHECK_DIV_IR(al, ah, r) do { \
	interval _a(al, ah), _b(r); \
	interval _c = _a / r; \
	interval _d = _a / _b; \
	CheckEq(_c, _d); \
} while (0)

#define CHECK_DIV_RI(r, bl, bh) do { \
	interval _a(r), _b(bl, bh); \
	interval _c = r / _b; \
	interval _d = _a / _b; \
	CheckEq(_c, _d); \
} while (0)

void IntervalTest::div() {
	CHECK_DIV_II(30, 42, 2, 3);
	CHECK_DIV_II(30, 42, -3, -2);
	CHECK_DIV_II(-30, 42, 2, 3);
	CHECK_DIV_II(-30, 42, -3, -2);
	CHECK_DIV_II(-42, -30, 2, 3);
	CHECK_DIV_II(-42, -30, -3, -2);
	CHECK_DIV_IR(30, 42, 3);
	CHECK_DIV_IR(30, 42, -3);
	CHECK_DIV_IR(-30, 42, 3);
	CHECK_DIV_IR(-30, 42, -3);
	CHECK_DIV_IR(-42, -30, 3);
	CHECK_DIV_IR(-42, -30, -3);
	CHECK_DIV_RI(30, 2, 3);
	CHECK_DIV_RI(30, -3, -2);
	CHECK_DIV_RI(-30, 2, 3);
	CHECK_DIV_RI(-30, -3, -2);
}


// going to test hundreds of things, don't want to drown out everything.
#define ENSURE(C, A, B) do { \
	if (!(C)) { \
		Check(C); \
		std::cout << "\t" << #A << ": " << A << ", " << #B << ": " << B << std::endl; \
		goto endloop; \
	} \
} while (0)
#define ENSURELAST(C, A, B) do { if (!Check(C)) {continue; endloop: return;} } while (0)

template <typename T> void IntervalTest::dofloatb() {
//	std::cout << "TEST BINOP: " << T::describe() << std::endl;
	for (interval a(-10, -9.91); a.lo() <= 10; a += 0.3) {
		for (interval b(-10, -9.91); b.lo() <= 10; b += 0.3) {
			if (!T::is_good(a, b)) continue;
			real al = a.lo(), bl = b.lo(), ah = a.hi(), bh = b.hi();
			interval ii  = T::ii(a, b);
			interval irl = T::ir(a, bl),  irh = T::ir(a, bh);
			interval rli = T::ri(al, b),  rhi = T::ri(ah, b);
			interval rlrl = T::rr(al, bl), rlrh = T::rr(al, bh);
			interval rhrl = T::rr(ah, bl), rhrh = T::rr(ah, bh);
			ENSURE(irl.has(rlrl), irl, rlrl);
			ENSURE(irl.has(rhrl), irl, rhrl);
			ENSURE(irh.has(rlrh), irh, rlrh);
			ENSURE(irh.has(rhrh), irh, rhrh);
			ENSURE(rli.has(rlrl), rli, rlrl);
			ENSURE(rli.has(rlrh), rli, rlrh);
			ENSURE(rhi.has(rhrl), rhi, rhrl);
			ENSURE(rhi.has(rhrh), rhi, rhrh);
			ENSURE(ii.has(irl), ii, irl);
			ENSURE(ii.has(irh), ii, irh);
			ENSURE(ii.has(rli), ii, rli);
			ENSURELAST(ii.has(rhi), ii, rhi);
		}
	}
}

template <typename T> void IntervalTest::dofloatu() {
//	std::cout << "TEST FUNC: " << T::describe() << std::endl;
	for (interval a(-10, -9.91); a.lo() < 10; a += 0.3) {
		if (!T::is_good(a)) continue;
		interval i = T::fi(a);
		interval rl = T::fr(a.lo()), rh = T::fr(a.hi()), rm = T::fr(midpoint(a).lo());
		ENSURE(i.has(rl), i, rl);
		ENSURE(i.has(rh), i, rh);
		ENSURELAST(i.has(rm), i, rm);
	}
}
#define BINOP(NAME, OP, GOOD) \
struct binop_##NAME { \
	static std::string describe() { return #NAME; } \
	static interval ii(interval const &a, interval const &b) { return a OP b; } \
	static interval ir(interval const &a, real b) { return a OP b; } \
	static interval ri(real a, interval const &b) { return a OP b; } \
	static interval rr(real a, real b) { return a OP b; } \
	static bool is_good(interval const &a, interval const &b) { return GOOD; } \
};

#define FUN(NAME, F, GOOD) \
struct fun_##NAME { \
	static std::string describe() { return #NAME; } \
	static interval fi(interval const &iv) { return F(iv); } \
	static interval fr(real r) { return interval(F(r)); } \
	static bool is_good(interval const &iv) { return GOOD; } \
};

BINOP(add, +, true)
BINOP(sub, -, true)
BINOP(mul, *, true)
BINOP(div, /, !b.has_zero())

FUN(negate, -, true)
FUN(sqrt, sqrt, iv.lo() >= real_zero)
FUN(abs, abs, true)

void IntervalTest::floats() {
	dofloatu<fun_negate>();
	dofloatu<fun_sqrt>();
	dofloatu<fun_abs>();

	dofloatb<binop_add>();
	dofloatb<binop_sub>();
	dofloatb<binop_mul>();
	dofloatb<binop_div>();
//	Check(interval(0,1.225).has(interval(1.225)));
}


}