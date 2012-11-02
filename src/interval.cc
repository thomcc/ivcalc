#include "interval.hh"
namespace calc {

interval&
interval::operator*=(interval const &i) {
	if (is_zero() || i.is_zero()) {
		_lo = _hi = real_zero;
		return *this;
	} else if (_lo > real_zero) {
		if (i._lo > real_zero) {
			_lo = std::max(real_zero, rmath::mul_lo(_lo, i._lo));
			_hi = rmath::mul_hi(_hi, i._hi);
		} else if (i._hi <= real_zero) {
			real lo = rmath::mul_lo(_hi, i._lo);
			_hi = std::min(real_zero, rmath::mul_hi(_lo, i._hi));
			_lo = lo;
		} else {
			_lo = rmath::mul_lo(_hi, i._lo);
			_hi = rmath::mul_hi(_hi, i._hi);
		}
	} else if (_hi <= real_zero) {
		if (i._lo >= real_zero) {
			_lo = rmath::mul_lo(_lo, i._hi);
			_hi = std::min(real_zero, rmath::mul_hi(_hi,i._lo));
		} else if (i._hi <= real_zero) {
			real lo = std::max(real_zero, rmath::mul_lo(_hi,i._hi));
			_hi = rmath::mul_hi(_lo, i._lo);
			_lo = lo;
		} else {
			real lo = rmath::mul_lo(_lo, i._hi);
			_hi = rmath::mul_hi(_lo, i._lo);
			_lo = lo;
		}
	} else if (i._lo >= real_zero) {
		_lo = rmath::mul_lo(_lo, i._hi);
		_hi = rmath::mul_hi(_hi, i._hi);
	} else if (i._hi <= real_zero) {
		real lo = rmath::mul_lo(_hi, i._lo);
		_hi = rmath::mul_hi(_lo, i._lo);
		_lo = lo;
	} else {
		real lo = std::min(rmath::mul_lo(_hi, i._lo), rmath::mul_lo(_lo, i._hi));
		_hi = std::max(rmath::mul_hi(_lo, i._lo), rmath::mul_hi(_hi, i._hi));
		_lo = lo;
	}
	return *this;
}



interval&
interval::operator/=(interval i) {
	if (has_zero() && i.has_zero()) return *this = interval::full();
	if (i._lo == real_zero) i._lo = real_zero;
	if (i._hi == real_zero) i._hi = -real_zero;
	if (_lo >= real_zero) {
		if (i._lo >= real_zero) {
			real lo = std::max(real_zero, rmath::div_lo(_lo, i._hi));
			_hi = rmath::div_hi(_hi, i._lo);
			_lo = lo;
		} else if (i._hi <= real_zero) {
			real lo = rmath::div_lo(_hi, i._hi);
			_hi = std::min(real_zero, rmath::div_hi(_lo, i._lo));
			_lo = lo;
		} else *this = full();
	} else if (_hi <= real_zero) {
		if (i._lo >= real_zero) {
			_lo = rmath::div_lo(_lo, i._lo);
			_hi = std::min(real_zero, rmath::div_hi(_hi, i._hi));
			return *this;
		} else if (i._hi <= real_zero) {
			real lo = std::max(real_zero, rmath::div_lo(_hi, i._lo));
			_hi = rmath::div_hi(_lo, i._hi);
			_lo = lo;
		} else *this = full();
	} else if (i._lo >= real_zero) {
		_lo = rmath::div_lo(_lo, i._lo);
		_hi = rmath::div_hi(_hi, i._lo);
	} else if (i._hi <= real_zero) {
		real lo = rmath::div_lo(_hi, i._hi);
		_hi = rmath::div_hi(_lo, i._hi);
		_lo = lo;
	} else *this = full();
	return *this;
}

namespace {
	interval
	sin_2_pi(real x) {
		return interval(rmath::sin2pi_lo(x), rmath::sin2pi_hi(x));
	}

	interval
	cos_2_pi(real x) {
		return interval(rmath::cos2pi_lo(x), rmath::cos2pi_hi(x));
	}

	interval
	do_sin2pi(real r, int a) {
		switch (a) {
		case 0: return sin_2_pi(r);
		case 1: return cos_2_pi(r);
		case 2: return -sin_2_pi(r);
		default: return -cos_2_pi(r);
		}
	}

	interval
	sin_range(int a, int b) {
		switch (4 * a + b) {
		case  0: return interval(-1.0, 1.0);
		case  1: return interval( 1.0, 1.0);
		case  2: return interval( 0.0, 1.0);
		case  3: return interval(-1.0, 1.0);
		case  4: return interval(-1.0, 0.0);
		case  5: return interval(-1.0, 1.0);
		case  6: return interval( 0.0, 0.0);
		case  7: return interval(-1.0, 0.0);
		case  8: return interval(-1.0, 0.0);
		case  9: return interval(-1.0, 1.0);
		case 10: return interval(-1.0, 1.0);
		case 11: return interval(-1.0,-1.0);
		case 12: return interval( 0.0, 0.0);
		case 13: return interval( 0.0, 1.0);
		case 14: return interval( 0.0, 1.0);
		default: return interval(-1.0, 1.0);
		}
	}


}

interval
sin2pi(interval const &x) {
	if (!isfinite(x.lo()) || !isfinite(x.hi()))
		return interval(real_neg_one, real_one);

	real m1 = std::rint(4.0l * x.lo());
	int j1 = (int)std::round(m1 - 4.0l * std::floor(m1 / 4.0l));
	real z1 = rmath::sub_lo(x.lo(), m1/4.0l);
	real n1 = std::floor(m1/4.0l);

	real m2 = std::rint(4.0l * x.hi());
	int j2 = (int)std::round(m2 - 4.0l * std::floor(m1 / 4.0l));
	real z2 = rmath::sub_hi(x.hi(), m2 / 4.0l);
	real n2 = std::floor(m2 / 4.0l);
	if ((z1 <= -0.25l) || (z1 >= 0.25l) || (z2 <= -0.25l) || (z2 >= 0.25l))
		return interval(real_neg_one, real_one);
	long mlo = (z1 >= 0) ? j1 : j1 - 1;
	long mhi = (z2 <= 0) ? j2 : j2 + 1;

	real w = mhi - mlo + 4.0l * (n2 - n1);

	if (w > 4)
		return interval(real_neg_one, real_one);

	interval y = do_sin2pi(z1, j1).join(do_sin2pi(z2, j2));

	int a = (int) ((mlo + 4) & 3);
	int b = (int) ((mhi + 3) & 3);
	if (w <= 1)
		return y;
	return y.join(sin_range(a, b));
}

interval
cos2pi(interval const &x) {
	if (!isfinite(x.lo()) || !isfinite(x.hi()))
		return interval(real_neg_one, real_one);

	real m1 = std::rint(4.0l * x.lo());
	int j1 = (int)std::round(m1 - 4.0l * std::floor(m1 / 4.0l));
	real z1 = rmath::sub_lo(x.lo(), m1 / 4.0l);
	real n1 = std::floor(m1 / 4.0l);

	real m2 = std::rint(4.0l * x.hi());
	int j2 = (int)std::round(m2 - 4.0l * std::floor(m2 / 4.0l));
	real z2 = rmath::sub_hi(x.hi(), m2 / 4.0l);
	real n2 = std::floor(m2 / 4.0l);

	if ((z1 <= -0.25l) || (z1 >= 0.25l) || (z2 <= -0.25l) || (z2 >= 0.25l))
		return interval(real_neg_one, real_one);

	long mlo = (z1 >= 0) ? j1 : j1 - 1;
	long mhi = (z2 <= 0) ? j2 : j2 + 1;
	real w = mhi - mlo + 4.0l * (n2 - n1);
	if (w > 4)
		return interval(real_neg_one, real_one);

	interval y = do_sin2pi(z1, (j1 + 1) & 3).join(do_sin2pi(z2, (j2 + 1) & 3));

	if (w <= 1) return y;

	int a = (int)((mlo + 5) & 3);
	int b = (int)((mlo + 4) & 3);

	return y.join(sin_range(a, b));
}

interval
midpoint(interval const &x) {
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

interval
leftendpoint(interval const &x) {
	if (isfinite(x.lo())) return interval(x.lo(), x.lo());
	return interval(rmath::r_down(x.lo()), rmath::r_up(x.lo()));
}

interval
rightendpoint(interval const &x) {
	if (isfinite(x.hi())) return interval(x.hi(), x.hi());
	return interval(rmath::r_down(x.hi()), rmath::r_up(x.hi()));
}

interval
even_power(interval const &x, real y) {
	if (y == real_zero) return interval::one();
	if (y < real_zero)
		return (interval::one() / even_power(x, -y));
	if (y > real_zero) {
		real zlo, zhi;
		if (x.lo() >= real_zero) {
			zlo = rmath::pow_lo(x.lo(), y);
			zhi = rmath::pow_hi(x.hi(), y);
		} else if (x.hi() <= real_zero) {
			zlo = rmath::pow_lo(-x.hi(), y);
			zhi = rmath::pow_hi(-x.lo(), y);
		} else {
			zlo = real_zero;
			zhi = std::max(rmath::pow_lo(-x.lo(), y), rmath::pow_hi(x.hi(), y));
		}
		return interval(zlo, zhi);
	}
	throw iv_arithmetic_error("even_power: can't raise a number to the NaNth power.");
}

interval
odd_power(interval const &x, real y) {
	if (y == real_zero) {
		if (x.lo() > real_zero)
			return interval::one();
		if (x.hi() < real_zero)
			return interval::minus_one();
		return interval(real_neg_one, real_one);
	} else if (y > real_zero) {
		if (x.lo() >= real_zero)
			return interval(rmath::pow_lo(x.lo(), y), rmath::pow_hi(x.hi(), y));
		if (x.hi() <= real_zero)
			return interval(-rmath::pow_hi(-x.lo(), y), -rmath::pow_lo(-x.hi(), y));
		return interval(-rmath::pow_hi(-x.lo(), y), rmath::pow_hi(x.hi(), y));
	} else if (y < real_zero) return (interval::one() / odd_power(x, -y));
	throw iv_arithmetic_error("odd_power: can't raise a number to the NaNth power");
}

interval
even_root(interval const &x, real y) {
	if (y == real_zero)
		throw iv_arithmetic_error("even_root: can't take the zeroth root of a number");
	else if (y > real_zero) {
		real zlo, ylo = rmath::div_lo(real_one, y);
		real zhi, yhi = rmath::div_hi(real_one, y);

		if (x.lo() >= real_one) zlo = rmath::pow_lo(x.lo(), ylo);
		else if (x.lo() >= real_zero) zlo = rmath::pow_lo(x.lo(), yhi);
		else zlo = real_zero;

		if (x.hi() >= real_one) zhi = rmath::pow_hi(x.hi(), yhi);
		if (x.lo() >= real_zero) zhi = rmath::pow_hi(x.hi(), ylo);
		else throw iv_arithmetic_error("even_root: can't take an even root of negative number.");

		return interval(zlo, zhi);
	} else if (y < real_zero) return (interval::one() / even_root(x, -y));
	throw iv_arithmetic_error("even_root: can't take the NaNth root of a number.");
}

interval
odd_root(interval const &x, real y) {
	if (y == real_zero) return interval::full();
	if (y > real_zero) {
		real zlo, ylo = rmath::div_lo(real_one, y);
		real zhi, yhi = rmath::div_hi(real_one, y);

		if (x.lo() >= real_one) zlo = rmath::pow_lo(x.lo(), ylo);
		else if (x.lo() >= real_zero) zlo = rmath::pow_lo(x.lo(), yhi);
		else if (x.lo() >= real_neg_one) zlo = -rmath::pow_hi(-x.lo(), ylo);
		else zlo = -rmath::pow_hi(-x.hi(), ylo);

		if (x.hi() >= real_one) zhi = rmath::pow_hi(x.hi(), yhi);
		else if (x.hi() >= real_zero) zhi = rmath::pow_hi(x.hi(), ylo);
		else if (x.hi() >= real_neg_one) zhi = -rmath::pow_lo(-x.hi(), yhi);
		else zhi = -rmath::pow_lo(-x.hi(), ylo);

		return interval(zlo, zhi);
	} else if (y < real_zero) return (interval::one() / odd_root(x, -y));
	throw iv_arithmetic_error("odd_root: can't take the NaNth root of a number.");
}

interval
integral_pow(interval const &x, interval const &y) {
	if ((y.lo() != y.hi()) || (remainderl(y.lo(), real_one) != real_zero))
		return interval::full();
	if (remainderl(y.lo(), 2.0l) == real_zero)
		return even_power(x, y.lo());
	return odd_power(x, y.lo());
}


namespace {
	real
	int_pow_lo(real x, int y) {
		real res = (y & 1) ? x : 1;
		y >>= 1;
		while (y > 0) {
			x = rmath::mul_lo(x, x);
			if (y & 1) res = rmath::mul_lo(x, res);
			y >>= 1;
		}
		return res;
	}

	real
	int_pow_hi(real x, int y) {
		real res = (y & 1) ? x : 1;
		y >>= 1;
		while (y > 0) {
			x = rmath::mul_hi(x, x);
			if (y & 1) res = rmath::mul_hi(x, res);
			y >>= 1;
		}
		return res;
	}
}


interval
int_pow(interval const &x, int y) {
	if (!y) {
		if (x.is_zero()) return interval::empty();
		return interval::one();
	} else if (y < 0) return int_pow(x, -y);
	else if (x.hi() < 0) {
		real ll = int_pow_lo(-x.hi(), y);
		real hh = int_pow_hi(-x.lo(), y);
		if (y & 1) return interval(-hh, -ll);
		return interval(ll, hh);
	} else if (x.lo() < 0) {
		if (y & 1)
			return interval(-int_pow_hi(-x.lo(), y), int_pow_hi(x.hi(), y));
		return interval(real_zero, int_pow_hi(std::max(-x.lo(), x.hi()), y));
	} else return interval(int_pow_lo(x.lo(), y), int_pow_hi(x.hi(), y));
}


interval
integral_root(interval const &x, interval const &y) {
	if ((y.lo() != y.hi()) || (remainderl(y.lo(), real_one) != real_zero))
		return interval::full();
	if (remainderl(y.lo(), 2.0l) == real_zero)
		return even_root(x, y.lo());
	return odd_root(x, y.lo());
}


}