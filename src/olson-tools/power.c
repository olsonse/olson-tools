/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2004-2009 Spencer E. Olson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *  
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *                                                                                 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.                                                                           .
 * 
 * Questions? Contact Spencer Olson (olsonse@umich.edu) 
 */

/* Copyright 2004-2009 Spencer Eugene Olson --- All Rights Reserved
 * computes x^y up to six times as fast as pow(...) from gnu math library and
 * several times faster than NAG_pow from NAG library.
 *
 */

#include <olson-tools/power.h>

#ifdef USE_SPENCERS_FAST_POW

/* for (g77) fortran we need to have pointer arguments as well as derefencing */
double fast_pow_( double *x, double *y ) {
    FAST_POW_CODE(*x,*y);
}

double fast_pow( double x, double y ) {
    FAST_POW_CODE(x,y);
}


/* I couldn't figure out how to load NAN and INF into st(0) so the tests are
 * commented out.  The FPU already returns the correct error values, but it is
 * significantly slower to do through the FPU.  I think it doesn't matter much
 * anyway since when there is an error, the user should probably not be worrying
 * about timing but rather fixing the error.
 */
double fast_log2 ( double x ) {
  FAST_LOG2_CODE(x);
/* the old code is similar except for unfinished test pieces. */
#if 0
  register double value;
  asm (
#ifdef FAST_LOG2_TEST_INPUT
    "ftst       # compare ( st(0), 0.0 )      \n"
    "fstsw %%ax # ax = fpu status word        \n" // checks for fp exceptions first
    "fwait      # wait for fstsw to complete  \n"
    ///* the fpu status word condition bits after fnstsw are
    // * layout :  B  c3     top    c2  c2  c0      ir  sf  p  u  o  z  d  i
    // *          15  14  13 12 11  10   9   8       7  6   5  4  3  2  1  0
    // *
    // * results of ftst:
    // *                   C3   C2   C0
    // *  If ST(0) > 0      0    0    0
    // *  If ST(0) < 0      0    0    1
    // *  If ST(0) = 0      1    0    0
    // *  If ST(0) ? 0      1    1    1
    // *
    // * x1xxx0x0 xxxxxxxx
    // * if st(0) == +-0
    // *  Also note that 0x4500 ==  01000101 00000000
    // *  and            0x4000 ==  01000000 00000000
    // */
    //"and $0x45,%%ah   # zero out un-important bits \n"
    //"cmp $0x40,%%ah   # \n"
    //"je 1f      # jne (y == 0; return1;) \n"
    "sahf       # EFLAGS = ax                 \n"
    "jpe 1f     # st(0) was invalid:return nan\n"
    "jz  2f     # st(0) == 0                  \n"
    "jb  1f     # st(0) < 0        :return nan\n"
#endif


    "fld1       # st(0) = 1                   \n"
    "fxch       # swap( st(0), st(1) )        \n"
    "fyl2x      # st(0) = st(1) * log2(st(0)) \n"

#ifdef FAST_LOG2_TEST_INPUT
    "jmp 3f     # shortcut to finish          \n"

    "1:         # return (nan)  ( = log(x<0) )\n"
    " jmp 3f    # shortcut to finish          \n"
    "2:         # return (-inf) ( = log(0) )  \n"
    " jmp 3f    # shortcut to finish          \n"
    "3:         # the finish                  \n"
#endif

    : "=t" (value) : "0" (x)
#ifdef FAST_LOG2_TEST_INPUT
    : "eax" // We clobber the ax register by using the fstsw instruction
#endif
  );

  return value;
#endif
}

/* for (g77) fortran we need to have pointer arguments as well as derefencing */
double fast_log2_( double * x ) {
  FAST_LOG2_CODE(*x); 
}

#endif // USE_SPENCERS_FAST_POW

double logn ( double n, double x ) {
  return fast_log2( x ) / fast_log2( n );
}

/* a fortran callable version */
double logn_( double * n, double *x ) {
  return logn(*n,*x);
}

