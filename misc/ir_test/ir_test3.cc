#include <cfenv>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <limits>
#include <algorithm> // std::max / std::min
#include <xmmintrin.h>

#define ROUND_UP FE_UPWARD
#define ROUND_DOWN FE_DOWNWARD
#define ROUND_NEAR FE_TONEAREST
#define ROUND_ZERO FE_TOWARDZERO

#define SET_ROUND(X) std::fesetround(X)

namespace calc {
typedef double real;

#define real_zero (static_cast<real>(0))
#define real_one (static_cast<real>(1))
#define real_neg_one (static_cast<real>(-1))

namespace rmath {

// todo, figure out a way to do this without rounding twice
// (once in compiler, once in code)

static const real pi_l = (3373259426.0 + 273688.0 / (1<<21)) / (1<<30);
static const real pi_h = (3373259426.0 + 273689.0 / (1<<21)) / (1<<30);

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

static inline real make_int(real x) { return std::rint(x); }
static inline real do_round(real d) { volatile real dd = d; return dd; }

// manipulating the rounding mode
static inline void set_rup()   { SET_ROUND(ROUND_UP); }
static inline void set_rdown() { SET_ROUND(ROUND_DOWN); }
static inline void set_rnear() { SET_ROUND(ROUND_NEAR); }
static inline void set_rzero() { SET_ROUND(ROUND_ZERO); }


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




extern "C" {

struct pod_interval {
	calc::real lo, hi;
};

pod_interval iv_make_empty(void);
pod_interval iv_make_zero(void);
pod_interval iv_make_one(void);
pod_interval iv_make_full(void);
pod_interval iv_make_pi(void);
pod_interval iv_make_half_pi(void);
pod_interval iv_make_two_pi(void);
pod_interval iv_make(calc::real x, calc::real y);
bool iv_is_empty(pod_interval a);
bool iv_is_zero(pod_interval a);
pod_interval iv_neg(pod_interval a);
pod_interval iv_add(pod_interval a, pod_interval b);
pod_interval iv_sub(pod_interval a, pod_interval b);
pod_interval iv_mul(pod_interval a, pod_interval b);
pod_interval iv_invert(pod_interval a);
pod_interval iv_div(pod_interval a, pod_interval b);
pod_interval iv_mod(pod_interval a, pod_interval b);
pod_interval iv_pow(pod_interval a, int b);

pod_interval iv_sqrt(pod_interval x);
pod_interval iv_exp(pod_interval x);
pod_interval iv_log(pod_interval x);

pod_interval iv_sin(pod_interval x);
pod_interval iv_cos(pod_interval x);
pod_interval iv_tan(pod_interval x);
pod_interval iv_sec(pod_interval x);
pod_interval iv_csc(pod_interval x);
pod_interval iv_cot(pod_interval x);

pod_interval iv_asin(pod_interval x);
pod_interval iv_acos(pod_interval x);
pod_interval iv_atan(pod_interval x);
pod_interval iv_asec(pod_interval x);
pod_interval iv_acsc(pod_interval x);
pod_interval iv_acot(pod_interval x);

pod_interval iv_sinh(pod_interval x);
pod_interval iv_cosh(pod_interval x);
pod_interval iv_tanh(pod_interval x);
pod_interval iv_sech(pod_interval x);
pod_interval iv_csch(pod_interval x);
pod_interval iv_coth(pod_interval x);

pod_interval iv_asinh(pod_interval x);
pod_interval iv_acosh(pod_interval x);
pod_interval iv_atanh(pod_interval x);
pod_interval iv_asech(pod_interval x);
pod_interval iv_acsch(pod_interval x);
pod_interval iv_acoth(pod_interval x);

void set_rounding_mode_up(void);
void set_rounding_mode_down(void);

calc::real force_rounding(calc::real d);

}





#define USING_RMATH(fun) using calc::rmath::fun##_lo; using calc::rmath::fun##_hi
#define USING_RMATH_EXTREMA() using calc::rmath::neg_inf; using calc::rmath::pos_inf; using calc::rmath::NaN

pod_interval iv_make_empty(void) {
	return pod_interval{calc::rmath::NaN(), calc::rmath::NaN()};
}

pod_interval iv_make_zero(void) {
	return pod_interval{0, 0};
}

pod_interval iv_make_one(void) {
	return pod_interval{1, 1};
}

pod_interval iv_make_full(void) {
	return pod_interval{calc::rmath::neg_inf(), calc::rmath::pos_inf()};
}

pod_interval iv_make_pi(void) {
	return pod_interval{calc::rmath::pi_lo(), calc::rmath::pi_hi()};
}

pod_interval iv_make_half_pi(void) {
	return pod_interval{calc::rmath::pi_over_two_lo(), calc::rmath::pi_over_two_hi()};
}

pod_interval iv_make_two_pi(void) {
	return pod_interval{calc::rmath::pi_times_two_lo(), calc::rmath::pi_times_two_hi()};
}

pod_interval iv_make(calc::real x, calc::real y) {
	return pod_interval{x, y};
}

bool iv_is_empty(pod_interval a) {
	return std::isnan(a.lo) || std::isnan(a.hi);
}

bool iv_is_zero(pod_interval a) {
	return a.lo == 0 && a.hi == 0;
}

pod_interval iv_neg(pod_interval a) {
	return pod_interval{-a.hi, -a.lo};
}

pod_interval iv_add(pod_interval a, pod_interval b) {
	USING_RMATH(add);
	return pod_interval{add_lo(a.lo, b.lo), add_hi(a.hi, b.hi)};
}

pod_interval iv_sub(pod_interval a, pod_interval b) {
	USING_RMATH(sub);
	return pod_interval{sub_lo(a.lo, b.hi), sub_hi(a.hi, b.lo)};
}

pod_interval iv_mul(pod_interval a, pod_interval b) {
	USING_RMATH(mul);
	if (a.lo < 0.0) {
		if (a.hi > 0.0) { // `this` straddles 0 (mid)
			if (b.lo < 0.0) {
				if (b.hi > 0.0) { // mid * mid = [min(x_*y^, x^*y_), max(x_*y_, x^*y^)]
					return pod_interval{
						std::min(mul_lo(a.lo, b.hi), mul_lo(a.hi, b.lo)),
						std::max(mul_hi(a.lo, b.lo), mul_hi(a.hi, b.hi))
					};
				} else { // mid * neg = [x^ * y_, x_ * y_]
					return pod_interval{mul_lo(a.hi, b.lo), mul_hi(a.lo, b.lo)};
				}
			} else {
				if (b.hi > 0.0) { // mid * pos = [x_ * y^, x^ * y^]
					return pod_interval{mul_lo(a.lo, b.hi), mul_hi(a.hi, b.hi)};
				} else { // mid * zero = zero
					return pod_interval{0.0, 0.0};
				}
			}
		} else { // `this` is negative
			if (b.lo < 0.0) {
				if (b.hi > 0.0) { // neg * mid = [x_ * y^, x_ * y_]
					return pod_interval{mul_lo(a.lo, b.hi),	mul_hi(a.lo, b.lo)};
				} else { // neg * neg = [x^*y^, x_*y_]
					return pod_interval{mul_lo(a.hi, b.hi), mul_hi(a.lo, b.lo)};
				}
			} else {
				if (b.hi > 0.0) { // neg * pos = [x_ * y^, x^ * y_]
					return pod_interval{mul_lo(a.lo, b.hi), mul_hi(a.hi, b.lo)};
				} else { // neg * zero = zero
					return pod_interval{0.0, 0.0};
				}
			}
		}
	} else { // `this` is geq 0
		if (a.hi > 0.0) { // `this` is positive
			if (b.lo < 0.0) {
				if (b.hi > 0.0) { // pos * mid = [x^*y_, x^*y^]
					return pod_interval{mul_lo(a.hi, b.lo), mul_hi(a.hi, b.hi)};
				} else { // pos * neg = [x^*y_, x_*y^]
					return pod_interval{mul_lo(a.hi, b.lo), mul_hi(a.lo, b.hi)};
				}
			} else {
				if (b.hi > 0.0) { // pos * pos = [x_*y_, x^*y^]
					return pod_interval{mul_lo(a.lo, b.lo), mul_hi(a.hi, b.hi)};
				} else { // pos * zero
					return pod_interval{0.0, 0.0};
				}
			}
		} else { // zero * anything = zero
			return pod_interval{0.0, 0.0};
		}
	}
}

pod_interval iv_invert(pod_interval a) {
	USING_RMATH(div); USING_RMATH_EXTREMA();
	if (a.lo <= 0 && a.hi >= 0) {
		if (a.lo != 0) {
			if (a.hi != 0) return iv_make_full();
			return pod_interval{neg_inf(), div_hi(1, a.lo)};
		} else if (a.hi != 0) return pod_interval{div_lo(1.0, a.hi), pos_inf()};
		return iv_make_empty();
	} else return pod_interval{div_lo(1.0, a.hi), div_hi(1.0, a.lo)};
}

pod_interval iv_div(pod_interval a, pod_interval b) {
	USING_RMATH(div); USING_RMATH_EXTREMA();
	calc::real al = a.lo, bl = b.lo, ah = a.hi, bh = b.hi;
	if (b.lo <= 0 && b.hi >= 0) {
		if (bl != 0.0) {
			if (a.lo == 0 && a.hi == 0) return a;
			if (bh != 0.0) return pod_interval{neg_inf(), pos_inf()};
			if (ah < 0.0) return pod_interval{div_lo(al, bl), pos_inf()};
			if (al < 0.0) return pod_interval{neg_inf(), pos_inf()};
			return pod_interval{neg_inf(), div_hi(al, bl)};
		} else if (bh != 0.0) {
			if (a.lo == 0 && a.hi == 0) return a;
			if (ah < 0.0) return pod_interval{neg_inf(), div_hi(ah, bh)};
			if (al < 0.0) return pod_interval{neg_inf(), pos_inf()};
			return pod_interval{div_lo(al, bh), pos_inf()};
		} else return pod_interval{NaN(), NaN()};
	} else if (ah < 0.0) {
		if (bh < 0.0)
			return pod_interval{div_lo(ah, bl), div_hi(al, bh)};
		else return pod_interval{div_lo(al, bl), div_hi(ah, bh)};
	} else if (al < 0.0) {
		if (bh < 0.0)
			return pod_interval{div_lo(ah, bh), div_hi(al, bh)};
		else return pod_interval{div_lo(al, bl), div_hi(ah, bl)};
	} else {
		if (bh < 0.0)
			return pod_interval{div_lo(ah, bh), div_hi(al, bl)};
		else return pod_interval{div_lo(al, bh), div_hi(ah, bl)};
	}
}

pod_interval iv_mod(pod_interval a, pod_interval b) {
	USING_RMATH(int); USING_RMATH(div);
	calc::real r = int_lo(div_lo(a.lo, (a.lo < 0) ? b.lo : b.hi));
	return iv_sub(a, iv_mul(pod_interval{r, r}, b));
}

pod_interval iv_pow(pod_interval a, int b) {
	USING_RMATH(mul); USING_RMATH_EXTREMA();
	if (b == 0) return iv_is_zero(a) ? iv_make_empty() : iv_make_one();
	if (b < 0) return iv_invert(iv_pow(a, -b));
	if (a.hi < 0) {
		if (b & 1) {
			calc::real lo = a.hi, lo_y = a.hi;
			calc::real hi = -a.lo, hi_y = a.lo;
			int p = b >> 1;
			while (p > 0) {
				lo = mul_lo(lo, lo);
				if (p & 1) lo_y = mul_lo(lo, lo_y);
				hi = mul_hi(hi, hi);
				if (p & 1) hi_y = mul_hi(hi, hi_y);
				p >>= 1;
			}
			return pod_interval{-lo_y, hi_y};
		}
		calc::real x = std::max(-a.lo, a.hi);
		calc::real y = 1;
		int p = b >> 1;
		while (p > 0) {
			x = mul_hi(x, x);
			if (p & 1) y = mul_hi(x, y);
			p >>= 1;
		}
		return pod_interval{0.0, y};
	}
	calc::real lo = a.lo, lo_y = (b & 1) ? a.lo : 1.0;
	calc::real hi = a.hi, hi_y = (b & 1) ? a.hi : 1.0;
	int p = b >> 1;
	while (p > 0) {
		lo = mul_lo(lo, lo);
		if (p & 1) lo_y = mul_lo(lo, lo_y);
		hi = mul_hi(hi, hi);
		if (p & 1) hi_y = mul_hi(hi, hi_y);
		p >>= 1;
	}
	return pod_interval{lo_y, hi_y};
}

pod_interval iv_sqrt(pod_interval x) {
	USING_RMATH(sqrt);
	if (x.hi < 0.0) return iv_make_empty();
	return pod_interval{(x.lo < 0.0 ? 0.0 : sqrt_lo(x.lo)), sqrt_hi(x.hi)};
}

pod_interval iv_exp(pod_interval x) {
	USING_RMATH(exp);
	return pod_interval{exp_lo(x.lo), exp_hi(x.hi)};
}

pod_interval iv_log(pod_interval x) {
	USING_RMATH(log); USING_RMATH_EXTREMA();
	if (x.hi < 0) return iv_make_empty();
	return pod_interval{(x.lo < 0 ? neg_inf() : log_lo(x.lo)), log_hi(x.hi)};
}

pod_interval iv_cos(pod_interval x) {
	USING_RMATH(cos); USING_RMATH(sub); USING_RMATH(pi); USING_RMATH(pi_times_two);
	pod_interval ps = iv_make_two_pi();
	pod_interval t = iv_mod(x, ps);
	if (std::abs(sub_hi(t.lo, t.hi)) >= pi_hi())
		return iv_neg(iv_cos(iv_sub(t, pod_interval{pi_lo(), pi_hi()})));
	calc::real lo = t.lo, hi = t.hi;
	if (hi <= pi_lo()) return pod_interval{cos_lo(hi), cos_hi(lo)};
	if (hi < ps.lo) return pod_interval{-1, cos_hi(std::min(sub_lo(ps.lo, hi), lo))};
	return pod_interval{-1, 1};
}

pod_interval iv_sin(pod_interval x) {
	USING_RMATH(pi_over_two);
	return iv_cos(iv_sub(x, pod_interval{pi_over_two_lo(), pi_over_two_hi()}));
}


pod_interval iv_tan(pod_interval x) {
	USING_RMATH(tan);
	pod_interval pi = iv_make_pi();
	pod_interval t = iv_mod(x, pi);
	calc::real po2l = calc::rmath::pi_over_two_lo();
	if (t.lo >= po2l) t = iv_sub(t, pi);
	if (t.lo <= -po2l || pi.hi >= po2l) return iv_make_full();
	return pod_interval{tan_lo(t.lo), tan_hi(t.hi)};
}

pod_interval iv_asin(pod_interval x) {
	USING_RMATH(asin); USING_RMATH(pi_over_two);
	if (x.hi < -1 || x.lo > 1) return iv_make_empty();
	return pod_interval{
		(x.lo <= -1 ? pi_over_two_hi() : asin_lo(x.lo)),
		(x.hi >= +1 ? pi_over_two_hi() : asin_hi(x.hi))
	};
}

pod_interval iv_acos(pod_interval x) {
	USING_RMATH(acos); USING_RMATH(pi);
	if (x.lo < -1 || x.hi > 1) return iv_make_empty();
	return pod_interval{(x.hi >= 1 ? 0 : acos_lo(x.hi)), (x.lo <= -1 ? pi_hi() : acos_hi(x.lo))};
}

pod_interval iv_atan(pod_interval x) {
	USING_RMATH(atan);
	return pod_interval{atan_lo(x.lo), atan_hi(x.hi)};
}

pod_interval iv_sinh(pod_interval x) {
	USING_RMATH(sinh);
	return pod_interval{sinh_lo(x.lo), sinh_hi(x.hi)};
}

pod_interval iv_cosh(pod_interval x) {
	USING_RMATH(cosh);
	if (x.hi < 0) return pod_interval{cosh_lo(x.hi), cosh_hi(x.lo)};
	if (x.lo > 0) return pod_interval{cosh_lo(x.lo), cosh_hi(x.hi)};
	return pod_interval{1, cosh_hi(-x.lo > x.hi ? x.lo : x.hi)};
}

pod_interval iv_tanh(pod_interval x) {
	USING_RMATH(tanh);
	return pod_interval{tanh_lo(x.lo), tanh_hi(x.hi)};
}

pod_interval iv_asinh(pod_interval x) {
	USING_RMATH(asinh);
	return pod_interval{asinh_lo(x.lo), asinh_hi(x.hi)};
}

pod_interval iv_acosh(pod_interval x) {
	USING_RMATH(acosh);
	if (x.hi > 1) return iv_make_empty();
	return pod_interval{x.lo <= 1 ? 0 : acosh_lo(x.lo), acosh_hi(x.hi)};
}

pod_interval iv_atanh(pod_interval x) {
	USING_RMATH(atanh); USING_RMATH_EXTREMA();
	if (x.hi < -1 || x.lo > 1) return iv_make_empty();
	return pod_interval{
		x.lo <= -1 ? neg_inf() : atanh_lo(x.lo),
		x.hi >= +1 ? pos_inf() : atanh_hi(x.hi)
	};
}

// for completeness.
pod_interval iv_sec(pod_interval x) { return iv_invert(iv_cos(x)); }
pod_interval iv_csc(pod_interval x) { return iv_invert(iv_sin(x)); }
pod_interval iv_cot(pod_interval x) { return iv_invert(iv_tan(x)); }
pod_interval iv_asec(pod_interval x) { return iv_acos(iv_invert(x)); }
pod_interval iv_acsc(pod_interval x) { return iv_asin(iv_invert(x)); }
pod_interval iv_acot(pod_interval x) { return iv_acot(iv_invert(x)); }
pod_interval iv_sech(pod_interval x) { return iv_invert(iv_cosh(x)); }
pod_interval iv_csch(pod_interval x) { return iv_invert(iv_sinh(x)); }
pod_interval iv_coth(pod_interval x) { return iv_div(iv_cosh(x), iv_sinh(x)); }
pod_interval iv_asech(pod_interval x) { return iv_acosh(iv_invert(x)); }
pod_interval iv_acsch(pod_interval x) { return iv_asinh(iv_invert(x)); }
pod_interval iv_acoth(pod_interval x) { return iv_atanh(iv_invert(x)); }



void set_rounding_mode_up(void) {
	calc::rmath::set_rup();
}

void set_rounding_mode_down(void) {
	calc::rmath::set_rdown();
}

calc::real force_rounding(calc::real d) {
	volatile calc::real r = d;
	return r;
}




extern "C" {
typedef __m128d vec_iv;

vec_iv _vec_iv_raw_make(double x, double y) {
	return (vec_iv){x, y};
}

vec_iv vec_iv_make(double x, double y) {
	return (vec_iv){-x, y};
}

double vec_iv_lo(vec_iv x) { return -x[0]; }
double vec_iv_hi(vec_iv x) { return x[1]; }
double vec_iv_lneg(vec_iv x) { return x[0]; }
vec_iv vec_iv_add(vec_iv x, vec_iv y) { return x + y; }

// swap high and lo parts to negate
vec_iv vec_iv_negate(vec_iv x) { return _mm_shuffle_pd(x, x, 1); }
vec_iv vec_iv_sub(vec_iv x, vec_iv y) { return x + vec_iv_negate(y); }


static inline vec_iv _vec_ivswap(vec_iv x, vec_iv cond) {
	vec_iv t = _mm_xor_pd((vec_iv)_mm_shuffle_epi32((__m128i)x, 0x4e), x);
	return _mm_xor_pd(_mm_and_pd(cond, t), x);
}

vec_iv vec_iv_mul(vec_iv x, vec_iv y) {
	vec_iv t1 = (vec_iv)_mm_shuffle_epi32((__m128i)x, 0xee);
	vec_iv t2 = (vec_iv)_mm_shuffle_epi32((__m128i)y, 0xee);
	vec_iv t3 = (vec_iv)_mm_xor_pd(x, t1);
	vec_iv t4 = (vec_iv)_mm_xor_pd(y, t2);
	if (_mm_movemask_pd(_mm_and_pd(t3, t4))) {
		vec_iv c = {0.0, 0.0};
		vec_iv c1 = _mm_cmple_pd(t2, c);
		vec_iv c2 = _mm_cmple_pd(t1, c);
		vec_iv c3 = (vec_iv){-0.0, 0.0};
		x = _vec_ivswap(_mm_xor_pd(x, c3), c1);
		y = _vec_ivswap(_mm_xor_pd(y, c3), c2);
		return x * _mm_xor_pd(y, c3);
	}
	t1 = (vec_iv)_mm_shuffle_epi32((__m128i)x, 0x43) * _mm_unpacklo_pd(y, y);
	t1 *= x;
	return _mm_max_pd(t1, t2);
}

vec_iv vec_iv_invert(vec_iv x) {
	if (_mm_movemask_pd(_mm_cmple_pd((vec_iv){0.0}, x)) == 3)
		return _vec_iv_raw_make(INFINITY, INFINITY);
	x = _mm_shuffle_pd(x, x, 1);
	return _mm_div_pd((vec_iv){-1, -1}, x);
}

vec_iv vec_iv_div(vec_iv x, vec_iv y) {
	return vec_iv_mul(x, vec_iv_negate(x));
}

vec_iv vec_iv_sqrt(vec_iv x) {
	if (vec_iv_lo(x) < 0) return vec_iv_make(NAN, NAN);
	// counteract unrounded sqrt below (leaves interval slightly
	// larger than necessary, but )
	x *= (vec_iv){-0x1.ffffffffffff8p-1, 1.0};
	x = _mm_sqrt_pd(x);
	return _mm_xor_pd(x, (vec_iv){-0.0, 0.0});
}


pod_interval ivvmul(pod_interval a, pod_interval b) {
	vec_iv viv = vec_iv_mul(vec_iv_make(a.lo, a.hi), vec_iv_make(b.lo, b.hi));
	return iv_make(vec_iv_lo(viv), vec_iv_hi(viv));
}

pod_interval ivvsub(pod_interval a, pod_interval b) {
	vec_iv viv = vec_iv_sub(vec_iv_make(a.lo, a.hi), vec_iv_make(b.lo, b.hi));
	return iv_make(vec_iv_lo(viv), vec_iv_hi(viv));
}

pod_interval ivvadd(pod_interval a, pod_interval b) {
	vec_iv viv = vec_iv_add(vec_iv_make(a.lo, a.hi), vec_iv_make(b.lo, b.hi));
	return iv_make(vec_iv_lo(viv), vec_iv_hi(viv));

}

pod_interval ivvdiv(pod_interval a, pod_interval b) {
	vec_iv viv = vec_iv_div(vec_iv_make(a.lo, a.hi), vec_iv_make(b.lo, b.hi));
	return iv_make(vec_iv_lo(viv), vec_iv_hi(viv));

}








}