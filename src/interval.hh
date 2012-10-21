#ifndef _INTERVAL_HH_
#define _INTERVAL_HH_
#include <cfloat>
#include <limits>
#include "common.hh"

namespace numbers {

typedef long double number_type;

class interval {
public:

	interval(number_type lo, number_type hi)
	: _lo(lo)
	, _hi(hi) {
		if (_lo >= _hi) std::swap(_lo, _hi);
	}

	explicit
	interval(number_type n)
	: _lo(n)
	, _hi(n)
	{}

	interval()
	: _lo(0)
	, _hi(0)
	{}

	interval(interval const &i)
	: _lo(i._lo)
	, _hi(i._hi)
	{}

	~interval() {}

	interval&
	operator=(interval const &rhs) {
		_lo = rhs._lo;
		_hi = rhs._hi;
		return *this;
	}


	// [xl, xu] + [yl, yu] = [xl+yl, xu+yu]
	interval&
	operator+=(interval const &y) {
		_lo += y.lo();
		_hi += y.hi();
		return *this;
	}

	// [xl, xu] - [yl, yu] = [xl-yu, xu-yl]
	interval&
	operator-=(interval const &y) {
		_lo -= y.hi();
		_hi -= y.lo();
		return *this;
	}
	// [xl,xu] * [yl,yu] = [min(xl*yl,xl*yu,xu*yl,xu*yu), max(xl*yl,xl*yu,xu*yl,xu*yu)]
	interval&
	operator*=(interval const &y) {
		number_type p1 = _lo * y.lo(), p2 = _lo * y.hi();
		number_type p3 = _hi * y.lo(), p4 = _hi * y.hi();
		_lo = MIN4(p1, p2, p3, p4);
		_hi = MAX4(p1, p2, p3, p4);
		return *this;
	}

	// 1/[yl, yu] = [min(1/yl, 1/yu),max(1/yl, 1/yu)];
	interval &
	invert() {
		number_type l1 = 1 / _lo;
		number_type h1 = 1 / _hi;
		_lo = MIN(l1, h1);
		_hi = MAX(l1, h1);
		return *this;
	}

	interval
	inverse() const {
		return interval(*this).invert();
	}

	interval&
	operator/=(interval const &y) {
		return *this *= y.inverse();
	}

	interval&
	operator+=(number_type n) {
		return *this += interval(n);
	}

	interval&
	operator-=(number_type n) {
		return *this -= interval(n);
	}

	interval&
	operator*=(number_type n) {
		return *this *= interval(n);
	}

	interval&
	operator/=(number_type n) {
		return *this /= interval(n);
	}

	bool
	contains(number_type n) const {
		return _lo <= n && n <= _hi;
	}

	number_type
	lo() const { 
		return _lo;
	}

	number_type
	hi() const { 
		return _hi;
	}

	// 
	interval&
	exponentiate(int n) {
		if (n == 0) {
			_lo = _hi = 1;
			return *this;
		}
		
		bool neg = false;
		
		if (n < 0) { 
			n = -n;
			neg = true;
		}

		number_type lo_to_n = 1, hi_to_n = 1;

		for (int i = n; i--;) {
			lo_to_n *= _lo;
			hi_to_n *= _hi;
		}

		if ((n & 1) || (_lo >= 0)) {
			_lo = lo_to_n;
			_hi = hi_to_n;
		} else if (!(n & 1) && _hi <= 0) {
			_lo = hi_to_n;
			_hi = lo_to_n;
		} else {
			_lo = 0;
			_hi = MAX(lo_to_n, hi_to_n);
		}

		if (!neg) return *this;

		lo_to_n = 1 / lo_to_n;
		hi_to_n = 1 / hi_to_n;

		_lo = MIN(lo_to_n, hi_to_n);
		_hi = MAX(lo_to_n, hi_to_n);
		return *this;
	}

	interval
	exponent(int n) const {
		return interval(*this).exponentiate(n);
	}
	// sqr([xl,xh]) = { 0 not in [xl,xh] | [min(sqr(xl), sqr(xh)), max(sqr(xl, sqr(xh)))]
	//                { else             | [0, max(sqr(xl), sqr(xh))]

	interval &
	square() {
		number_type ll = _lo * _lo;
		number_type hh = _hi * _hi;
		if (contains(0)) {
			_lo = 0;
			_hi = MAX(ll, hh);
		} else {
			_lo = MIN(ll, hh);
			_hi = MAX(ll, hh);
		}
		return *this;
	}

	interval
	squared() const {
		return interval(*this).square();
	}

	// sqrt([xl,xh]) = { 0 < xl | [sqrt(xl), sqrt(xh)]
	//                 { else   | [0, sqrt(xh)]
	interval&
	make_sqrt() {
		if (_lo < 0) {
			_lo = 0;
			_hi = sqrtl(_hi);
		} else {
			_lo = sqrtl(_lo);
			_hi = sqrtl(_hi);
		}
		return *this;
	}

	interval
	sqrt() const {
		return interval(*this).make_sqrt();
	}

	interval&
	make_exp() {
		_lo = expl(_lo);
		_hi = expl(_hi);
		return *this;
	}
	// exp([xl,xh]) = [exp(xl), exp(xh)]
	interval
	exp() const {
		return interval(*this).make_exp();
	}

	bool
	strictly_gt(interval const &other) {
		return _lo > other._hi;
	}

	bool
	strictly_lt(interval const &other) {
		return _hi < other._lo;
	}
	

private:
	number_type _lo, _hi;
};


inline interval 
operator+(interval const &x) {
	return interval(x);
}

inline interval
operator-(interval const &x) {
	return interval(-x.hi(), -x.lo());
}

inline interval
operator-(interval const &x, interval const &y) {
	return interval(x) -= y;
}

inline interval
operator+(interval const &x, interval const &y) {
	return interval(x) += y;
}

inline interval
operator*(interval const &x, interval const &y) {
	return interval(x) *= y;
}

inline interval
operator/(interval const &x, interval const &y) {
	return interval(x) /= y;
}

inline interval
operator-(interval const &x, number_type y) {
	return interval(x) -= y;
}

inline interval
operator+(interval const &x, number_type y) {
	return interval(x) += y;
}

inline interval
operator*(interval const &x, number_type y) {
	return interval(x) *= y;
}

inline interval
operator/(interval const &x, number_type y) {
	return interval(x) /= y;
}

inline interval
operator-(number_type x, interval const &y) {
	return interval(x) -= y;
}

inline interval
operator+(number_type x, interval const &y) {
	return interval(x) += y;
}

inline interval
operator*(number_type x, interval const &y) {
	return interval(x) *= y;
}

inline interval
operator/(number_type x, interval const &y) {
	return interval(x) /= y;
}

inline bool
operator==(interval const &x, interval const &y) {
	return (x.lo() == y.lo()) && (x.hi() == y.hi());
}

inline bool
operator==(number_type n, interval const &y) {
	interval x(n);
	return (x.lo() == y.lo()) && (x.hi() == y.hi());
}

inline bool
operator==(interval const &x, number_type n) {
	interval y(n);
	return (x.lo() == y.lo()) && (x.hi() == y.hi());
}

inline bool
operator!=(interval const &x, interval const &y) {
	return !(x == y);
}

inline bool
operator!=(interval const &x, number_type y) {
	return !(x == y);
}

inline bool
operator!=(number_type x, interval const &y) {
	return !(x == y);
}

// hm.... what to do for < > <= >= ?
// [1.0, 3.0] < [0.0, 3.0] ???

inline std::ostream &
operator<<(std::ostream &out, interval const &i) {
	return out << "[" << i.lo() << ", " << i.hi() << "]";
}

}








#endif