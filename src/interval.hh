#ifndef _INTERVAL_HH_
#define _INTERVAL_HH_

#include <cfloat>
#include <limits>
#include <stdexcept>
#include "common.hh"
#include "rounding.hh"
#include "utilities.hh"
namespace calc {

class iv_arithmetic_error : public std::runtime_error {
public:
	iv_arithmetic_error() : std::runtime_error("iv_arithmetic_error") {}
	iv_arithmetic_error(char const *msg) : std::runtime_error(msg) {}
	iv_arithmetic_error(std::string const &msg) : std::runtime_error(msg) {}
};


class interval {
private:
	real _lo, _hi;
public:
	interval();
	interval(real x);
	interval(real lo, real hi);
	interval(interval const &other);

	real lo() const;
	real hi() const;

	interval &operator=(interval const &i);
	interval &operator=(real r);

	bool operator==(interval const &other) const;
	bool operator!=(interval const &other) const;

	interval &operator+=(interval const &i);
	interval &operator+=(real r);
	interval &operator-=(interval const &i);
	interval &operator-=(real r);
	interval &operator*=(interval const &i);
	interval &operator*=(real r);
	interval &operator/=(interval i);
	interval &operator/=(real r);

	friend std::ostream &operator<<(std::ostream &o, interval const &i);

	bool is_singleton() const;
	bool is_empty() const;
	bool is_infinite() const;
	bool is_zero() const;
	bool is_one() const;
	bool has_zero() const;

	void set_lo(real lo);
	void set_hi(real hi);

	interval &intersect(interval const &i);
	interval intersection(interval const &i) const;

	interval &join(interval const &i);
	interval joined(interval const &i) const;

	static interval empty();
	static interval full();
	static interval zero();
	static interval one();
	static interval minus_one();
	static interval pi();
	static interval two_pi();
	static interval half_pi();
	static interval hull(real r1, real r2);

};

interval sin2pi(interval const &x);
interval cos2pi(interval const &x);
interval even_power(interval const &x, real y);
interval odd_power(interval const &x, real y);
interval even_root(interval const &x, real y);
interval odd_root(interval const &x, real y);
interval integral_pow(interval const &x, interval const &y);
interval int_pow(interval const &x, int y);
interval integral_root(interval const &x, interval const &y);
interval midpoint(interval const &x);
interval leftendpoint(interval const &x);
interval rightendpoint(interval const &x);





inline interval::interval(real lo, real hi)
: _lo(lo), _hi(hi) {
	if (_lo > _hi) std::swap(_lo, _hi);
}

inline interval::interval(real x)
: _lo(x), _hi(x) {}

inline interval::interval()
: _lo(real_zero), _hi(real_zero) {}

inline interval::interval(interval const &other)
: _lo(other._lo), _hi(other._hi) {}

inline real
interval::lo() const {
	return _lo;
}

inline real
interval::hi() const {
	return _hi;
}

inline interval&
interval::operator=(interval const &i) {
	_lo = i._lo;
	_hi = i._hi;
	return *this;
}

inline interval&
interval::operator=(real r) {
	_lo = _hi = r;
	return *this;
}

inline bool
interval::operator==(interval const &other) const {
	return (_lo == other._lo) && (_hi == other._hi);
}

inline bool
interval::operator!=(interval const &other) const {
	return !(*this == other);
}

inline interval&
interval::operator+=(interval const &i) {
	_lo = rmath::add_lo(_lo, i._lo);
	_hi = rmath::add_hi(_hi, i._hi);
	return *this;
}

inline interval&
interval::operator+=(real r) {
	_lo = rmath::add_lo(_lo, r);
	_hi = rmath::add_hi(_hi, r);
	return *this;
}

inline interval&
interval::operator-=(interval const &i) {
	_lo = rmath::sub_lo(_lo, i._hi);
	_hi = rmath::sub_hi(_hi, i._lo);
	return *this;
}

inline interval&
interval::operator-=(real r) {
	_lo = rmath::sub_lo(_lo, r);
	_hi = rmath::sub_hi(_hi, r);
	return *this;
}

inline interval&
interval::operator*=(real r) {
	return *this *= interval(r);
}

inline interval&
interval::operator/=(real r) {
	return *this /= interval(r);
}

inline std::ostream&
operator<<(std::ostream &o, interval const &i) {
	if (i.is_empty()) return o << "[]";
	if (i.is_singleton()) return o << "[" << i.lo() << "]";
	return o << "[" << i.lo() << ", " << i.hi() << "]";
}

inline bool
interval::is_singleton() const {
	return _lo == _hi;
}

inline bool
interval::is_empty() const {
	return rmath::is_nan(_lo) || rmath::is_nan(_hi);
}

inline bool
interval::is_infinite() const {
	return !(std::isfinite(_lo) && std::isfinite(_hi));
}

inline bool
interval::is_zero() const {
	return (_lo == real_zero) && (_hi == real_zero);
}

inline bool
interval::is_one() const {
	return (_hi == real_one) && (_lo == real_one);
}

inline bool
interval::has_zero() const {
	return (_lo <= real_zero) && (real_zero <= _hi);
}

inline interval&
interval::intersect(interval const &i) {
	_lo = std::max(_lo, i._lo);
	_hi = std::min(_lo, i._lo);
	return *this;
}

inline interval
interval::intersection(interval const &i) const {
	return interval(*this).intersect(i);
}

inline void
interval::set_lo(real lo) {
	_lo = lo;
}

inline void
interval::set_hi(real hi) {
	_hi = hi;
}

inline interval&
interval::join(interval const &i) {
	_lo = std::min(_lo, i._lo);
	_hi = std::max(_hi, i._hi);
	return *this;
}

inline interval
interval::joined(interval const &i) const {
	return interval(*this).join(i);
}


inline interval
interval::empty() {
	return interval(rmath::NaN(), rmath::NaN());
}

inline interval
interval::full() {
	return interval(rmath::neg_inf(), rmath::pos_inf());
}

inline interval
interval::zero() {
	return interval(real_zero, real_zero);
}

inline interval
interval::one() {
	return interval(real_one, real_one);
}

inline interval
interval::minus_one() {
	return interval(real_neg_one, real_neg_one);
}

inline interval
interval::pi() {
	return interval(rmath::pi_lo(), rmath::pi_hi());
}

inline interval
interval::two_pi() {
	return interval(rmath::pi_times_two_lo(), rmath::pi_times_two_hi());
}

inline interval
interval::half_pi() {
	return interval(rmath::pi_over_two_lo(), rmath::pi_over_two_hi());
}

inline interval
interval::hull(real r1, real r2) {
	bool b1 = rmath::is_nan(r1), b2 = rmath::is_nan(r2);
	if (b1 && b2) return interval::empty();
	if (b1) return interval(r2, r2);
	if (b2) return interval(r1, r1);
	if (r1 <= r2) return interval(r1, r2);
	return interval(r2, r1);
}

inline interval
operator+(interval const &a, interval const &b) {
	return interval(rmath::add_lo(a.lo(), b.lo()), rmath::add_hi(a.hi(), b.hi()));
}

inline interval
operator+(interval const &a, real b) {
	return interval(rmath::add_lo(a.lo(), b), rmath::add_hi(a.hi(), b));
}

inline interval
operator+(real a, interval const &b) {
	return interval(rmath::add_lo(a, b.lo()), rmath::add_hi(a, b.hi()));
}

inline interval
operator+(interval a) {
	return a;
}

inline interval
operator-(interval const &a, interval const &b) {
	return interval(rmath::sub_lo(a.lo(), b.hi()), rmath::sub_hi(a.hi(), b.lo()));
}

inline interval
operator-(interval const &a, real b) {
	return interval(rmath::sub_lo(a.lo(), b), rmath::sub_hi(a.hi(), b));
}

inline interval
operator-(real a, interval const &b) {
	return interval(rmath::sub_lo(a, b.hi()), rmath::sub_hi(a, b.lo()));
}

inline interval
operator-(interval const &b) {
	return interval(-b.hi(), -b.lo());
}

inline interval
operator*(interval a, interval const &b) {
	return a *= b;
}

inline interval
operator/(interval a, interval const &b) {
	return a /= b;
}

inline interval
operator*(interval a, real b) {
	return a *= interval(b);
}

inline interval
operator*(real b, interval const &a) {
	return interval(b) * a;
}

inline interval
operator/(real b, interval const &a) {
	return interval(b) / a;
}

inline interval
exp(interval const &x) {
	return interval(rmath::exp_lo(x.lo()), rmath::exp_hi(x.hi()));
}

inline interval
log(interval const &x) {
	if (x.hi() <= real_zero) return interval::empty();
	return interval(rmath::log_lo(std::max(x.lo(), real_zero)), rmath::log_hi(x.hi()));
}

inline interval
tan2pi(interval const &x) {
	return sin2pi(x) / cos2pi(x);
}

inline interval
sin(interval const &x) {
	return sin2pi(x / interval::two_pi());
}

inline interval
cos(interval const &x) {
	return cos2pi(x / interval::two_pi());
}

inline interval
tan(interval const &x) {
	return tan2pi(x / interval::two_pi());
}

inline interval
asin(interval const &x) {
	interval y = x.intersection(interval(real_neg_one, real_one));
	return interval(rmath::asin_lo(y.lo()), rmath::asin_hi(y.hi()));
}

inline interval
acos(interval const &x) {
	return interval(rmath::acos_lo(x.hi()), rmath::acos_hi(x.lo()));
}

inline interval
atan(interval const &x) {
	return interval(rmath::atan_lo(x.lo()), rmath::atan_hi(x.hi()));
}

inline interval
asin2pi(interval const &x) {
	interval y = x.intersection(interval(real_neg_one, real_one));
	return interval(rmath::asin2pi_lo(y.lo()), rmath::asin2pi_hi(x.hi()));
}

inline interval
acos2pi(interval const &x) {
	return interval(rmath::acos2pi_lo(x.hi()), rmath::acos2pi_hi(x.lo()));
}

inline interval
atan2pi(interval const &x) {
	return interval(rmath::atan2pi_lo(x.lo()), rmath::atan2pi_hi(x.hi()));
}


inline interval
power(interval const &x, interval const &y) {
	if (x.hi() <= 0) {
		std::string s = stringize() << "power(x, y): x <= 0 not allowed. got: " << x;
		throw iv_arithmetic_error(s);
	}
	return exp(y * log(interval(std::max(x.lo(), real_zero), x.hi())));
}


inline interval
abs(interval const &x) {
	if (x.lo() > 0) return x;
	if (x.hi() < 0) return -x;
	return interval(real_zero, std::max(-x.lo(), x.hi()));
}

inline interval
sgn(interval const &x) {
	if (x.hi() < 0) return interval::minus_one();
	if (x.lo() > 0) return interval::one();
	return interval::zero();
}

inline interval
iv_max(interval const &x, interval const &y) {
	return interval(std::max(y.lo(), x.lo()), std::max(y.hi(), x.hi()));
}

inline interval
iv_min(interval const &x, interval const &y) {
	return interval(std::min(x.lo(), y.lo()), std::min(x.hi(), y.hi()));
}

inline interval
hull(interval const &x, interval const &y) {
	if (x.is_empty()) return y;
	else if (y.is_empty()) return x;
	return interval(std::min(x.lo(), y.lo()), std::max(x.hi(), y.hi()));
}

inline interval
sqrt(interval const &x) {
	if (x.hi() < 0) return interval::empty();
	real l = (x.lo() <= 0) ? real_zero : rmath::sqrt_lo(x.lo());
	return interval(l, rmath::sqrt_hi(x.hi()));
}

inline interval
square(interval const &x) {
	real xl = x.lo(), xh = x.hi();
	if (xh < 0) return interval(rmath::mul_lo(xh, xh), rmath::mul_hi(xl, xl));
	if (xl > 0) return interval(rmath::mul_lo(xl, xl), rmath::mul_hi(xh, xh));
	return interval(real_zero, (-xl > xh ? rmath::mul_hi(xl, xl) : rmath::mul_hi(xh, xh)));
}




}
#endif