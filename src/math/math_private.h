/*	$OpenBSD: math_private.h,v 1.17 2014/06/02 19:31:17 kettenis Exp $	*/
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/*
 * from: @(#)fdlibm.h 5.1 93/09/24
 */

#ifndef _MATH_PRIVATE_H_
#define _MATH_PRIVATE_H_

#include <stdint.h>

// sys/types.h =================
#define LITTLE_ENDIAN  0x1234
#define BIG_ENDIAN     0x4321
#define BYTE_ORDER LITTLE_ENDIAN

typedef uint32_t u_int32_t;
typedef uint64_t u_int64_t;
// =============================

/* The original fdlibm code used statements like:
	n0 = ((*(int*)&one)>>29)^1;		* index of high word *
	ix0 = *(n0+(int*)&x);			* high word of x *
	ix1 = *((1-n0)+(int*)&x);		* low word of x *
   to dig two 32 bit words out of the 64 bit IEEE floating point
   value.  That is non-ANSI, and, moreover, the gcc instruction
   scheduler gets it wrong.  We instead use the following macros.
   Unlike the original code, we determine the endianness at compile
   time, not at run time; I don't see much benefit to selecting
   endianness at run time.  */

/* A union which permits us to convert between a float and a 32 bit
   int.  */

typedef union
{
  float value;
  u_int32_t word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)					\
do {								\
  ieee_float_shape_type gf_u;					\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (0)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)					\
do {								\
  ieee_float_shape_type sf_u;					\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (0)

/* float versions of fdlibm kernel functions */
extern int   __ieee754_rem_pio2f(float,float*);
extern float __kernel_sinf(float,float,int);
extern float __kernel_cosf(float,float);
extern float __kernel_tanf(float,float,int);
extern int   __kernel_rem_pio2f(float*,float*,int,int,int,const int*);

#endif /* _MATH_PRIVATE_H_ */
