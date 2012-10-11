#ifndef _INTERVAL_HH_
#define _INTERVAL_HH_
#include <cfloat>
#include "common.hh"

namespace numbers {
	
	typedef long double number_type;
	static const long double ld_minus_one = -1.0L;
	static const long double ld_one = 1.0L;
	static const long double ld_zero = 0.0L;


	class interval {
		number_type _lo, _hi;
	public:
		static const interval zero;
		static const interval minus_one;
		static const interval one;
		interval(number_type lo, number_type hi) : _lo(lo), _hi(hi) {}
		interval(number_type center) : _lo(center-LDBL_EPSILON), _hi(center+LDBL_EPSILON) {}
		interval() : _lo(0), _hi(0) {}
		interval(interval const &i) : _lo(i._lo), _hi(i._hi) {}
		~interval() {}
		interval &operator=(const interval &rhs) {
			_lo = rhs._lo;
			_hi = rhs._hi;
			return *this;
		}

		number_type lo() const { return _lo; }
		number_type hi() const { return _hi; }

		interval &operator+=(const interval &y) {
			_lo += y.lo();
			_hi += y.hi();
			return *this;
		}

		interval operator+(const interval &y) const {
			interval r = *this;
			r += y;
			return r;
		}

		interval &operator-=(const interval &y) {
			_lo -= y.lo();
			_hi -= y.hi();
			return *this;
		}

		interval operator-(const interval &y) const {
			interval r = *this;
			r -= y;
			return r;
		}

		interval operator-() const {
			interval r = *this;
			r = zero - r;
			return r;
		}

		interval &operator*=(const interval &y) {
			number_type p1 = _lo * y.lo(), p2 = _lo * y.hi();
			number_type p3 = _hi * y.lo(), p4 = _hi * y.hi();
			_lo = MIN(MIN(p1, p2), MIN(p3, p4));
			_hi = MAX(MAX(p1, p2), MAX(p3, p4));
			return *this;
		}

		interval operator*(const interval &y) const {
			interval r = *this;
			r *= y;
			return r;
		}

		interval operator/(const interval &y) const {
			interval z(ld_one/y.lo(), ld_one/y.hi()), r = *this;
			r *= z;
			return r;
		}

		interval &operator/=(const interval &y) {
			interval z = *this/y;
			_lo = z.lo();
			_hi = z.hi();
			return *this;
		}

		bool contains(number_type n) const {
			return _lo <= n && n <= _hi;
		}
	};

	inline std::ostream &operator<<(std::ostream &out, const interval &i) {
		return out << "[" << i.lo() << ", " << i.hi() << "]";
	}
}








#endif