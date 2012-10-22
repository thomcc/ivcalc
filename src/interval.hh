#ifndef _INTERVAL_HH_
#define _INTERVAL_HH_

#include <cfloat>
#include <limits>

#include "common.hh"
#include "rounding.hh"

namespace calc {

typedef long double number_type;

template <typename Rounding>
class Interval {
public:
	Interval(number_type lo, number_type hi) {
		if (lo < hi) std::swap(lo, hi);
		Rounding c;
		_lo = c.r_down(lo);
		_hi = c.r_up(hi);
	}

	explicit Interval(number_type n) {
		Rounding c;
		_lo = c.r_down(n);
		_hi = c.r_up(n);
	}

	Interval() : _lo(0), _hi(0) {}

	Interval(Interval const &i) : _lo(i._lo), _hi(i._hi) {}

//	template <typename ROther>
//	Interval(Interval<ROther> const &i2) : _lo(i2._lo), _hi(i2._hi) {}

	~Interval() {}

	Interval &operator=(Interval const &rhs) {
		_lo = rhs._lo;
		_hi = rhs._hi;
		return *this;
	}

//	template <typename ROther>
//	Interval &operator=(Interval<ROther> const &rhs) {
//		_lo = rhs._lo;
//		_hi = rhs._hi;
//		return *this;
//	}

	Interval &operator=(number_type n) {
		_lo = n;
		_hi = n;
		return *this;
	}

	// [xl, xu] + [yl, yu] = [xl+yl, xu+yu]
	Interval &operator+=(Interval const &y) {
		Rounding r;
		_lo = r.add_d(_lo, y._lo);
		_hi = r.add_u(_hi, y._hi);
		return *this;
	}

	Interval &operator+=(number_type n) {
		Rounding r;
		_lo = r.add_d(_lo, n);
		_hi = r.add_u(_hi, n);
		return *this;
	}

//	template <typename ROther>
//	Interval &operator+=(Interval<ROther> const &y) {
//		Rounding r;
//		_lo = r.add_d(_lo, y._lo);
//		_hi = r.add_u(_hi, y._hi);
//		return *this;
//	}

	// [xl, xu] - [yl, yu] = [xl-yu, xu-yl]
	Interval &operator-=(Interval const &y) {
		Rounding r;
		_lo = r.sub_d(_lo, y._lo);
		_hi = r.sub_u(_lo, y._hi);
		return *this;
	}

	Interval &operator-=(number_type n) {
		Rounding r;
		_lo = r.sub_d(_lo, n);
		_hi = r.sub_u(_hi, n);
		return *this;
	}

//	template <typename ROther>
//	Interval &operator-=(Interval<ROther> const &rhs) {
//		Rounding r;
//		_lo = r.sub_d(_lo, rhs._hi);
//		_hi = r.sub_u(_hi, rhs._lo);
//		return *this;
//	}
//
	// [xl,xu] * [yl,yu] = [min(xl*yl,xl*yu,xu*yl,xu*yu), max(xl*yl,xl*yu,xu*yl,xu*yu)]
	// ... discounting rounding modes.
	// bleck.  what a mess.
	Interval operator*(Interval const &y) const {
		Rounding r;
		number_type xl = _lo, xu = _hi, yl = y._lo, yu = y._hi;
		if (xl < 0) {
			if (xu > 0) {
				if (yl < 0) {
					if (yu > 0) {
						return Interval(std::min(r.mul_d(xl, yu), r.mul_d(xu, yl)),
						                std::max(r.mul_u(xl, yl), r.mul_u(xu, yu)));
					} else return Interval(r.mul_d(xu, yl), r.mul_u(xl, yl));
				}
				if (yu > 0) return Interval(r.mul_d(xl, yu), r.mul_u(xu, yu));
				else return iv_zero();
			}
			if (yl < 0) {
				if (yu > 0) return Interval(r.mul_d(xl, yu), r.mul_u(xl, yl));
				else return Interval(r.mul_d(xu, yu), r.mul_u(xl, yl));
			} else if (yu > 0)
				return Interval(r.mul_d(xu, yl), r.mul_u(xu, yl));
			return iv_zero();
		}
		if (xu > 0) {
			if (yl < 0) {
				if (yu > 0) return Interval(r.mul_d(xu, yl), r.mul_u(xu, yu));
				return Interval(r.mul_d(xu, yl), r.mul_u(xl, yu));
			} else if (yu > 0)
				return Interval(r.mul_d(xl, yl), r.mul_u(xu, yu));
			return iv_zero();
		}

		return iv_zero();
	} 

	Interval operator*(number_type n) const {
		Rounding r;
		if (n < 0)
			return Interval(r.mul_d(n, _hi), r.mul_u(n, _lo));
		else if (n == 0)
			return Interval(0, 0);
		return Interval(r.mul_d(n, _lo), r.mul_u(n, _hi));
	}

//	template<typename ROther>
//	Interval operator*(Interval<ROther> const &i) {
//		return *this * Interval<Rounding>(i);
//	}
//
	Interval operator/(Interval const &y) const {
		if ((y._lo <= 0) && (y._hi >= 0)) {
			// y contains 0
			if (y._lo != 0) {

				// dividing by a not strictly positive or negative zero 
				if (y._hi != 0) return zdiv();

				// dividing by -zero
				return n_zdiv(y._lo);
			}

			// dividing by +zero
			if (y._hi != 0) return p_zdiv(y._hi);

			// dividing by exactly zero
			return iv_empty();
		}

		// we aren't dividing by zero
		return do_div(y);
	}

//	template<typename ROther>
//	Interval operator/(Interval<ROther> const &i) const {
//		return *this / Interval<Rounding>(i);
//	}

	Interval operator/(number_type n) const {
		if (n == 0) return iv_empty();
		Rounding r;
		if (n < 0) return Interval(r.div_d(_hi, n), r.div_u(_lo, n));
		return Interval(r.div_d(_lo, n), r.div_u(_hi, n));
	}

	Interval &operator/=(Interval const &i) {
		return *this = *this / i;
	}

//	template <typename ROther>
//	Interval &operator/=(Interval<ROther> const &i) {
//		return *this = *this / Interval<Rounding>(i);
//	}

	Interval &operator/=(number_type n) {
		return *this = *this / n;
	}

	Interval &operator*=(Interval const &y) {
		return *this = *this * y;
	}

	Interval &operator*=(number_type n) {
		return *this = *this * n;
	}

//	template <typename ROther>
//	Interval &operator*=(Interval<ROther> const &i) {
//		return *this = *this * Interval<Rounding>(i);
//	}


	// unary +
	Interval const &operator+() const {
		return *this;
	}


	// iv + iv2 (same rounding type)
	Interval operator+(Interval const &rhs) const {
		Rounding r;
		return Interval(r.add_d(_lo, rhs._lo), r.add_u(_hi, rhs._hi));
	}

	// iv + iv2 (different rounding type)
//	template <typename ROther>
//	Interval operator+(Interval<ROther> const &rhs) const {
//		return *this + Interval<Rounding>(rhs);
//	}

	// iv + number
	Interval operator+(number_type n) const {
		Rounding r;
		return Interval(r.add_d(_lo, n), r.add_u(_hi, n));
	}


	// iv - iv2 (same rounding type)
	Interval operator-(Interval const &rhs) const {
		Rounding r;
		return Interval(r.sub_d(_lo, rhs._hi), r.sub_u(_hi, rhs._lo));
	}

	// iv - iv2 (rounding types differ)
//	template <typename ROther>
//	Interval operator-(Interval<ROther> const &rhs) const {
//		return *this - Interval<Rounding>(rhs);
//	}

	// iv - number
	Interval operator-(number_type n) const {
		Rounding r;
		return Interval(r.sub_d(_lo, n), r.sub_u(_hi, n));
	}


	// unary op -
	Interval operator-() const {
		return Interval(-_lo, -_hi);
	}


	bool is_zero() const {
		return (_lo == 0) && (_hi == 0);
	}

	bool has_zero() const {
		return (_lo < 0) && (_hi > 0);
	}

	bool contains(number_type n) const {
		return _lo <= n && n <= _hi;
	}

	bool contains(Interval const &i) const {
		return contains(i.lo()) && contains(i.hi());
	}

	bool operator==(Interval const &rhs) const {
		return (rhs._lo == _lo) && (rhs._hi == _hi);
	}

	bool operator!=(Interval const &rhs) const {
		return !(*this == rhs);
	}

	bool is_empty() const {
		return (_lo != _lo) && (_hi != _hi);
	}

	bool is_finite() const {
		return isfinite(_lo) && isfinite(_hi);
	}

	number_type lo() const { return _lo; }
	number_type hi() const { return _hi; }

	number_type mid() const {
		Rounding r;
		return r.avg(_lo, _hi);
	}

	Interval inverse() const {
		Rounding r;
		if (has_zero()) {
			if (_lo != 0) {
				if (_hi != 0) return iv_biggest();
				return from_neg_inf(r.div_u(1, _lo));
			}
			if (_hi != 0) return to_pos_inf(r.div_d(1, _hi));
			return iv_empty();
		}
		return Interval(r.div_d(1, _hi), r.div_u(1, _lo));
	}

	Interval expt(int p) const {
		if (!p) {
			if ((_lo == 0) && (_hi == 0)) return iv_empty();
			return Interval(static_cast<number_type>(1));
		} else if (p < 0) return expt(-p).inverse();
//		Rounding r;
		if (_hi < 0) {
			number_type ll = expt_d(-_hi, p), uu = expt_u(-_lo, p);
			if (p & 1) return Interval(-uu, -ll);
			return Interval(ll, uu);
		} else if (_lo < 0) {
			if (p & 1) return Interval(-expt_u(-_lo, p), expt_u(_hi, p));
			return Interval(0, expt_u(std::max(-_lo, _hi), p));
		}
		return Interval(expt_d(_lo, p), expt_u(_hi, p));
	}

	number_type
	size() const {
		Rounding r;
		number_type n = r.sub_u(_hi, _lo);
		return (n != n) ? 0 : n;
	}

	static Interval empty() {
		return Interval<Rounding>(
			std::numeric_limits<number_type>::quiet_NaN(),
			std::numeric_limits<number_type>::quiet_NaN()
		);
	}

	static Interval inf() {
		return Interval<Rounding>(
			-std::numeric_limits<number_type>::infinity(),
			 std::numeric_limits<number_type>::infinity()
		);
	}

	static Interval hull(number_type a, number_type b) {
		if (a != a) {
			if (b != b) return empty();
			return Interval(b, b);
		} else if (b != b) return Interval(a, a);
		if (a < b) return Interval(a, b);
		return Interval(b, a);
	}

private:

	number_type expt_d(number_type b, int e) const {
		Rounding r;
		number_type res = (e & 1) ? b : 1;
		e >>= 1;
		while (e > 0) {
			b = r.mul_d(b, b);
			if (e & 1) res = r.mul_d(b, res);
			e >>= 1;
		}
		return res;
	}

	number_type expt_u(number_type b, int e) const {
		Rounding r;
		number_type res = (e & 1) ? b : 1;
		e >>= 1;
		while (e > 0) {
			b = r.mul_u(b, b);
			if (e & 1) res = r.mul_u(b, res);
			e >>= 1;
		}
		return res;
	}

	Interval iv_empty() const {
		return Interval(
			std::numeric_limits<number_type>::quiet_NaN(),
			std::numeric_limits<number_type>::quiet_NaN()
		);
	}

	Interval iv_zero() const {
		return Interval(0,0);
	}
	
	Interval iv_biggest() const {
		return Interval(
			-std::numeric_limits<number_type>::infinity(),
			 std::numeric_limits<number_type>::infinity()
		);
	}
	
	Interval from_neg_inf(number_type n) const {
		return Interval(-std::numeric_limits<number_type>::infinity(), n);
	}

	Interval to_pos_inf(number_type n) const {
		return Interval(n, std::numeric_limits<number_type>::infinity());
	}

	Interval zdiv() const {
		if (is_zero()) return iv_zero();
		return iv_biggest();
	}

	Interval n_zdiv(number_type n) const {
		Rounding r;
		if (is_zero()) return iv_zero();
		if (_hi < 0) return to_pos_inf(r.div_d(_hi, n));
		if (_lo < 0) return iv_biggest();
		return from_neg_inf(r.div_u(_lo, n));
	}

	Interval p_zdiv(number_type n) const {
		Rounding r;
		if (is_zero()) return iv_zero();
		if (_hi < 0) return from_neg_inf(r.div_u(_hi, n));
		if (_lo < 0) return iv_biggest();
		return to_pos_inf(r.div_d(_lo, n));
	}

	Interval do_div(Interval const &y) const {
		Rounding r;
		number_type xu = _hi, xl = _lo, yu = y._hi, yl = y._lo;
		
		if (xu < 0) {
			if (yu < 0) return Interval(r.div_d(xu, yl), r.div_u(xl, yu));
			return Interval(r.div_d(xl, yl), r.div_u(xu, yu));
		}

		if (xl < 0) {
			if (yu < 0) return Interval(r.div_d(xu, yu), r.div_u(xl, yu));
			return Interval(r.div_d(xl, yl), r.div_u(xu, yl));
		}

		if (yu < 0) return Interval(r.div_d(xu, yu), r.div_u(xl, yl));

		return Interval(r.div_d(xl, yu), r.div_u(xu, yl));
	}

	number_type _lo, _hi;
};
// change fp round mode every time
typedef Interval<RoundStandard> interval_rstd;
// never change fp rounding mode
typedef Interval<RoundExact>    interval_rexact;
// change fp rounding mode infrequently by using tricks like
// round_down(a + b) == -round_up(-a - b).
typedef Interval<RoundFast>     interval_rfast;

typedef interval_rfast interval;

template <typename R>
inline std::ostream &operator<<(std::ostream &out, Interval<R> i) {
	return out << "[" << i.lo() << ", " << i.hi() << "]";
}

template <typename Rounding>
inline Interval<Rounding> operator+(number_type n, Interval<Rounding> const &i) {
	Rounding r;
	return Interval<Rounding>(r.add_d(n, i.lo()), r.add_u(n, i.hi()));
}

template <typename Rounding>
inline Interval<Rounding> operator-(number_type n, Interval<Rounding> const &i) {
	Rounding r;
	return Interval<Rounding>(r.sub_d(n, i.hi()), r.sub_u(n, i.lo()));

}

template <typename Rounding>
inline Interval<Rounding> operator*(number_type n, Interval<Rounding> const &i) {
	Rounding r;
	if (n < 0) return Interval<Rounding>(r.mul_d(n, i.hi()), r.mul_u(n, i.lo()));
	else if (n == 0) return Interval<Rounding>(0, 0);
	return Interval<Rounding>(r.mul_d(n, i.lo()), r.mul_u(n, i.hi()));
}

template <typename Rounding>
inline Interval<Rounding> operator/(number_type n, Interval<Rounding> const &i) {
	return Interval<Rounding>(n) / i; // meh.
}


template <typename Rounding>
inline Interval<Rounding>
fmod(Interval<Rounding> const &a, Interval<Rounding> const &b) {
	Rounding r;
	number_type n = r.floor(r.div_d(a.lo(), ((a.lo() < 0) ? b.lo() : b.hi())));
	return a - n * b;
}

template <typename Rounding>
inline Interval<Rounding>
fmod(Interval<Rounding> const &a, number_type b) {
	Rounding r;
	number_type n = r.floor(r.div_d(a.lo(), b));
	return a - n * Interval<Rounding>(b);
}

template <typename Rounding>
inline Interval<Rounding>
fmod(number_type a, Interval<Rounding> const &b) {
	Rounding r;
	number_type n = r.floor(r.div_d(a, ((a < 0) ? b.lo() : b.hi())));
	return a - n * b;
}

static const long double pi_lo = (3373259426.0 + 273688.0 / (1<<21)) / (1<<30);
static const long double pi_hi = (3373259426.0 + 273689.0 / (1<<21)) / (1<<30);
template <typename Rounding>
inline Interval<Rounding> pi() {
	return Interval<Rounding>(pi_lo, pi_hi);
}

template <typename Rounding>
inline Interval<Rounding> half_pi() {
	return Interval<Rounding>(pi_lo / 2, pi_hi / 2);
}

template <typename Rounding>
inline Interval<Rounding> two_pi() {
	return Interval<Rounding>(pi_lo * 2, pi_hi * 2);
}

template <typename Rounding>
inline Interval<Rounding> tan(Interval<Rounding> const &i) {
	Rounding r;
	const Interval<Rounding> p(pi<Rounding>());
	Interval<Rounding> i2 = fmod(i, p);
	number_type ph = pi_lo/2;
	if (i2.lo() >= ph) i2 -= p;
	if (i2.lo() <= -ph || i2.hi() >= ph) 
		return Interval<Rounding>::inf();
	return Interval<Rounding>(r.tan_d(i2.lo()), r.tan_u(i2.hi()));
}

template <typename Rounding>
inline Interval<Rounding> cos(Interval<Rounding> const &i) {
	Rounding r;
	const Interval<Rounding> pi_pi(two_pi<Rounding>());
	Interval<Rounding> i2(fmod(i, pi_pi));
	if (i2.size() >= pi_pi.lo()) 
		return Interval<Rounding>(-1, 1);
	if (i2.lo() >= pi_hi) 
		return -cos(i2 - pi<Rounding>());
	number_type l = i2.lo(), u = i2.hi();
	if (u <= pi_lo) 
		return Interval<Rounding>(r.cos_d(u), r.cos_u(l));
	if (u <= pi_pi.lo()) 
		return Interval<Rounding>(-1, r.cos_u(std::min(r.sub_d(pi_pi.lo(), u), l)));
	return Interval<Rounding>(-1, 1);
}

template<typename Rounding>
inline Interval<Rounding> sin(Interval<Rounding> const &i) {
	return cos(i - half_pi<Rounding>());
}


template<typename Rounding>
inline Interval<Rounding> exp(Interval<Rounding> const &i) {
	Rounding r;
	return Interval<Rounding>(r.exp_d(i.lo()), r.exp_u(i.hi()));
}

template<typename Rounding>
inline Interval<Rounding> log(Interval<Rounding> const &i) {
	if (i.hi() < 0) return Interval<Rounding>::empty();
	Rounding r;
	number_type low = (i.lo() < 0) ? -std::numeric_limits<number_type>::infinity() : r.log_d(i.lo());
	return Interval<Rounding>(low, r.log_u(i.hi()));
}


}
#endif