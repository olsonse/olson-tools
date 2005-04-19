// -*- c++ -*-
// $Id: random.C,v 1.2 2005/04/19 17:23:39 olsonse Exp $
/*
 * Copyright 1997-2004 Spencer Olson
 *
 * $Log: random.C,v $
 * Revision 1.2  2005/04/19 17:23:39  olsonse
 * Various other fixes for new compilers.
 *
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
 *
 * Revision 1.2  2004/04/13 15:54:15  labrat
 * Simplified and cleaned up gauss_deviate which is used to create the input
 * beam.
 * gauss_deviate now accepts only one parameter (sigma of distribution).
 *
 * Revision 1.1.1.1  2003/05/18 02:05:10  olsonse
 * Initial import of atomguide into cvs.
 * This marks the change from a running version that did not really do cells
 * correctly, to one where cells will correctly be done (hopefully).
 *
 *
 */

#include "MersenneTwister.h"
#include <math.h>


#define gauss_deviate gauss_deviate__
#define MTRNGrand mtrngrand_
#define MTRNGrandV mtrngrandv_
#define MTRNGrandExc mtrngrandexc_
#define MTRNGrandExcV mtrngrandexcv_
#define MTRNGrandDblExc mtrngranddblexc_
#define MTRNGrandDblExcV mtrngranddblexcv_
#define MTRNGrandInt mtrngrandint_
#define MTRNGrandIntV mtrngrandintv_
#define MTRNGseedV1 mtrngrandv1_
#define MTRNGseedV2 mtrngrandv2_
#define MTRNGseed mtrngseed_
#define MTRNGsave mtrngsave_
#define MTRNGload mtrngload_

MTRand __my_rand = MTRand();

typedef unsigned long uint32;

void function_to_shut_compiler_up();

extern "C" {
    /* static */ double gauss_deviate( const double sigma[]);
    /* static */ double MTRNGrand();				/* real number in [0,1] */
    /* static */ double MTRNGrandV( const double n[] );	/* real number in [0,n] */
    /* static */ double MTRNGrandExc();			/* real number in [0,1) */
    /* static */ double MTRNGrandExcV( const double n[] );	/* real number in [0,n) */
    /* static */ double MTRNGrandDblExc();			/* real number in (0,1) */
    /* static */ double MTRNGrandDblExcV( const double n[] );	/* real number in (0,n) */
    /* static */ uint32 MTRNGrandInt();			/* integer in [0,2^32-1] */
    /* static */ uint32 MTRNGrandIntV( const uint32 n[] );	/* integer in [0,n] for n < 2^32 */
	
    // Re-seeding functions with same behavior as initializers
    /* static */ void   MTRNGseedV1( uint32 * oneSeed );
    /* static */ void   MTRNGseedV2( uint32 *const bigSeed );
    /* static */ void   MTRNGseed();
	
    // Saving and loading generator state
    /* static */ void   MTRNGsave( uint32* saveArray );  // to array of size SAVE
    /* static */ void   MTRNGload( uint32 *const loadArray );  // from such array
}// extern

/* static */ double MTRNGrand() {				/* real number in [0,1] */
    return __my_rand();
}

/* static */ double MTRNGrandV( const double n[] ) {	/* real number in [0,n] */
    return __my_rand.rand(*n);
}

/* static */ double MTRNGrandExc() {			/* real number in [0,1) */
    return __my_rand.randExc();
}

/* static */ double MTRNGrandExcV( const double n[] ){	/* real number in [0,n) */
    return __my_rand.randExc(*n);
}

/* static */ double MTRNGrandDblExc() {			/* real number in (0,1) */
    return __my_rand.randDblExc();
}

/* static */ double MTRNGrandDblExcV( const double n[] ) {	/* real number in (0,n) */
    return __my_rand.randDblExc(*n);
}

/* static */ uint32 MTRNGrandInt() {			/* integer in [0,2^32-1] */
    return __my_rand.randInt();
}

/* static */ uint32 MTRNGrandIntV( const uint32 n[] ) {	/* integer in [0,n] for n < 2^32 */
    return __my_rand.randInt(*n);
}
	
// Re-seeding functions with same behavior as initializers
/* static */ void MTRNGseedV1( uint32 * oneSeed ) {
    __my_rand.seed(*oneSeed);
}

/* static */ void MTRNGseedV2( uint32 *const bigSeed ) {
    __my_rand.seed(bigSeed);
}

/* static */ void MTRNGseed() {
    __my_rand.seed();
}
	
// Saving and loading generator state
/* static */ void MTRNGsave( uint32* saveArray ) {  // to array of size SAVE
    __my_rand.save(saveArray);
}

/* static */ void MTRNGload( uint32 *const loadArray ) {
    __my_rand.load(loadArray);
}

/** The following is for getting a gaussian distribution of random
 * numbers.  This function uses a Box-Mueller transformation to get the normal
 * deviates (as described in Numerical Recipes chapter 7 section 2).
 * This function returns a normally distributed deviate with zero
 * mean and arbitrary variance, using ????? as the source of uniform deviates.
 *@memo Random Gaussian Deviate
*/
/* static */ double gauss_deviate( const double sigma[]) {
  static int iset = 0;
  static double gset, csigma = 0;

  if ( iset == 1 && *sigma == csigma ) { // We have an extra deviate handy, so
    iset = 0; // Unset flag.
    return gset; // and return the extra deviate.
  } else {
    double fac, r, v1, v2;
    csigma = *sigma;

    do {
      /* We don't have an extra deviate handy, so
       * pick two uniform number in the square extending from -1 to +1
       * in each direction.  See if they are in the unit circle, and
       * if they are not, try again.
      */
#define LGR_RANGE_RAND	(2.0*__my_rand() - 1.0)
      v1 = LGR_RANGE_RAND;
      v2 = LGR_RANGE_RAND;
#undef LGR_RANGE_RAND
      r = v1 * v1 + v2 * v2;
    } while ( r >= 1.0 );
    fac = sqrt( -2.0 * log( r ) / r );
    /* Now make the Box-Muller transformation to get two normal deviates.
     * Return one and save the other for the next time.
     * We will attempt to rescale the deviate to the
     * range given.
    */
    gset = v1 * fac * csigma;
    iset = 1; // Set flag.
    return v2 * fac * csigma;
  }
}// gauss_deviate


void function_to_shut_compiler_up() {
    double d = 1;
    uint32 ui = 1;
    uint32 uiA[MTRand::SAVE];
    gauss_deviate( &d );
    MTRNGrand();				/* real number in [0,1] */
    MTRNGrandV( &d );	/* real number in [0,n] */
    MTRNGrandExc();			/* real number in [0,1) */
    MTRNGrandExcV( &d );	/* real number in [0,n) */
    MTRNGrandDblExc();			/* real number in (0,1) */
    MTRNGrandDblExcV( &d );	/* real number in (0,n) */
    MTRNGrandInt();			/* integer in [0,2^32-1] */
    MTRNGrandIntV( &ui );	/* integer in [0,n] for n < 2^32 */
	
    // Re-seeding functions with same behavior as initializers
    MTRNGseedV1( &ui );
    MTRNGseedV2( &ui );
    MTRNGseed();
	
    // Saving and loading generator state
    MTRNGsave( uiA );  // to array of size SAVE
    MTRNGload( uiA );  // from such array
}
