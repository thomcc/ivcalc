#include <stdio.h>
#include <fenv.h>

typedef double real;

static inline void set_round(int mode) { fesetround(mode); }
static inline real do_round(real d) { volatile real dd = d; return dd; }
static inline void set_rup() { set_round(FE_UPWARD); }
static inline void set_rdown() { set_round(FE_DOWNWARD); }
static inline real add_hi(real a, real b) { set_rup();   return do_round(a+b); }
static inline real add_lo(real a, real b) { set_rdown(); return do_round(a+b); }

int main() {
  real a = 0, b = 3, c = 2, d = 4;
  real e = add_lo(a, c);
  real f = add_hi(b, d);
  printf("[%lf, %lf]\n", e, f);
}


