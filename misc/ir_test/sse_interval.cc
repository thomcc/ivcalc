#include "xmmintrin.h"
#include "math.h"

typedef __m128d vec_iv;

vec_iv _vec_iv_raw_make(double x, double y) {
	return (vec_iv){x, y};
}

vec_iv vec_iv_make(double x, double y) {
	return (vec_iv){-x, y};
}




double vec_iv_lo(vec_iv x) { return -x[0]; }
double vec_iv_hi(vec_iv x) { return x[1]; }
double vec_iv_lneg(vec_iv x) { return x[0]; }
vec_iv vec_iv_add(vec_iv x, vec_iv y) { return x + y; }

// swap high and lo parts to negate
vec_iv vec_iv_negate(vec_iv x) { return _mm_shuffle_pd(x, x, 1); }
vec_iv vec_iv_sub(vec_iv x, vec_iv y) { return x + vec_iv_negate(y); }


static inline vec_iv _vec_ivswap(vec_iv x, vec_iv cond) {
	vec_iv t = _mm_xor_pd((vec_iv)_mm_shuffle_epi32((__m128i)x, 0x4e), x);
	return _mm_xor_pd(_mm_and_pd(cond, t), x);
}

vec_iv vec_iv_mul(vec_iv x, vec_iv y) {
	vec_iv t1 = (vec_iv)_mm_shuffle_epi32((__m128i)x, 0xee);
	vec_iv t2 = (vec_iv)_mm_shuffle_epi32((__m128i)y, 0xee);
	vec_iv t3 = (vec_iv)_mm_xor_pd(x, t1);
	vec_iv t4 = (vec_iv)_mm_xor_pd(y, t2);
	if (_mm_movemask_pd(_mm_and_pd(t3, t4))) {
		vec_iv c = {0.0, 0.0};
		vec_iv c1 = _mm_cmple_pd(t2, c);
		vec_iv c2 = _mm_cmple_pd(t1, c);
		vec_iv c3 = (vec_iv){-0.0, 0.0};
		x = _vec_ivswap(_mm_xor_pd(x, c3), c1);
		y = _vec_ivswap(_mm_xor_pd(y, c3), c2);
		return x * _mm_xor_pd(y, c3);
	}
	t1 = (vec_iv)_mm_shuffle_epi32((__m128i)x, 0x43) * _mm_unpacklo_pd(y, y);
	t1 *= x;
	return _mm_max_pd(t1, t2);
}

vec_iv vec_iv_invert(vec_iv x) {
	if (_mm_movemask_pd(_mm_cmple_pd((vec_iv){0.0}, x)) == 3)
		return _vec_iv_raw_make(INFINITY, INFINITY);
	x = _mm_shuffle_pd(x, x, 1);
	return _mm_div_pd((vec_iv){-1, -1}, x);
}

vec_iv vec_iv_div(vec_iv x, vec_iv y) {
	return vec_iv_mul(x, vec_iv_negate(x));
}

vec_iv vec_iv_sqrt(vec_iv x) {
	if (vec_iv_lo(x) < 0) return vec_iv_make(NAN, NAN);
	// counteract unrounded sqrt below (leaves interval slightly
	// larger than necessary, but )
	x *= (vec_iv){-0x1.ffffffffffff8p-1, 1.0};
	x = _mm_sqrt_pd(x);
	return _mm_xor_pd(x, (vec_iv){-0.0, 0.0});
}



