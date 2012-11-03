#ifndef __ROUNDING_HH__
#define __ROUNDING_HH__

#include "common.hh"
#include <cfenv>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <limits>
namespace calc {

typedef long double real;

#define real_zero (static_cast<real>(0))
#define real_one (static_cast<real>(1))
#define real_neg_one (static_cast<real>(-1))

namespace rmath {

// todo, figure out a way to do this without rounding twice
// (once in compiler, once in code)

static const real pi_l = 3.14159265358979323829596852490908531763125210004425048828125L;
static const real pi_h = 3.141592653589793238729649393903287091234233203524017333984375L;

static inline real
pi_lo() {
	return pi_l;
}

static inline real
pi_hi() {
	return pi_h;
}

static inline real
pi_over_two_lo() {
	return pi_l / 2.0l;
}

static inline real
pi_over_two_hi() {
	return pi_h / 2.0l;
}

static inline real
pi_over_four_lo() {
	return pi_l / 4.0l;
}

static inline real
pi_over_four_hi() {
	return pi_h / 4.0l;
}

static inline real
pi_times_two_lo() {
	return pi_l * 2.0l;
}

static inline real
pi_times_two_hi() {
	return pi_h * 2.0l;
}



// utilities for obtaining extema and testing numbers
static inline real
neg_inf() {
	return -std::numeric_limits<real>::infinity();
}

static inline real
pos_inf() {
	return std::numeric_limits<real>::infinity();
}

static inline real
NaN() {
	return std::numeric_limits<real>::quiet_NaN();
}

static inline bool
is_pos_inf(real r) {
	return r == pos_inf();
}

static inline bool
is_neg_inf(real r) {
	return r == neg_inf();
}

static inline bool
is_nan(real r) {
	return r != r;
}

// manipulating the rounding mode

static inline void
set_rup() {
	std::fesetround(FE_UPWARD);
}

static inline void
set_rdown() {
	std::fesetround(FE_DOWNWARD);
}

static inline void
set_rnear() {
	std::fesetround(FE_TONEAREST);
}

static inline void
set_rzero() {
	std::fesetround(FE_TOWARDZERO);
}

// force rounding despite the compiler's objections
static inline real
do_round(real d) {
	volatile real dd = d;
	return dd;
}

template <typename T>
static inline real
r_up(T const &n) {
	set_rup();
	return do_round(n);
}

template <typename T>
static inline real
r_down(T const &n) {
	set_rdown();
	return do_round(n);
}

// unsure if i'll need to manipulate the rounding
// mode here in the future.
template <typename T>
static inline T
make_int(T const &x) {
	return std::rint(x);
}

// arithmetic with a percise rounding mode.
static inline real
add_hi(real a, real b) {
	set_rup();
	return do_round(a+b);
}

static inline real
add_lo(real a, real b) {
	set_rdown();
	return do_round(a+b);
}

static inline real
add_nr(real a, real b) {
	set_rnear();
	return do_round(a+b);
}

static inline real
sub_hi(real a, real b) {
	set_rup();
	return do_round(a-b);
}

static inline real
sub_lo(real a, real b) {
	set_rdown();
	return do_round(a-b);
}

static inline real
sub_nr(real a, real b) {
	set_rdown();
	return do_round(a-b);
}

static inline real
mul_hi(real a, real b) {
	set_rup();
	return do_round(a*b);
}

static inline real
mul_lo(real a, real b) {
	set_rdown();
	return do_round(a*b);
}

static inline real
mul_nr(real a, real b) {
	set_rnear();
	return do_round(a*b);
}

static inline real
div_hi(real a, real b) {
	set_rup();
	return do_round(a/b);
}

static inline real
div_lo(real a, real b) {
	set_rdown();
	return do_round(a/b);
}

static inline real
exp_lo(real x) {
	if (x == neg_inf()) return real_zero;
	else if (x == pos_inf()) return x;
	set_rdown();
	return std::max(real_zero, do_round(expl(x)));
}

static inline real
exp_hi(real x) {
	if (x == neg_inf()) return real_zero;
	else if (x == pos_inf()) return x;
	set_rup();
	return std::max(real_zero, do_round(expl(x)));
}

static inline real
log_lo(real x) {
	if (x < real_zero) return NaN();
	else if (is_pos_inf(x)) return x;
	set_rdown();
	return do_round(logl(x));
}

static inline real
log_hi(real x) {
	if (x < real_zero) return NaN();
	else if (is_pos_inf(x)) return x;
	set_rup();
	return do_round(logl(x));
}


static inline real
avg(real l, real h) {
	set_rnear();
	return do_round((l + h) / 2.0l);
}

#define HI_LO_FUNC(NAME)         \
static inline real               \
NAME##_lo(real x) {              \
	set_rdown();                 \
	return do_round(NAME##l(x)); \
}                                \
static inline real               \
NAME##_hi(real x) {              \
	set_rup();                   \
	return do_round(NAME##l(x)); \
}

HI_LO_FUNC(sin)
HI_LO_FUNC(cos)
HI_LO_FUNC(tan)
HI_LO_FUNC(asin)
HI_LO_FUNC(acos)
HI_LO_FUNC(atan)
HI_LO_FUNC(sinh)
HI_LO_FUNC(cosh)
HI_LO_FUNC(tanh)
HI_LO_FUNC(asinh)
HI_LO_FUNC(acosh)
HI_LO_FUNC(atanh)
HI_LO_FUNC(sqrt)

#undef HI_LO_FUNC

// only call with -1/4 <= x < 1/4
static inline real
sin2pi_lo(real x) {
	set_rdown();
	real n = do_round(pi_times_two_lo() * x);
	return do_round(sinl(n));
}

static inline real
sin2pi_hi(real x) {
	set_rup();
	real n = do_round(pi_times_two_hi() * x);
	return do_round(sinl(n));
}

static inline real
cos2pi_lo(real x) {
	set_rdown();
	real n = do_round(pi_times_two_lo() * x);
	return do_round(cosl(n));
}

static inline real
cos2pi_hi(real x) {
	set_rup();
	real n = do_round(pi_times_two_hi() * x);
	return do_round(cosl(n));
}


static inline real
tan2pi_lo(real x) {
	set_rdown();
	real n = do_round(pi_times_two_lo() * x);
	return do_round(tanl(n));
}

static inline real
tan2pi_hi(real x) {
	set_rup();
	real n = do_round(pi_times_two_hi() * x);
	return do_round(tanl(n));
}

// assumes 0 <= x <= 1
// computes asin(x) / 2*pi
static inline real
asin2pi_lo(real x) {
	return div_lo(asinl(x), pi_times_two_hi());
}

static inline real
asin2pi_hi(real x) {
	return div_hi(asinl(x), pi_times_two_lo());
}

static inline real
acos2pi_lo(real x) {
	return div_lo(acosl(x), pi_times_two_hi());
}

static inline real
acos2pi_hi(real x) {
	return div_hi(acosl(x), pi_times_two_lo());
}


static inline real
atan2pi_lo(real x) {
	return div_lo(atanl(x), pi_times_two_hi());
}

static inline real
atan2pi_hi(real x) {
	return div_hi(atanl(x), pi_times_two_lo());
}

// assumes x > 0.
// computes x ** y via e^(y * log(x))
static inline real
pow_hi(real x, real y) {
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

static inline real
pow_lo(real x, real y) {
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