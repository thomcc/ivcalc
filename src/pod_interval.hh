#ifndef __POD_INTERVAL_HH__
#define __POD_INTERVAL_HH__

#include "rounding.hh"

extern "C" {

#define USING_RMATH(fun) using calc::rmath::fun##_lo; using calc::rmath::fun##_hi
#define USING_RMATH_EXTREMA() using calc::rmath::neg_inf; using calc::rmath::pos_inf; using calc::rmath::NaN

struct pod_interval {
	calc::real lo, hi;
};

static inline pod_interval iv_make_empty(void) {
	return pod_interval{calc::rmath::NaN(), calc::rmath::NaN()};
}

static inline pod_interval iv_make_zero(void) {
	return pod_interval{0, 0};
}

static inline pod_interval iv_make_one(void) {
	return pod_interval{1, 1};
}

static inline pod_interval iv_make_full(void) {
	return pod_interval{calc::rmath::neg_inf(), calc::rmath::pos_inf()};
}

static inline pod_interval iv_make_pi(void) {
	return pod_interval{calc::rmath::pi_lo(), calc::rmath::pi_hi()};
}

static inline pod_interval iv_make_half_pi(void) {
	return pod_interval{calc::rmath::pi_over_two_lo(), calc::rmath::pi_over_two_hi()};
}

static inline pod_interval iv_make_two_pi(void) {
	return pod_interval{calc::rmath::pi_times_two_lo(), calc::rmath::pi_times_two_hi()};
}

static inline pod_interval iv_make(calc::real x, calc::real y) {
	return pod_interval{x, y};
}

static inline bool iv_is_empty(pod_interval a) {
	return a.lo == calc::rmath::NaN() || a.hi == calc::rmath::NaN();
}

static inline bool iv_is_zero(pod_interval a) {
	return a.lo == 0 && a.hi == 0;
}

static inline pod_interval iv_neg(pod_interval a) {
	return pod_interval{-a.hi, -a.lo};
}

static inline pod_interval iv_add(pod_interval a, pod_interval b) {
	USING_RMATH(add);
	return pod_interval{add_lo(a.lo, b.lo), add_hi(a.hi, b.hi)};
}

static inline pod_interval iv_sub(pod_interval a, pod_interval b) {
	USING_RMATH(sub);
	return pod_interval{sub_lo(a.lo, b.hi), sub_hi(a.hi, b.lo)};
}

static inline pod_interval iv_mul(pod_interval a, pod_interval b) {
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

static inline pod_interval iv_invert(pod_interval a) {
	USING_RMATH(div); USING_RMATH_EXTREMA();
	if (a.lo <= 0 && a.hi >= 0) {
		if (a.lo != 0) {
			if (a.hi != 0) return iv_make_full();
			return pod_interval{neg_inf(), div_hi(1, a.lo)};
		} else if (a.hi != 0) return pod_interval{div_lo(1.0, a.hi), pos_inf()};
		return iv_make_empty();
	} else return pod_interval{div_lo(1.0, a.hi), div_hi(1.0, a.lo)};
}

static inline pod_interval iv_div(pod_interval a, pod_interval b) {
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

static inline pod_interval iv_mod(pod_interval a, pod_interval b) {
	USING_RMATH(int); USING_RMATH(div);
	calc::real r = int_lo(div_lo(a.lo, (a.lo < 0) ? b.lo : b.hi));
	return iv_sub(a, iv_mul(pod_interval{r, r}, b));
}

static inline pod_interval iv_pow(pod_interval a, int b) {
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

static inline pod_interval iv_sqrt(pod_interval x) {
	USING_RMATH(sqrt);
	if (x.hi < 0.0) return iv_make_empty();
	return pod_interval{(x.lo < 0.0 ? 0.0 : sqrt_lo(x.lo)), sqrt_hi(x.hi)};
}

static inline pod_interval iv_exp(pod_interval x) {
	USING_RMATH(exp);
	return pod_interval{exp_lo(x.lo), exp_hi(x.hi)};
}

static inline pod_interval iv_log(pod_interval x) {
	USING_RMATH(log); USING_RMATH_EXTREMA();
	if (x.hi < 0) return iv_make_empty();
	return pod_interval{(x.lo < 0 ? neg_inf() : log_lo(x.lo)), log_hi(x.hi)};
}

static inline pod_interval iv_cos(pod_interval x) {
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

static inline pod_interval iv_sin(pod_interval x) {
	USING_RMATH(pi_over_two);
	return iv_cos(iv_sub(x, pod_interval{pi_over_two_lo(), pi_over_two_hi()}));
}


static inline pod_interval iv_tan(pod_interval x) {
	USING_RMATH(tan);
	pod_interval pi = iv_make_pi();
	pod_interval t = iv_mod(x, pi);
	calc::real po2l = calc::rmath::pi_over_two_lo();
	if (t.lo >= po2l) t = iv_sub(t, pi);
	if (t.lo <= -po2l || pi.hi >= po2l) return iv_make_full();
	return pod_interval{tan_lo(t.lo), tan_hi(t.hi)};
}

static inline pod_interval iv_asin(pod_interval x) {
	USING_RMATH(asin); USING_RMATH(pi_over_two);
	if (x.hi < -1 || x.lo > 1) return iv_make_empty();
	return pod_interval{
		(x.lo <= -1 ? pi_over_two_hi() : asin_lo(x.lo)),
		(x.hi >= +1 ? pi_over_two_hi() : asin_hi(x.hi))
	};
}

static inline pod_interval iv_acos(pod_interval x) {
	USING_RMATH(acos); USING_RMATH(pi);
	if (x.lo < -1 || x.hi > 1) return iv_make_empty();
	return pod_interval{(x.hi >= 1 ? 0 : acos_lo(x.hi)), (x.lo <= -1 ? pi_hi() : acos_hi(x.lo))};
}

static inline pod_interval iv_atan(pod_interval x) {
	USING_RMATH(atan);
	return pod_interval{atan_lo(x.lo), atan_hi(x.hi)};
}

static inline pod_interval iv_sinh(pod_interval x) {
	USING_RMATH(sinh);
	return pod_interval{sinh_lo(x.lo), sinh_hi(x.hi)};
}

static inline pod_interval iv_cosh(pod_interval x) {
	USING_RMATH(cosh);
	if (x.hi < 0) return pod_interval{cosh_lo(x.hi), cosh_hi(x.lo)};
	if (x.lo > 0) return pod_interval{cosh_lo(x.lo), cosh_hi(x.hi)};
	return pod_interval{1, cosh_hi(-x.lo > x.hi ? x.lo : x.hi)};
}

static inline pod_interval iv_tanh(pod_interval x) {
	USING_RMATH(tanh);
	return pod_interval{tanh_lo(x.lo), tanh_hi(x.hi)};
}

static inline pod_interval iv_asinh(pod_interval x) {
	USING_RMATH(asinh);
	return pod_interval{asinh_lo(x.lo), asinh_hi(x.hi)};
}

static inline pod_interval iv_acosh(pod_interval x) {
	USING_RMATH(acosh);
	if (x.hi > 1) return iv_make_empty();
	return pod_interval{x.lo <= 1 ? 0 : acosh_lo(x.lo), acosh_hi(x.hi)};
}

static inline pod_interval iv_atanh(pod_interval x) {
	USING_RMATH(atanh); USING_RMATH_EXTREMA();
	if (x.hi < -1 || x.lo > 1) return iv_make_empty();
	return pod_interval{
		x.lo <= -1 ? neg_inf() : atanh_lo(x.lo),
		x.hi >= +1 ? pos_inf() : atanh_hi(x.hi)
	};
}

static inline void set_rounding_mode_up(void) {
	calc::rmath::set_rup();
}

static inline void set_rounding_mode_down(void) {
	calc::rmath::set_rdown();
}

static inline calc::real force_rounding(calc::real d) {
	volatile calc::real r = d;
	return r;
}

#undef USING_RMATH_EXTREMA
#undef USING_RMATH

}
#endif