#ifndef __ROUNDING_HH__
#define __ROUNDING_HH__

#include "common.hh"
#include <cfenv>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <limits>

#ifndef X86_ROUND
# if defined(__GNUC__) && defined(__i386__) && !defined(C99_ROUND)
// X86_ROUND requires gcc-style inline asm and an x86 machine
#  define X86_ROUND 1
# else
#  define X86_ROUND 0
# endif
#else
# define X86_ROUND 0
#endif


namespace calc {

//typedef long double real;
//#define REAL_FMT "%Lf"
typedef double real;
#define REAL_FMT "%lf"
#define real_zero (static_cast<real>(0))
#define real_one (static_cast<real>(1))
#define real_neg_one (static_cast<real>(-1))

namespace rmath {

// todo, figure out a way to do this without rounding twice
// (once in compiler, once in code)

static const real pi_l = 3.14159265358979323829596852490908531763125210004425048828125L;
static const real pi_h = 3.141592653589793238729649393903287091234233203524017333984375L;

static inline real pi_lo() { return pi_l; }
static inline real pi_hi() { return pi_h; }
static inline real pi_over_two_lo() { return pi_l / 2.0l; }
static inline real pi_over_two_hi() { return pi_h / 2.0l; }
static inline real pi_over_four_lo() { return pi_l / 4.0l; }
static inline real pi_over_four_hi() { return pi_h / 4.0l; }
static inline real pi_times_two_lo() { return pi_l * 2.0l; }
static inline real pi_times_two_hi() { return pi_h * 2.0l; }


// utilities for obtaining extema and testing numbers
static inline real neg_inf() { return -std::numeric_limits<real>::infinity(); }
static inline real pos_inf() { return std::numeric_limits<real>::infinity(); }
static inline real NaN() { return std::numeric_limits<real>::quiet_NaN(); }
static inline bool is_pos_inf(real r) { return r == pos_inf(); }
static inline bool is_neg_inf(real r) { return r == neg_inf(); }
static inline bool is_nan(real r) { return std::isnan(r); }
#if X86_ROUND
// use x86 rounding modes.  clang seems to be less
// eager to optimize around this, which is beneficial
// in this case.
#define ROUND_NEAR static_cast<unsigned short>(0x127f)
#define ROUND_DOWN static_cast<unsigned short>(0x167f)
#define ROUND_UP   static_cast<unsigned short>(0x1a7f)
#define ROUND_ZERO static_cast<unsigned short>(0x1e7f)
static inline void set_round(unsigned short mode) {
	__asm__ __volatile__("fldcw %0" : : "m"(mode));
}
static inline real make_int(real d) {
	real dd;
	__asm__ ("frndint" : "=&t"(dd) : "0"(d));
	return dd;
}
static inline real do_round(real r) { return r; }
#else
// using c99 rounding modes, which should be supported on any
//
#define ROUND_NEAR FE_TONEAREST
#define ROUND_DOWN FE_DOWNWARD
#define ROUND_UP FE_UPWARD
#define ROUND_ZERO FE_TOWARDZERO
static inline void set_round(int mode) { std::fesetround(mode); }
static inline real make_int(real x) { return std::rint(x); }
static inline real do_round(real d) { volatile real dd = d; return dd; }
#endif
// manipulating the rounding mode
static inline void set_rup()   { set_round(ROUND_UP);   }
static inline void set_rdown() { set_round(ROUND_DOWN); }
static inline void set_rnear() { set_round(ROUND_NEAR); }
static inline void set_rzero() { set_round(ROUND_ZERO); }


// force rounding despite the compiler's objections

template <typename T> static inline real r_up(T const &n)   { set_rup();   return do_round(n); }
template <typename T> static inline real r_down(T const &n) { set_rdown(); return do_round(n); }
template <typename T> static inline real r_near(T const &n) { set_rnear(); return do_round(n); }
template <typename T> static inline real r_zero(T const &n) { set_rzero(); return do_round(n); }


static inline real int_hi(real r) { set_rup();   return std::rint(r); }
static inline real int_lo(real r) { set_rdown(); return std::rint(r); }
static inline real int_nr(real r) { set_rnear(); return std::rint(r); }
static inline real int_zr(real r) { set_rzero(); return std::rint(r); }

// arithmetic with a percise rounding mode.
static inline real add_hi(real a, real b) { set_rup();   return do_round(a+b); }
static inline real add_lo(real a, real b) { set_rdown(); return do_round(a+b); }
static inline real add_nr(real a, real b) { set_rnear(); return do_round(a+b); }
static inline real add_zr(real a, real b) { set_rzero(); return do_round(a+b); }

static inline real sub_hi(real a, real b) { set_rup();   return do_round(a-b); }
static inline real sub_lo(real a, real b) { set_rdown(); return do_round(a-b); }
static inline real sub_nr(real a, real b) { set_rnear(); return do_round(a-b); }
static inline real sub_zr(real a, real b) { set_rzero(); return do_round(a-b); }

static inline real mul_hi(real a, real b) { set_rup();   return do_round(a*b); }
static inline real mul_lo(real a, real b) { set_rdown(); return do_round(a*b); }
static inline real mul_nr(real a, real b) { set_rnear(); return do_round(a*b); }
static inline real mul_zr(real a, real b) { set_rzero(); return do_round(a*b); }

static inline real div_hi(real a, real b) { set_rup();   return do_round(a/b); }
static inline real div_lo(real a, real b) { set_rdown(); return do_round(a/b); }
static inline real div_nr(real a, real b) { set_rnear(); return do_round(a/b); }
static inline real div_zr(real a, real b) { set_rzero(); return do_round(a/b); }

static inline real exp_hi(real x) { return is_neg_inf(x) ? real_zero : (is_pos_inf(x) ? x : (set_rup(),   std::max(real_zero, do_round(std::exp(x))))); }
static inline real exp_lo(real x) { return is_neg_inf(x) ? real_zero : (is_pos_inf(x) ? x : (set_rdown(), std::max(real_zero, do_round(std::exp(x))))); }
static inline real exp_nr(real x) { return is_neg_inf(x) ? real_zero : (is_pos_inf(x) ? x : (set_rnear(), std::max(real_zero, do_round(std::exp(x))))); }
static inline real exp_zr(real x) { return is_neg_inf(x) ? real_zero : (is_pos_inf(x) ? x : (set_rzero(), std::max(real_zero, do_round(std::exp(x))))); }

static inline real log_hi(real x) { return (x < 0) ? NaN() : (is_pos_inf(x) ? x : (set_rup(),   do_round(std::log(x)))); }
static inline real log_lo(real x) { return (x < 0) ? NaN() : (is_pos_inf(x) ? x : (set_rdown(), do_round(std::log(x)))); }
static inline real log_nr(real x) { return (x < 0) ? NaN() : (is_pos_inf(x) ? x : (set_rnear(), do_round(std::log(x)))); }
static inline real log_zr(real x) { return (x < 0) ? NaN() : (is_pos_inf(x) ? x : (set_rzero(), do_round(std::log(x)))); }

static inline real avg(real l, real h) { set_rnear(); return do_round((l + h) / 2.0l); }

#define REAL_FUNC(NAME) \
static inline real NAME##_hi(real x) { set_rup();   return do_round(std :: NAME(x)); } \
static inline real NAME##_lo(real x) { set_rdown(); return do_round(std :: NAME(x)); } \
static inline real NAME##_nr(real x) { set_rnear(); return do_round(std :: NAME(x)); } \
static inline real NAME##_zr(real x) { set_rzero(); return do_round(std :: NAME(x)); }

REAL_FUNC(sin)
REAL_FUNC(cos)
REAL_FUNC(tan)
REAL_FUNC(asin)
REAL_FUNC(acos)
REAL_FUNC(atan)
REAL_FUNC(sinh)
REAL_FUNC(cosh)
REAL_FUNC(tanh)
REAL_FUNC(asinh)
REAL_FUNC(acosh)
REAL_FUNC(atanh)
REAL_FUNC(sqrt)

#undef REAL_FUNC
/*
// only call with -1/4 <= x < 1/4
static inline real sin2pi_lo(real x) {
	set_rdown();
	real n = do_round(pi_times_two_lo() * x);
	return do_round(std::sin(n));
}

static inline real sin2pi_hi(real x) {
	set_rup();
	real n = do_round(pi_times_two_hi() * x);
	return do_round(std::sin(n));
}

static inline real cos2pi_lo(real x) {
	set_rdown();
	real n = do_round(pi_times_two_lo() * x);
	return do_round(std::cos(n));
}

static inline real cos2pi_hi(real x) {
	set_rup();
	real n = do_round(pi_times_two_hi() * x);
	return do_round(std::cos(n));
}


static inline real tan2pi_lo(real x) {
	set_rdown();
	real n = do_round(pi_times_two_lo() * x);
	return do_round(std::tan(n));
}

static inline real tan2pi_hi(real x) {
	set_rup();
	real n = do_round(pi_times_two_hi() * x);
	return do_round(std::tan(n));
}

// assumes 0 <= x <= 1
// computes asin(x) / 2*pi
static inline real asin2pi_lo(real x) { return div_lo(std::asin(x), r_up(pi_times_two_hi())); }
static inline real asin2pi_hi(real x) { return div_hi(std::asin(x), r_down(pi_times_two_lo())); }
static inline real acos2pi_lo(real x) { return div_lo(std::acos(x), r_up(pi_times_two_hi())); }
static inline real acos2pi_hi(real x) { return div_hi(std::acos(x), r_down(pi_times_two_lo())); }
static inline real atan2pi_lo(real x) { return div_lo(std::atan(x), r_up(pi_times_two_hi())); }
static inline real atan2pi_hi(real x) { return div_hi(std::atan(x), r_down(pi_times_two_lo())); }
*/
// assumes x > 0.
// computes x ** y via e^(y * log(x))
static inline real pow_hi(real x, real y) {
	if (x < real_zero) return NaN();
	if (x == real_zero) return x;
	if (y > real_zero) {
		if (x == real_one) return x;
		if (x < real_one) return exp_lo(mul_lo(y, log_hi(x)));
		return exp_lo(mul_lo(y, log_lo(x)));
	}
	if ((y == real_zero) || (x == real_one)) return real_one;
	if (x > real_one) return exp_lo(mul_lo(y, log_hi(x)));
	return exp_lo(mul_lo(y, log_lo(x)));
}

static inline real pow_lo(real x, real y) {
	if (x < real_zero) return NaN();
	if (x == real_zero) return x;
	if (y > real_zero) {
		if (x == real_one) return x;
		if (x < real_one) return exp_hi(mul_hi(y, log_lo(x)));
		return exp_hi(mul_hi(y, log_hi(x)));
	}
	if ((y == real_zero) || (x == real_one)) return real_one;
	if (x > real_one) return exp_lo(mul_hi(y, log_lo(x)));
	return exp_lo(mul_hi(y, log_hi(x)));
}


} // rmath



} // calc






#endif