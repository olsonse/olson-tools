// -*- c++ -*-
// $Id: random.h,v 1.1 2005/01/08 04:27:25 olsonse Exp $
/*
 * Copyright 1997-2004 Spencer Olson
 *
 * $Log: random.h,v $
 * Revision 1.1  2005/01/08 04:27:25  olsonse
 * Initial revision
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

#ifndef MY_RANDOM_H
#define MY_RANDOM_H

/** \file
 * Several random number utilities.
 * By default, each of these will use a pre-initialized MTRand (MersenneTwister) class
 * to allocate random numbers.  This private MTRand instance will be
 * initialized via MTRand::seed().
 *
 * NOTE:
 * The screwy necessity of having pointers to arguments for gauss_deviate and
 * MTRNGseedV1 is because I want to make sure that this is all callable from
 * fortran.  Remember that fortran ONLY passes by reference into functions.
 *
 * @see MersenneTwister.h.
 * */

#  ifdef __cplusplus
   /* include this here in case some c++ module wants to directly access the
    * RNG class. 
    * */
#    include "MersenneTwister.h"
#  endif 

typedef unsigned long uint32;

#  ifdef __cplusplus

/** pre-allocated MersenneTwister class for use in generating random numbers
 * using the following routines.
 */
extern MTRand __my_rand;

extern "C" {
#  else
#  endif
#  ifndef DOXYGEN_SKIP
#   define gauss_deviate gauss_deviate__
#  endif // DOXYGEN_SKIP
    /** Generate a Gaussian deviate.
     * The random gaussian distribution is created using a Box-Mueller
     * transformation from a uniform distribution.
     * @param sigma
     *    The standard deviation of the resulting gaussian
     *    distribution.
     * @return Random number from a gaussian distribution with specified
     *    standard deviation; possible values are (-Inf,+Inf).
     * @see Numerical recipes for a description of a Box-Mueller
     *    transformation.
     * */
    double gauss_deviate( double * sigma);

#  ifndef DOXYGEN_SKIP
#   define ran2 ran2_
#  endif // DOXYGEN_SKIP
    /** RAN2 from numerical recipes. */
    double ran2(int * idum);			/* numerical recip. ran2 */

#  ifndef DOXYGEN_SKIP
#   define MTRNGrand mtrngrand_
#  endif // DOXYGEN_SKIP

#  ifdef __cplusplus
    /** Real number in [0,1].
     * This definition is for the c++ compiler when -O3 is used.  Otherwise,
     * it appears that the C version is linked to.
     * @see MTRand::rand().
     */
    inline double MTRNGrand() {				/* real number in [0,1] */
        return __my_rand();
    }
#else
    /** Real number in [0,1].
     * @see MTRand::rand().
     */
    double MTRNGrand();				/* real number in [0,1] */
#  endif

#  ifndef DOXYGEN_SKIP
#   define MTRNGrandV mtrngrandv_
#  endif // DOXYGEN_SKIP
#  ifdef __cplusplus
    /** Real number in [0,n].
     * This definition is for the c++ compiler when -O3 is used.  Otherwise,
     * it appears that the C version is linked to.
     * @see MTRand::rand( const double& n ).
     */
    inline double MTRNGrandV( const double * n ) {	/* real number in [0,n] */
        return __my_rand.rand(*n);
    }
#else
    /** Real number in [0,n].
     * @see MTRand::rand( const double& n ).
     */
    double MTRNGrandV( const double * n );	/* real number in [0,n] */
#endif

#  ifndef DOXYGEN_SKIP
#   define MTRNGrandExc mtrngrandexc_
#  endif // DOXYGEN_SKIP
#  ifdef __cplusplus
    /** Real number in [0,1).
     * This definition is for the c++ compiler when -O3 is used.  Otherwise,
     * it appears that the C version is linked to.
     * @see MTRand::randExc().
     */
    inline double MTRNGrandExc() {			/* real number in [0,1) */
        return __my_rand.randExc();
    }
#else
    /** Real number in [0,1).
     * @see MTRand::randExc().
     */
    double MTRNGrandExc();			/* real number in [0,1) */
#endif

#  ifndef DOXYGEN_SKIP
#   define MTRNGrandExcV mtrngrandexcv_
#  endif // DOXYGEN_SKIP
#  ifdef __cplusplus
    /** Real number in [0,n).
     * This definition is for the c++ compiler when -O3 is used.  Otherwise,
     * it appears that the C version is linked to.
     * @see MTRand::randExc( const double& n).
     */
    inline double MTRNGrandExcV( const double * n ) {	/* real number in [0,n) */
        return __my_rand.randExc(*n);
    }
#else
    /** Real number in [0,n).
     * @see MTRand::randExc( const double& n).
     */
    double MTRNGrandExcV( const double * n );	/* real number in [0,n) */
#endif

#  ifndef DOXYGEN_SKIP
#   define MTRNGrandDblExc mtrngranddblexc_
#  endif // DOXYGEN_SKIP
#  ifdef __cplusplus
    /** Real number in (0,1).
     * This definition is for the c++ compiler when -O3 is used.  Otherwise,
     * it appears that the C version is linked to.
     * @see MTRand::randDblExc().
     */
    inline double MTRNGrandDblExc() {			/* real number in (0,1) */
        return __my_rand.randDblExc();
    }
#else
    /** Real number in (0,1).
     * @see MTRand::randDblExc().
     */
    double MTRNGrandDblExc();			/* real number in (0,1) */
#endif

#  ifndef DOXYGEN_SKIP
#   define MTRNGrandDblExcV mtrngranddblexcv_
#  endif // DOXYGEN_SKIP
#  ifdef __cplusplus
    /** Real number in (0,n).
     * This definition is for the c++ compiler when -O3 is used.  Otherwise,
     * it appears that the C version is linked to.
     * @see MTRand::randDblExc( const double& n ).
     */
    inline double MTRNGrandDblExcV( const double * n ) {	/* real number in (0,n) */
        return __my_rand.randDblExc(*n);
    }
#else
    /** Real number in (0,n).
     * @see MTRand::randDblExc( const double& n ).
     */
    double MTRNGrandDblExcV( const double * n );	/* real number in (0,n) */
#endif

#  ifndef DOXYGEN_SKIP
#   define MTRNGrandInt mtrngrandint_
#  endif // DOXYGEN_SKIP
#  ifdef __cplusplus
    /** Integer in [0,\f$ 2^{32} - 1 \f$].
     * This definition is for the c++ compiler when -O3 is used.  Otherwise,
     * it appears that the C version is linked to.
     * @see MTRand::randInt().
     */
    inline uint32 MTRNGrandInt() {			/* integer in [0,2^32-1] */
        return __my_rand.randInt();
    }
#else
    /** Integer in [0,\f$ 2^{32} - 1 \f$].
     * @see MTRand::randInt().
     */
    uint32 MTRNGrandInt();			/* integer in [0,2^32-1] */
#endif

#  ifndef DOXYGEN_SKIP
#   define MTRNGrandIntV mtrngrandintv_
#  endif // DOXYGEN_SKIP
#  ifdef __cplusplus
    /** Integer in [0,n] for \f$ n < 2^{32} \f$.
     * This definition is for the c++ compiler when -O3 is used.  Otherwise,
     * it appears that the C version is linked to.
     * @see MTRand::randInt( const uint32& n ).
     */
    inline uint32 MTRNGrandIntV( const uint32 * n ) {	/* integer in [0,n] for n < 2^32 */
        return __my_rand.randInt(*n);
    }
#else
    /** Integer in [0,n] for \f$ n < 2^{32} \f$.
     * @see MTRand::randInt( const uint32& n ).
     */
    uint32 MTRNGrandIntV( const uint32 * n );	/* integer in [0,n] for n < 2^32 */
#endif
	
#  ifndef DOXYGEN_SKIP
#   define MTRNGseedV1 mtrngrandv1_
#  endif // DOXYGEN_SKIP
    /** Seed the generator with a simple uint32_t.
     * @see MTRand::seed( uint32 oneSeed ).
     */
    void   MTRNGseedV1( uint32 * oneSeed );

#  ifndef DOXYGEN_SKIP
#   define MTRNGseedV2 mtrngrandv2_
#  endif // DOXYGEN_SKIP
    /** Seed the generator with an array of 624 uint32's.
     * There are 2^19937-1 possible initial states.  This function allows
     * any one of those to be chosen by providing 19937 bits.  The lower
     * 31 bits of the first element, bigSeed[0], are discarded.  Any bits
     * above the lower 32 in each element are also discarded.  Theoretically,
     * the rest of the array can contain any values except all zeroes.
     *
     * Just call seed() if you want to get array from /dev/urandom.
     * @see MTRand::seed( uint32 *const bigSeed ), MTRand::seed().
     */
    void   MTRNGseedV2( uint32 *const bigSeed );

#  ifndef DOXYGEN_SKIP
#   define MTRNGseed mtrngseed_
#  endif // DOXYGEN_SKIP
    /** Seed the generator with an array from /dev/urandom if available.
     * Otherwise use a hash of time() and clock() values.
     * @see MTRand::seed().
     */
    void   MTRNGseed();
	
    // Saving and loading generator state
#  ifndef DOXYGEN_SKIP
#   define MTRNGsave mtrngsave_
#  endif // DOXYGEN_SKIP
    /** Save generator to array of a size MTRand::SAVE.
     * @param saveArray An array of size MTRand::SAVE to which the generator should be
     *    saved.
     * @see MTRand::save( uint32* saveArray ).
     */
    void   MTRNGsave( uint32* saveArray );  // to array of size MTRand::SAVE

#  ifndef DOXYGEN_SKIP
#   define MTRNGload mtrngload_
#  endif // DOXYGEN_SKIP
    /** Load generator from array of a size MTRand::SAVE.
     * @param loadArray An array of size MTRand::SAVE from which the generator should be
     *    loaded.
     * @see MTRand::save( uint32* saveArray ).
     */
    void   MTRNGload( uint32 *const loadArray );  // from such array
#ifdef __cplusplus
 }
#endif

#endif // MY_RANDOM_H

/** \file MersenneTwister.h
 * Mersenne Twister random number generator -- a C++ class MTRand.
 *
 * Based on code by Makoto Matsumoto, Takuji Nishimura, and Shawn Cokus \n
 * Richard J. Wagner  v0.8  24 March 2002  rjwagner@writeme.com

 * The Mersenne Twister is an algorithm for generating random numbers.  It
 * was designed with consideration of the flaws in various other generators.
 * The period, 2^19937-1, and the order of equidistribution, 623 dimensions,
 * are far greater.  The generator is also fast; it avoids multiplication and
 * division, and it benefits from caches and pipelines.  For more information
 * see the inventors' web page at http://www.math.keio.ac.jp/~matumoto/emt.html

 * \b Reference: \n
 * M. Matsumoto and T. Nishimura, "Mersenne Twister: A 623-Dimensionally
 * Equidistributed Uniform Pseudo-Random Number Generator", ACM Transactions on
 * Modeling and Computer Simulation, Vol. \b 8, No. 1, January 1998, pp 3-30.

 * <b>Copyright (C) 2002  Richard J. Wagner</b>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 * The original code included the following notice:
 *
 *     Copyright (C) 1997, 1999 Makoto Matsumoto and Takuji Nishimura. \n
 *     When you use this, send an email to: matumoto@math.keio.ac.jp
 *     with an appropriate reference to your work.
 *
 * It would be nice to CC: rjwagner@writeme.com and Cokus@math.washington.edu
 * when you write.

 * Not thread safe (unless auto-initialization is avoided and each thread has
 * its own MTRand object).
 */

/** \class MTRand
 * Implementation of a Mersenne Twister random number generator.
 *
 * Access to 32-bit random numbers is provided by member functions:
 * rand(), rand( const double& n ), randExc(), randExc( const double& n ),
 * randDblExc(), randDblExc( const double& n ), randInt(), randInt( const
 * uint32& n ), and operator()().
 *
 * Do NOT use these for CRYPTOGRAPHY without securely hashing several returned
 * values together, otherwise the generator state can be learned after
 * reading 624 consecutive values.
 */

/** \var MTRand::uint32
 * unsigned integer type, at least 32 bits.
 */

/** \var MTRand::N
 * length of state vector.
 */

/** \var MTRand::SAVE
 * length of array for save().
 */
