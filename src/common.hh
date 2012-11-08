#ifndef __COMMON_HH__
#define __COMMON_HH__

#include <cstdio>
//#include <iosfwd>
#include <cstdlib>
#include <cmath>
#include <memory>
#include <iostream>
#include <cassert>

// assert at runtime that a condition with a message is true.
// runs even when NDEBUG is defined, unlike the macros in <cassert>.
#define EXPECT(CND, FMT, ...) do {\
	if (!(CND)) {\
		fprintf(stderr, "[ERROR] %s:%s:%d " FMT "\n" , __FILE__, __func__, __LINE__, ##__VA_ARGS__);\
		exit(1);\
	}\
} while (0)

// assert a condition with no message at runtime.
#define ASSERT(CND) EXPECT(CND, "Assertation %s is false.", #CND)
// assert at runtime that something isn't null.
#define ASSERT_NOT_NULL(VAL) EXPECT((VAL) != NULL, "Expected %s to be non-null", #VAL)
// assert at runtime that the value is within range
#define RANGE_CHECK(V, MAX) EXPECT((((V) >= 0) && ((V) < (MAX))), "Expected %d to be in range [0, %d)", ((int)(V)), ((int)(MAX)))


// needs to be in a `private` section, and will expand to the copy constructor
// and the assignment operator, disallowing, as its name suggests,
// copy and swap
#define DISALLOW_COPY_AND_SWAP(CLASSNAME) \
	CLASSNAME(CLASSNAME const&); \
	CLASSNAME &operator=(CLASSNAME const&)

// just in case.
#undef min
#undef max

// make macro usage explicit
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define MIN3(a, b, c) MIN(MIN(a, b), c)
#define MAX3(a, b, c) MAX(MAX(a, b), c)

#define MIN4(a, b, c, d) MIN(MIN(a, b), MIN(c, d))
#define MAX4(a, b, c, d) MAX(MAX(a, b), MAX(c, d))


// paste two tokens.  needs 2 layers of indirections to get macros to expand
// right e.g. if __LINE__ is 10, foo##__LINE__ would expand to foo__LINE__,
// but TOKENPASTE(foo, __LINE__) would expand to foo10
#define TOKENPASTE_I(A, B) A ## B
#define TOKENPASTE(A, B) TOKENPASTE_I(A, B)
// same as above but for the # preprocessor operator (stringize something)
#define STRINGIZE_I(X) #X
#define STRINGIZE(X) STRINGIZE_I(X)

// assert something at compile time
#define STATIC_ASSERT(e) extern char (*TOKENPASTE(static_assertation, __LINE__)(void)) [sizeof(char[1 - 2*!(e)])]

// assert that t extends s (specifically, that s can be assigned to t).
// need to be real types (can't be foward declared).
#define STATIC_ASSERT_SUBTYPE(T, S) \
while (false) { \
  *(static_cast<T* volatile*>(0)) = static_cast<S*>(0); \
}



#ifndef __has_attribute
#  define __has_attribute(X) 0
#endif

namespace calc {
typedef double real;
#define REAL_FMT "%lf"
class interval;
class Expr;
class AddExpr;
class SubExpr;
class NegExpr;
class MulExpr;
class DivExpr;
class VarExpr;
class ExptExpr;
class LitExpr;
class CallExpr;
class EmptyExpr;
class FuncExpr;


typedef std::shared_ptr<Expr> ExprSPtr;
//typedef std::unique_ptr<Expr> ExprPtr;
}


#endif