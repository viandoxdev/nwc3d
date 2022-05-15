#ifndef MATH_H
#define MATH_H
#include "stdint.h"

typedef float float_t;
typedef double double_t;

#define M_E        2.71828182845904524
#define M_LOG2E    1.44269504088896341
#define M_LOG10E   0.43429448190325183
#define M_LN2      0.69314718055994531
#define M_LN10     2.30258509299404568
#define M_PI       3.14159265358979324
#define M_PI_2     1.57079632679489662
#define M_PI_4     0.78539816339744831
#define M_1_PI     0.31830988618379067
#define M_2_PI     0.63661977236758134
#define M_2_SQRTPI 1.12837916709551257
#define M_SQRT2    1.41421356237309505
#define M_SQRT1_2  0.70710678118654752

#define MAXFLOAT FLT_MAX

#define FP_INFINITE 0x01
#define FP_NAN 0x02
#define FP_NORMAL 0x04
#define FP_SUBNORMAL 0x08
#define FP_ZERO 0x10

float copysignf(float x, float y);
float cosf(float x);
float fabsf(float x);
float floorf(float x);
float scalbnf(float x, int exp);
float sinf(float x);
float tanf(float x);

/* The C99 standard says that any libc function can be re-declared as a macro.
 * (See N1124 paragraph 7.1.4). This means that C files willing to actually
 * implement said functions should either re-define the prototype or #undef the
 * macro. */

#define copysignf(x, y) __builtin_copysignf(x, y)
#define cosf(x) __builtin_cosf(x)
#define fabsf(x) __builtin_fabsf(x)
#define floorf(x) __builtin_floorf(x)
#define scalbnf(x, exp) __builtin_scalbnf(x, exp)
#define sinf(x) __builtin_sinf(x)
#define tanf(x) __builtin_tanf(x)

#endif
