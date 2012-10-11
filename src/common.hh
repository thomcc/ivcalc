#ifndef __COMMON_HH__
#define __COMMON_HH__

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cmath>

#define EXPECT(CND, FMT, ...) do {\
	if (!(CND)) {\
		fprintf(stderr, "Expectation failure: %s::%d " FMT "\n" , __FILE__, __LINE__, ##__VA_ARGS__);\
		exit(1);\
	}\
} while (0)

#define DISALLOW_COPY_AND_SWAP(CLASSNAME) \
	CLASSNAME(const CLASSNAME&); \
	CLASSNAME &operator=(const CLASSNAME&)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#endif