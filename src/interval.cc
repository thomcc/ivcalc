#include "interval.hh"
#include <iostream>
namespace calc {

interval &interval::operator*=(interval const &i) {
	if (is_empty()) return *this;
	if (i.is_empty()) return *this = interval::empty();
	real xl = _lo, xh = _hi, yl = i._lo, yh = i._hi;
	if (xl < real_zero) {
		if (xh > real_zero) { // `this` straddles 0 (mid)
			if (yl < real_zero) {
				if (yh > real_zero) { // mid * mid = [min(x_*y^, x^*y_), max(x_*y_, x^*y^)]
					_lo = std::min(rmath::mul_lo(xl, yh), rmath::mul_lo(xh, yl));
					_hi = std::max(rmath::mul_hi(xl, yl), rmath::mul_hi(xh, yh));
				} else { // mid * neg = [x^ * y_, x_ * y_]
					_lo = rmath::mul_lo(xh, yl);
					_hi = rmath::mul_hi(xl, yl);
				}
			} else {
				if (yh > real_zero) { // mid * pos = [x_ * y^, x^ * y^]
					_lo = rmath::mul_lo(xl, yh);
					_hi = rmath::mul_hi(xh, yh);
				} else { // mid * zero = zero
					_lo = _hi = real_zero;
				}
			}
		} else { // `this` is negative
			if (yl < real_zero) {
				if (yh > real_zero) { // neg * mid = [x_ * y^, x_ * y_]
					_lo = rmath::mul_lo(xl, yh);
					_hi = rmath::mul_hi(xl, yl);
				} else { // neg * neg = [x^*y^, x_*y_]
					_lo = rmath::mul_lo(xh, yh);
					_hi = rmath::mul_hi(xl, yl);
				}
			} else {
				if (yh > real_zero) { // neg * pos = [x_ * y^, x^ * y_]
					_lo = rmath::mul_lo(xl, yh);
					_hi = rmath::mul_hi(xh, yl);
				} else { // neg * zero = zero
					_lo = _hi = real_zero;
				}
			}
		}
	} else { // `this` is geq 0
		if (xh > real_zero) { // `this` is positive
			if (yl < real_zero) {
				if (yh > real_zero) { // pos * mid = [x^*y_, x^*y^]
					_lo = rmath::mul_lo(xh, yl);
					_hi = rmath::mul_hi(xh, yh);
				} else { // pos * neg = [x^*y_, x_*y^]
					_lo = rmath::mul_lo(xh, yl);
					_hi = rmath::mul_hi(xl, yh);
				}
			} else {
				if (yh > real_zero) { // pos * pos = [x_*y_, x^*y^]
					_lo = rmath::mul_lo(xl, yl);
					_hi = rmath::mul_hi(xh, yh);
				} else { // pos * zero
					_lo = _hi = real_zero;
				}
			}
		} else { // zero * anything = zero
			_lo = _hi = real_zero;
		}
	}
	return *this;
}

interval operator/(interval const &a, interval const &b) {
	if (a.is_empty() || b.is_empty()) return interval::empty();
	using namespace rmath;
	real al = a.lo(), ah = a.hi(), bl = b.lo(), bh = b.hi();
	if (b.has_zero()) {
		if (bl != real_zero) {
			if (a.is_zero()) return a;
			if (bh != real_zero) return interval::full();
			if (ah < real_zero) return interval(div_lo(al, bl), pos_inf());
			if (al < real_zero) return interval::full();
			return interval(neg_inf(), div_hi(al, bl));
		} else if (bh != real_zero) {
			if (a.is_zero()) return a;
			if (ah < real_zero) return interval(neg_inf(), div_hi(ah, bh));
			if (al < real_zero) return interval::full();
			return interval(div_lo(al, bh), pos_inf());
		} else return interval::empty();
	} else if (ah < real_zero) {
		if (bh < real_zero)
			return interval(div_lo(ah, bl), div_hi(al, bh));
		else return interval(div_lo(al, bl), div_hi(ah, bh));
	} else if (al < real_zero) {
		if (bh < real_zero)
			return interval(div_lo(ah, bh), div_hi(al, bh));
		else return interval(div_lo(al, bl), div_hi(ah, bl));
	} else {
		if (bh < real_zero)
			return interval(div_lo(ah, bh), div_hi(al, bl));
		else return interval(div_lo(al, bh), div_hi(ah, bl));
	}
}

interval modulo(interval const &a, interval const &b) {
	if (a.is_empty() || b.is_empty()) return interval::empty();
	real rr = rmath::int_lo(rmath::div_lo(a.lo(), (a.lo() < 0) ? b.lo() : b.hi()));
	return a - rr * b;
}

interval modulo(interval const &a, real b) {
	if (rmath::is_nan(b) || a.is_empty()) return interval::empty();
	real rr = rmath::int_lo(rmath::div_lo(b, b < 0 ? a.lo() : a.hi()));
	return a - rr * interval(b);
}

interval modulo(real a, interval const &b) {
	if (rmath::is_nan(a) || b.is_empty()) return interval::empty();
	real rr = rmath::int_lo(rmath::div_lo(a, a < 0 ? b.lo() : b.hi()));
	return a - rr * b;
}

interval midpoint(interval const &x) {
	real zz = rmath::avg(x.lo(), x.hi());
	if (isfinite(zz)) return interval(zz, zz);
	if (x.lo() == rmath::neg_inf()) {
		if (x.hi() > real_zero) return interval::zero();
		if (x.hi() == real_zero) return interval::minus_one();
		return interval(rmath::mul_lo(x.hi(), 2.0l), rmath::mul_hi(x.hi(), 2.0l));
	} else if (x.lo() == rmath::pos_inf()) {
		if (x.lo() < real_zero) return interval::zero();
		if (x.lo() == real_zero) return interval::one();
		return interval(rmath::mul_lo(x.lo(), 2.0l), rmath::mul_hi(x.lo(), 2.0l));
	}
	std::cerr << "Error in midpoint(interval) got: " << x << std::endl;
	return interval(x);
}

interval leftendpoint(interval const &x) {
	if (isfinite(x.lo())) return interval(x.lo(), x.lo());
	return interval(rmath::r_down(x.lo()), rmath::r_up(x.lo()));
}

interval rightendpoint(interval const &x) {
	if (isfinite(x.hi())) return interval(x.hi(), x.hi());
	return interval(rmath::r_down(x.hi()), rmath::r_up(x.hi()));
}


static inline real int_pow_lo(real x, int y) {
	real res = (y & 1) ? x : 1;
	y >>= 1;
	while (y > 0) {
		x = rmath::mul_lo(x, x);
		if (y & 1) res = rmath::mul_lo(x, res);
		y >>= 1;
	}
	return res;
}

static inline real int_pow_hi(real x, int y) {
	real res = (y & 1) ? x : 1;
	y >>= 1;
	while (y > 0) {
		x = rmath::mul_hi(x, x);
		if (y & 1) res = rmath::mul_hi(x, res);
		y >>= 1;
	}
	return res;
}




interval int_pow(interval const &x, int y) {
	if (y == 0) return x.is_zero() ? interval::empty() : interval::one();
	if (y < 0) return interval(1) / int_pow(x, -y);
	if (x.hi() < 0) {
		real plo = int_pow_lo(-x.hi(), y), phi = int_pow_hi(-x.lo(), y);
		return (y&1) ? interval(-phi, -plo) : interval(plo, phi);
	} else if (x.lo() < 0) {
		if (y & 1) return interval(-int_pow_hi(x.lo(), y), int_pow_hi(x.hi(), y));
		return interval(real_zero, int_pow_hi(std::max(-x.lo(), x.hi()), y));
	} else return interval(int_pow_lo(x.lo(), y), int_pow_hi(x.hi(), y));
}


} // namespace calc


// these functions are (sometimes) called from jitted code

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
	return a.lo == calc::rmath::NaN() || a.hi == calc::rmath::NaN();
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
