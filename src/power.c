/* Copyright 2004 Spencer Eugene Olson --- All Rights Reserved
 * computes x^y at least twice as fast as pow(...) from gnu math library and
 * several times faster than NAG_pow from NAG library.
 *
 */

#include "power.h"

#ifdef USE_SPENCERS_FAST_POW

#define FAST_POW_CODE(derefd_x, derefd_y)			\
    register double value;					\
								\
    /*								\
      here's the pseudo code for the following:			\
								\
    if (y == 0) return 1;					\
    if (x == 0) return 0;					\
								\
    return x^y;							\
    */								\
    asm (         						\
      /* first test for y == 0 */				\
      "fxam			# st(0) == 0 ? \n\t"		\
      "fnstsw %%ax		# mov ax, fpu status word\n\t"	\
      /* the fpu status word condition bits after fnstsw are	\
       * x1xxx0x0 xxxxxxxx					\
       * if st(0) == +-0					\
       *  Also note that 0x4500 ==  01000101 00000000		\
       *  and            0x4000 ==  01000000 00000000		\
       */							\
      "and $0x45,%%ah		# fpu status word & ??? \n\t"	\
      "cmp $0x40,%%ah		# \n\t"				\
      "je 1f			# jne (y == 0; return1;) \n\t"	\
      /* now test the x value */				\
      "fxch			#\n\t"				\
      "fxam			# st(0) == 0 ? \n\t"		\
      "fnstsw %%ax		# mov ax, fpu status word\n\t"	\
      /* the fpu status word condition bits after fnstsw are	\
       * x1xxx0x0 xxxxxxxx					\
       * if st(0) == +-0					\
       *  Also note that 0x4500 ==  01000101 00000000		\
       *  and            0x4000 ==  01000000 00000000		\
       */							\
      "and $0x45,%%ah		# fpu status word & ??? \n\t"	\
      "cmp $0x40,%%ah		# \n\t"				\
      "je 2f			# jne (x == 0; return 0;) \n\t"	\
      /* now finally do the hard work of computing x^y */	\
      "fyl2x			# st(1) * log2(ST(0))\n\t"	\
      "fld	%%st(0)		#\n\t"				\
      "frndint			# int(x)\n\t"			\
      "fxch			#\n\t"				\
      "fsub	%%st(1)		# fract(x)\n\t"			\
      "f2xm1			# 2^(ST(0)) - 1\n\t"		\
      "fld1			#\n\t"				\
      "faddp  %%st(0),%%st(1)	#\n\t"				\
      "fscale			#\n\t"				\
      /* hard work done! */					\
      "jmp 3f			# jmp finished \n\t"		\
"1:				# y == 0; return 1; \n\t"	\
      "fld1			# put 1 in st(0) \n\t"		\
      "fstp   %%st(1)		# \n\t"				\
      "jmp 3f			# jmp finished \n\t"		\
"2:				# x == 0; return 0; \n\t"	\
      "fldz			# put 0 in st(0) \n\t"		\
      "fstp   %%st(1)		# \n\t"				\
      /*"jmp 3f			# jmp finished \n\t" */		\
"3:				# finished \n\t"		\
      : "=t" (value) : "f" ((derefd_x)), "0" ((derefd_y))	\
      : "eax"                                                   \
    );								\
								\
    return value;						\
    /*end fast pow code */


/* for (g77) fortran we need to have pointer arguments as well as derefencing */
double fast_pow__( double *x, double *y ) {
    FAST_POW_CODE(*x,*y);
}

double fast_pow( double x, double y ) {
    FAST_POW_CODE(x,y);
}

#endif // USE_SPENCERS_FAST_POW
