// -*- c -*-
// $Id: power.h,v 1.2 2005/05/12 04:27:34 olsonse Exp $
/*
 * Copyright 2004 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: power.h,v $
 * Revision 1.2  2005/05/12 04:27:34  olsonse
 * Fixed to for Intel 8.1 compilers.
 * Found (using intel compiler) and fixed an array overflow in BField::potential.
 * Didn't find it earlier because the array is on the stack for the function.
 *
 * Added fmacros.h file to simplify mixing fortran code with others.
 * Added alias function names for Fortran interoperability.
 *
 * Revision 1.1.1.1  2005/01/08 04:27:24  olsonse
 * Initial import
 *
 */

/** \file
 * Insanity drove be to write my own pow function.
 *
 * Copyright Spencer Olson 2004
 * 
 * Because of my own insanity, I wrote my own pow function.
 * pow(x,y) == \f$ x^{y} \f$
 *
 * Note that this does not necessarily perform exactly according to ieee754 standards,
 * but oh well; it is at least twice as fast though.
 * 
 * doing the following showed a total error of ~1.0e-12:
 * \verbatim
   x = 1.9
   for y = -1000:.01:1000
      tot_err += fabs( (pow(x,y) - fast_pow(x,y))/pow(x,y) )
   end
   \endverbatim
 *
 * With another test, I was able to get the total error up to 1.70605e-10 by
 * \verbatim
   for x = -100:0.1:110
       for y = -150:0.01:150
           tot_err += fabs( (pow(x,y) - fast_pow(x,y))/pow(x,y) )
       end
   end
   \endverbatim
 *
 */

#ifndef MY_OWN_POW
#define MY_OWN_POW

#  ifdef __cplusplus
extern "C" {
#  endif 

#ifdef USE_SPENCERS_FAST_POW

/** Because of my own insanity, I wrote my own pow function.
 * Call this one for c/c++ code.  It will be every so infinitesimally faster
 * than fast_pow__.
 * @param x The base.
 * @param y The exponent.
 * @return \f$ x^{y} \f$
 * */
double fast_pow(double x,double y);

/** This one is primarily for use from (g77) fortran code.
 * It has only 2 additional mov instructions. */
double fast_pow__(double*,double*);

/** This one is primarily for use from (NAG f95) fortran code.
 * It has only 2 additional mov instructions. */
double fast_pow_(double*,double*) __attribute__ (( alias ("fast_pow__") )) ;

#endif // USE_SPENCERS_FAST_POW

#  ifdef __cplusplus
}
#  endif 

#endif // MY_OWN_POW
