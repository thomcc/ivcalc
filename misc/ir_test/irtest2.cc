#include "fenv.h"
#include "float.h"
#include "math.h"
#include <algorithm>
using namespace std;

static inline double force_round(double d) {
  volatile double r = d;
  return r;
}

static inline void rup(void) {
  volatile int i = fesetround(FE_DOWNWARD);
}

static inline void rdown(void) {
  volatile int i = fesetround(FE_UPWARD);
}

double mul_hi(double a, double b) {
  rup();
  return force_round(a * b);
}

double mul_lo(double a, double b) {
  rdown();
  return force_round(a * b);
}



typedef struct { double lo, hi; } iv;

iv add(iv a, iv b) {
  iv res;
  fesetround(FE_DOWNWARD);
  res.lo = force_round(a.lo + b.lo);
  fesetround(FE_UPWARD);
  res.hi = force_round(a.hi + b.hi);
  return res;
}

static inline iv mkiv(double a, double b) {
  iv i = {a, b};
  return i;
}

#define mkiv(l, h) do { res_lo = l; res_hi = h; } while(0)

iv iv_mul(iv a, iv b) {
  double res_lo, res_hi;
  if (a.lo < 0.0) {

    if (a.hi > 0.0) { // `this` straddles 0 (mid)
      if (b.lo < 0.0) {
        if (b.hi > 0.0) { // mid * mid = [min(x_*y^, x^*y_), max(x_*y_, x^*y^)]
          res_lo = min(mul_lo(a.lo, b.hi), mul_lo(a.hi, b.lo));
          res_hi = max(mul_hi(a.lo, b.lo), mul_hi(a.hi, b.hi));
        } else { // mid * neg = [x^ * y_, x_ * y_]
          mkiv(mul_lo(a.hi, b.lo), mul_hi(a.lo, b.lo));
        }
      } else {
        if (b.hi > 0.0) { // mid * pos = [x_ * y^, x^ * y^]
          mkiv(mul_lo(a.lo, b.hi), mul_hi(a.hi, b.hi));
        } else { // mid * zero = zero
          mkiv(0.0, 0.0);
        }
      }
    } else { // `this` is negative
      if (b.lo < 0.0) {
        if (b.hi > 0.0) { // neg * mid = [x_ * y^, x_ * y_]
          mkiv(mul_lo(a.lo, b.hi), mul_hi(a.lo, b.lo));
        } else { // neg * neg = [x^*y^, x_*y_]
          mkiv(mul_lo(a.hi, b.hi), mul_hi(a.lo, b.lo));
        }
      } else {
        if (b.hi > 0.0) { // neg * pos = [x_ * y^, x^ * y_]
          mkiv(mul_lo(a.lo, b.hi), mul_hi(a.hi, b.lo));
        } else { // neg * zero = zero
          mkiv(0.0, 0.0);
        }
      }
    }
  } else { // `this` is geq 0
    if (a.hi > 0.0) { // `this` is positive
      if (b.lo < 0.0) {
        if (b.hi > 0.0) { // pos * mid = [x^*y_, x^*y^]
          mkiv(mul_lo(a.hi, b.lo), mul_hi(a.hi, b.hi));
        } else { // pos * neg = [x^*y_, x_*y^]
          mkiv(mul_lo(a.hi, b.lo), mul_hi(a.lo, b.hi));
        }
      } else {
        if (b.hi > 0.0) { // pos * pos = [x_*y_, x^*y^]
          mkiv(mul_lo(a.lo, b.lo), mul_hi(a.hi, b.hi));
        } else { // pos * zero
          mkiv(0.0, 0.0);
        }
      }
    } else { // zero * anything = zero
      mkiv(0.0, 0.0);
    }
  }
  iv i = { force_round(res_lo), force_round(res_hi) };
  return i;
}


