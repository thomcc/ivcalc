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


}


// rounding mode manipulation: called from jitted code.
//void set_rounding_mode_near(void) { ::calc::rmath::set_rnear(); }
//void set_rounding_mode_tozr(void) { ::calc::rmath::set_rzero(); }