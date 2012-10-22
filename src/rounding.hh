#ifndef __ROUNDING_HH__
#define __ROUNDING_HH__

#include "common.hh"
#include <cfenv>
#include <cmath>
#include <cassert>
namespace numbers {


struct _roundingBase {
	typedef int rounding;

	static void 
	setup() {}
	
	static void 
	get_rounding(rounding &mode) {
		mode = std::fegetround();
	}
	
	static void 
	set_rounding(rounding mode) {
		std::fesetround(mode);
	}
	
	static void 
	fe_up() {
		set_rounding(FE_UPWARD);
	}
	
	static void 
	fe_down() {
		set_rounding(FE_DOWNWARD);
	}
	
	static void 
	fe_nearest() {
		set_rounding(FE_TONEAREST);
	}
	
	static void
	fe_zero() {
		set_rounding(FE_TOWARDZERO);
	}

	static long double
	do_round(long double d) {
		volatile long double dd = d;
		return dd;
	}

	template <typename T>
	static T
	round_int(T const &x) { 
		return std::rint(x);
	}
};

struct RoundExact : _roundingBase {
	template <typename S>
	long double
	r_down(S const &n) {
		return n;
	}

	template <typename S>
	long double
	r_up(S const &n) {
		return n;
	}

	long double
	add_u(long double a, long double b) {
		return a+b;
	}

	long double
	add_d(long double a, long double b) {
		return a+b;
	}

	long double
	sub_u(long double a, long double b) {
		return a-b;
	}

	long double
	sub_d(long double a, long double b) {
		return a-b;
	}

	long double
	mul_u(long double a, long double b) {
		return a*b;
	}

	long double
	mul_d(long double a, long double b) {
		return a*b;
	}

	long double
	div_u(long double a, long double b) {
		return a/b;
	}

	long double
	div_d(long double a, long double b) {
		return a/b;
	}

	long double
	avg(long double a, long double b) {
		return (a+b)/2.0;
	}

#define FN_U_AND_D(FN)       \
	long double              \
	FN##_u(long double a) {  \
		return std :: FN(a); \
	}                        \
	long double              \
	FN##_d(long double a) {  \
		return std :: FN(a); \
	}

	FN_U_AND_D(sqrt)
	FN_U_AND_D(exp)
	FN_U_AND_D(log)
	FN_U_AND_D(sin)
	FN_U_AND_D(cos)
	FN_U_AND_D(tan)
	FN_U_AND_D(asin)
	FN_U_AND_D(acos)
	FN_U_AND_D(atan)
	FN_U_AND_D(sinh)
	FN_U_AND_D(cosh)
	FN_U_AND_D(tanh)
	FN_U_AND_D(asinh)
	FN_U_AND_D(acosh)
	FN_U_AND_D(atanh)

#undef FN_U_AND_D

	long double
	ceil(long double a) {
		return std::ceil(a);
	}

	long double
	floor(long double a) {
		return std::floor(a);
	}
};

struct RoundStandard : _roundingBase {


	template <typename S>
	long double
	r_down(S const &n) {
		fe_down();
		return do_round(n);
	}

	template <typename S>
	long double
	r_up(S const &n) {
		fe_up();
		return do_round(n);
	}

	long double
	add_u(long double a, long double b) {
		fe_up();
		return do_round(a+b);
	}

	long double
	add_d(long double a, long double b) {
		fe_down();
		return do_round(a+b);
	}

	long double
	sub_u(long double a, long double b) {
		fe_up();
		return do_round(a-b);
	}

	long double
	sub_d(long double a, long double b) {
		fe_down();
		return do_round(a-b);
	}

	long double
	mul_u(long double a, long double b) {
		fe_up();
		return do_round(a*b);
	}

	long double
	mul_d(long double a, long double b) {
		fe_down();
		return do_round(a*b);
	}

	long double
	div_u(long double a, long double b) {
		fe_up();
		return do_round(a/b);
	}

	long double
	div_d(long double a, long double b) {
		fe_down();
		return do_round(a/b);
	}

	long double
	avg(long double a, long double b) {
		fe_nearest();
		return do_round((a+b)/2.0);
	}

#define FN_U_AND_D(FN)                 \
	long double                        \
	FN##_u(long double a) {            \
		fe_up();                       \
		return do_round(std :: FN(a)); \
	}                                  \
	long double                        \
	FN##_d(long double a) {            \
		fe_down();                     \
		return do_round(std :: FN(a)); \
	}

	FN_U_AND_D(sqrt)
	FN_U_AND_D(exp)
	FN_U_AND_D(log)
	FN_U_AND_D(sin)
	FN_U_AND_D(cos)
	FN_U_AND_D(tan)
	FN_U_AND_D(asin)
	FN_U_AND_D(acos)
	FN_U_AND_D(atan)
	FN_U_AND_D(sinh)
	FN_U_AND_D(cosh)
	FN_U_AND_D(tanh)
	FN_U_AND_D(asinh)
	FN_U_AND_D(acosh)
	FN_U_AND_D(atanh)

#undef FN_U_AND_D

	long double
	ceil(long double a) {
		fe_up();
		return round_int(a);
	}

	long double
	floor(long double a) {
		fe_down();
		return round_int(a);
	}
};



struct RoundFast : _roundingBase {
	static void
	setup() {
		fe_up();
	}

	template <typename S>
	long double
	r_down(S const &n) {
		return -do_round(-n);
	}

	template <typename S>
	long double
	r_up(S const &n) {
		return do_round(n);
	}

	long double
	add_u(long double a, long double b) {
		return do_round(a+b);
	}

	long double
	add_d(long double a, long double b) {
		return -do_round(((-a) - b));
	}

	long double
	sub_u(long double a, long double b) {
		return do_round(a - b);
	}

	long double
	sub_d(long double a, long double b) {
		return -do_round(b - a);
	}

	long double
	mul_u(long double a, long double b) {
		return do_round(a * b);
	}

	long double
	mul_d(long double a, long double b) {
		return -do_round(a * (-b));
	}

	long double
	div_u(long double a, long double b) {
		return do_round(a / b);
	}

	long double
	div_d(long double a, long double b) {
		return -do_round(a / (-b));
	}

	long double
	avg(long double a, long double b) {
		fe_nearest();
		long double c = do_round((a+b)/2);
		fe_up();
		return c;
	}

#define FN_U_AND_D(FN)                          \
	long double                                 \
	FN##_u(long double a) {                     \
		return do_round(std :: FN(a));          \
	}                                           \
	long double                                 \
	FN##_d(long double a) {                     \
		fe_down();                              \
		long double d = do_round(std :: FN(a)); \
		fe_up();                                \
		return d;                               \
	}

	FN_U_AND_D(sqrt)
	FN_U_AND_D(exp)
	FN_U_AND_D(log)
	FN_U_AND_D(cos)
	FN_U_AND_D(acos)
	FN_U_AND_D(cosh)
	FN_U_AND_D(asinh)
	FN_U_AND_D(atanh)

#undef FN_U_AND_D
#define FN_U_AND_D(FN)                          \
	long double                                 \
	FN##_u(long double a) {                     \
		return do_round(std :: FN(a));          \
	}                                           \
	long double                                 \
	FN##_d(long double a) {                     \
		return -do_round(-(std :: FN(a)));      \
	}

	FN_U_AND_D(sin)
	FN_U_AND_D(tan)
	FN_U_AND_D(asin)
	FN_U_AND_D(atan)
	FN_U_AND_D(sinh)
	FN_U_AND_D(tanh)
	FN_U_AND_D(acosh)

#undef FN_U_AND_D

	long double
	ceil(long double a) {
		return -round_int(-a);
	}

	long double
	floor(long double a) {
		return round_int(a);
	}
};
}









#endif