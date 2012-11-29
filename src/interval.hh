#ifndef _INTERVAL_HH_
#define _INTERVAL_HH_

#include <cfloat>
#include <limits>
#include <stdexcept>
#include "common.hh"
#include "rounding.hh"
#include "utilities.hh"
#include "xmmintrin.h"
namespace calc {

class iv_arithmetic_error : public std::runtime_error {
public:
	iv_arithmetic_error() : std::runtime_error("iv_arithmetic_error") {}
	iv_arithmetic_error(char const *msg) : std::runtime_error(msg) {}
	iv_arithmetic_error(std::string const &msg) : std::runtime_error(msg) {}
};

class interval {
//private: // attempt to make usable from JIT code...
public:
	real _lo, _hi;

	interval();
	interval(real x);
	interval(real lo, real hi);
	interval(interval const &other) = default;

	real lo() const;
	real hi() const;

	interval &operator=(interval const &i) = default;
	interval &operator=(real r);

	bool operator==(interval const &other) const;
	bool operator!=(interval const &other) const;

	interval &operator+=(interval const &i);
	interval &operator+=(real r);
	interval &operator-=(interval const &i);
	interval &operator-=(real r);
	interval &operator*=(interval const &i);
	interval &operator*=(real r);
	interval &operator/=(interval const &i);
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
	real size() const;
	bool has(interval const &i) const;
	bool has(real r) const;
	bool overlap(interval const &i) const;
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

interval int_pow(interval const &x, int y);
interval midpoint(interval const &x);
interval leftendpoint(interval const &x);
interval rightendpoint(interval const &x);
interval operator/(interval const &a, interval const &b);
interval modulo(interval const &a, interval const &b);
interval modulo(interval const &a, real b);
interval modulo(real a, interval const &b);
interval int_pow(interval const &base, int expt);

inline interval::interval(real lo, real hi) : _lo(lo), _hi(hi) {
	if (_lo > _hi) std::swap(_lo, _hi);
}

inline interval::interval(real x) : _lo(x), _hi(x) {}
inline interval::interval() : _lo(real_zero), _hi(real_zero) {}
//inline interval::interval(interval const &other) : _lo(other._lo), _hi(other._hi) {}

inline real interval::lo() const { return _lo; }
inline real interval::hi() const { return _hi; }
inline real interval::size() const { return std::abs(rmath::sub_hi(_lo, _hi)); }
//inline interval& interval::operator=(interval const &i) { _lo = i._lo;  _hi = i._hi; return *this; }
inline interval& interval::operator=(real r) { _lo = _hi = r; return *this; }
inline bool interval::operator==(interval const &other) const { return (_lo == other._lo) && (_hi == other._hi); }

inline bool interval::operator!=(interval const &other) const { return !(*this == other); }

inline interval& interval::operator+=(interval const &i) {
	_lo = rmath::add_lo(_lo, i._lo);
	_hi = rmath::add_hi(_hi, i._hi);
	return *this;
}

inline interval& interval::operator+=(real r) {
	_lo = rmath::add_lo(_lo, r);
	_hi = rmath::add_hi(_hi, r);
	return *this;
}

inline interval& interval::operator-=(interval const &i) {
	_lo = rmath::sub_lo(_lo, i._hi);
	_hi = rmath::sub_hi(_hi, i._lo);
	return *this;
}

inline interval& interval::operator-=(real r) {
	_lo = rmath::sub_lo(_lo, r);
	_hi = rmath::sub_hi(_hi, r);
	return *this;
}

inline interval& interval::operator*=(real r) { return *this *= interval(r); }
inline interval& interval::operator/=(real r) { return *this = (*this / interval(r)); }
inline interval& interval::operator/=(interval const &i) { return *this = (*this / i); }

inline std::ostream& operator<<(std::ostream &o, interval const &i) {
	std::streamsize sz = o.precision();
	o.precision(std::numeric_limits<real>::digits10+1);
//	o << std::fixed;
	if (i.is_empty()) o << "[]";
	else if (i.is_singleton()) o << "[" << i.lo() << "]";
	else o << "[" << i.lo() << ", " << i.hi() << "]";
	o.precision(sz);
	return o;
}

inline bool interval::is_singleton() const { return _lo == _hi; }
inline bool interval::is_empty() const { return rmath::is_nan(_lo) || rmath::is_nan(_hi); }
inline bool interval::is_infinite() const { return !(std::isfinite(_lo) && std::isfinite(_hi)); }
inline bool interval::is_zero() const { return (_lo == real_zero) && (_hi == real_zero); }
inline bool interval::is_one() const { return (_hi == real_one) && (_lo == real_one); }
inline bool interval::has_zero() const { return (_lo <= real_zero) && (real_zero <= _hi); }

inline interval& interval::intersect(interval const &i) {
	_lo = std::max(_lo, i._lo);
	_hi = std::min(_lo, i._lo);
	return *this;
}

inline void interval::set_lo(real lo) { _lo = lo; }
inline void interval::set_hi(real hi) { _hi = hi; }

inline interval& interval::join(interval const &i) {
	_lo = std::min(_lo, i._lo);
	_hi = std::max(_hi, i._hi);
	return *this;
}

inline interval interval::joined(interval const &i) const { return interval(*this).join(i); }
inline interval interval::intersection(interval const &i) const { return interval(*this).intersect(i); }
inline bool interval::has(real r) const { return (!is_empty()) &&( _lo <= r) && (r <= _hi); }

inline bool interval::has(interval const &i) const {
	return i.is_empty() || (!is_empty() && _lo <= i._lo && i._hi <= _hi);
}

inline bool interval::overlap(interval const &i) const {
	return (_lo <= i._lo && i._lo <= _hi) || (i._lo <= _lo && _lo <= i._hi);
}


inline interval interval::empty() { return interval(rmath::NaN(), rmath::NaN()); }
inline interval interval::full() { return interval(rmath::neg_inf(), rmath::pos_inf()); }
inline interval interval::zero() { return interval(real_zero, real_zero); }
inline interval interval::one() { return interval(real_one, real_one); }
inline interval interval::minus_one() { return interval(real_neg_one, real_neg_one); }
inline interval interval::pi() { return interval(rmath::pi_lo(), rmath::pi_hi()); }
inline interval interval::two_pi() { return interval(rmath::pi_times_two_lo(), rmath::pi_times_two_hi()); }
inline interval interval::half_pi() { return interval(rmath::pi_over_two_lo(), rmath::pi_over_two_hi()); }

inline interval
interval::hull(real r1, real r2) {
	bool b1 = rmath::is_nan(r1), b2 = rmath::is_nan(r2);
	if (b1 && b2) return interval::empty();
	if (b1) return interval(r2, r2);
	if (b2) return interval(r1, r1);
	if (r1 <= r2) return interval(r1, r2);
	return interval(r2, r1);
}

inline interval operator+(interval const &a, interval const &b) {
	return interval(rmath::add_lo(a.lo(), b.lo()), rmath::add_hi(a.hi(), b.hi()));
}

inline interval operator+(interval const &a, real b) {
	return interval(rmath::add_lo(a.lo(), b), rmath::add_hi(a.hi(), b));
}

inline interval operator+(real a, interval const &b) {
	return interval(rmath::add_lo(a, b.lo()), rmath::add_hi(a, b.hi()));
}

inline interval operator+(interval const &a) { return a; }
inline interval operator-(interval const &a) { return interval(-a.hi(), -a.lo()); }

inline interval operator-(interval const &a, interval const &b) {
	return interval(rmath::sub_lo(a.lo(), b.hi()), rmath::sub_hi(a.hi(), b.lo()));
}

inline interval operator-(interval const &a, real b) {
	return interval(rmath::sub_lo(a.lo(), b), rmath::sub_hi(a.hi(), b));
}

inline interval operator-(real a, interval const &b) {
	return interval(rmath::sub_lo(a, b.hi()), rmath::sub_hi(a, b.lo()));
}

inline interval operator*(interval a, interval const &b) { return a *= b; }
inline interval operator*(interval a, real b) { return b * a; }

inline interval
operator*(real b, interval const &a) {
	if (rmath::is_nan(b) || a.is_empty()) return interval::empty();
	real lo = a.lo(), hi = a.hi();
	if (b < real_zero) return interval(rmath::mul_lo(b, hi), rmath::mul_hi(b, lo));
	else if (b == real_zero) return interval::zero();
	return interval(rmath::mul_lo(b, lo), rmath::mul_hi(b, hi));
}

inline interval operator/(real b, interval const &a) { return interval(b) / a; }

inline interval operator/(interval const &a, real b) {
	if (b == real_zero || rmath::is_nan(b) || a.is_empty()) return interval::empty();
	real lo = a.lo(), hi = a.hi();
	if (b < real_zero) return interval(rmath::div_lo(hi, b), rmath::div_hi(lo, b));
	return interval(rmath::div_lo(lo, b), rmath::div_hi(hi, b));
}


inline interval log(interval const &x) {
	if (x.hi() < 0) return interval::empty();
	return interval(x.lo() < 0 ? rmath::neg_inf() : rmath::log_lo(x.lo()), rmath::log_hi(x.hi()));
}

inline interval cos(interval const &x) {
	interval tau = interval::two_pi(), i = modulo(x, tau);
	if (i.size() > tau.lo()) return interval(real_neg_one, real_one);
	if (i.lo() >= rmath::pi_hi()) return -cos(i-interval::pi());
	if (i.hi() <= rmath::pi_lo()) return interval(rmath::cos_lo(i.hi()), rmath::cos_hi(i.lo()));
	if (i.hi() <= tau.lo())
		return interval(real_neg_one, rmath::cos_hi(std::min(rmath::sub_lo(tau.lo(), i.hi()), i.lo())));
	return interval(real_neg_one, real_one);
}
inline interval sin(interval const &x) {
	return cos(x - interval::half_pi());
}

inline interval tan(interval const &x) {
	interval pi = interval::pi(), i = modulo(x, pi);
	real phl = rmath::pi_over_two_lo();
	if (i.lo() >= phl) i -= pi;
	if (i.lo() <= -phl || i.hi() >= phl) return interval::full();
	return interval(rmath::tan_lo(i.lo()), rmath::tan_hi(i.hi()));
}

inline interval asin(interval const &x) {
	if (x.is_empty() || x.hi() < real_neg_one || x.lo() > real_one) return interval::empty();
	return interval(
		(x.lo() <= real_neg_one) ? -rmath::pi_over_two_lo() : rmath::asin_lo(x.lo()),
		(x.hi() >= real_one)     ?  rmath::pi_over_two_hi() : rmath::asin_hi(x.hi())
	);
}

inline interval acos(interval const &x) {
	if (x.is_empty() || x.hi() < real_neg_one || x.lo() > real_one) return interval::empty();
	return interval(
		(x.lo() <= real_one)     ? real_zero      : rmath::acos_lo(x.hi()),
		(x.hi() >= real_neg_one) ? rmath::pi_hi() : rmath::acos_hi(x.lo())
	);
}
#define BASIC_FUNC(NAME) \
inline interval NAME(interval const &x) { \
	if (x.is_empty()) return interval::empty(); \
	return interval(rmath:: NAME##_lo(x.lo()), rmath:: NAME##_hi(x.hi())); \
}

BASIC_FUNC(exp)
BASIC_FUNC(atan)
BASIC_FUNC(sinh)
BASIC_FUNC(tanh)
BASIC_FUNC(asinh)

#undef BASIC_FUNC

inline interval cosh(interval const &x) {
	if (x.hi() < 0) return interval(rmath::cosh_lo(x.hi()), rmath::cosh_hi(x.lo()));
	if (x.lo() >= 0) return interval(rmath::cosh_lo(x.lo()), rmath::cosh_hi(x.hi()));
	return interval(real_one, rmath::cosh_hi(-x.lo() > x.hi() ? x.lo() : x.hi()));
}

inline interval acosh(interval const &x) {
	if (x.is_empty() || x.hi() < real_one) return interval::empty();
	return interval((x.lo() <= real_one ? real_zero : rmath::acosh_lo(x.lo())), rmath::acosh_hi(x.hi()));
}

inline interval atanh(interval const &x) {
	if (x.is_empty() || (x.hi() < real_neg_one) || (x.lo() > real_one)) return interval::empty();
	return interval(
		x.lo() <= real_neg_one ? rmath::neg_inf() : rmath::atanh_lo(x.lo()),
		x.hi() >= real_one     ? rmath::pos_inf() : rmath::atanh_hi(x.hi())
	);
}


inline interval abs(interval const &x) {
	if (x.lo() > 0) return x;
	if (x.hi() < 0) return -x;
	return interval(real_zero, std::max(-x.lo(), x.hi()));
}

inline interval sgn(interval const &x) {
	if (x.hi() < 0) return interval::minus_one();
	if (x.lo() > 0) return interval::one();
	return interval::zero();
}

inline interval iv_max(interval const &x, interval const &y) { return interval(std::max(y.lo(), x.lo()), std::max(y.hi(), x.hi())); }
inline interval iv_min(interval const &x, interval const &y) { return interval(std::min(x.lo(), y.lo()), std::min(x.hi(), y.hi())); }

inline interval hull(interval const &x, interval const &y) {
	if (x.is_empty()) return y;
	else if (y.is_empty()) return x;
	return interval(std::min(x.lo(), y.lo()), std::max(x.hi(), y.hi()));
}

inline interval sqrt(interval const &x) {
	if (x.hi() < 0) return interval::empty();
	real l = (x.lo() <= 0) ? real_zero : rmath::sqrt_lo(x.lo());
	return interval(l, rmath::sqrt_hi(x.hi()));
}

inline interval square(interval const &x) {
	real xl = x.lo(), xh = x.hi();
	if (xh < 0) return interval(rmath::mul_lo(xh, xh), rmath::mul_hi(xl, xl));
	if (xl > 0) return interval(rmath::mul_lo(xl, xl), rmath::mul_hi(xh, xh));
	return interval(real_zero, (-xl > xh ? rmath::mul_hi(xl, xl) : rmath::mul_hi(xh, xh)));
}


inline interval power(interval const &x, interval const &y) {
	if (x.hi() <= 0) {
		std::string s = stringize() << "power(x, y): x <= 0 not allowed. got: " << x;
		throw iv_arithmetic_error(s);
	}
	return exp(y * log(interval(std::max(x.lo(), real_zero), x.hi())));
}

}


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
// todo: compare accuracy. benchmarks show they're much faster,
// if they're just as accurate, USE THEM
typedef __m128d vec_iv;
vec_iv _vec_iv_raw_make(double x, double y);
vec_iv vec_iv_make(double x, double y);
double vec_iv_lo(vec_iv x);
double vec_iv_hi(vec_iv x);
double vec_iv_lneg(vec_iv x);
vec_iv vec_iv_add(vec_iv x, vec_iv y);
vec_iv vec_iv_negate(vec_iv x);
vec_iv vec_iv_sub(vec_iv x, vec_iv y);
vec_iv vec_iv_mul(vec_iv x, vec_iv y);
vec_iv vec_iv_invert(vec_iv x);
vec_iv vec_iv_div(vec_iv x, vec_iv y);


pod_interval iv_vmul(pod_interval a, pod_interval b);
pod_interval iv_vsub(pod_interval a, pod_interval b);
pod_interval iv_vadd(pod_interval a, pod_interval b);
pod_interval iv_vdiv(pod_interval a, pod_interval b);


}

#endif