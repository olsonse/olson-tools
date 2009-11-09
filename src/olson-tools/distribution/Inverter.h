// -*- c++ -*-
// $Id: Distribution.h,v 1.3 2005/05/12 04:27:29 olsonse Exp $
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2004-2008 Spencer Olson
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

/*
 * $Log: Distribution.h,v $
 * Revision 1.3  2005/05/12 04:27:29  olsonse
 * Fixed to for Intel 8.1 compilers.
 * Found (using intel compiler) and fixed an array overflow in BField::potential.
 * Didn't find it earlier because the array is on the stack for the function.
 *
 * Added fmacros.h file to simplify mixing fortran code with others.
 * Added alias function names for Fortran interoperability.
 *
 * Revision 1.2  2005/04/19 17:23:21  olsonse
 * Added new RKIntegrator wrapper class to allow for generic integration
 * templates.
 *
 * Also added trapfe library to help with trapping floating point exceptions.
 *
 * Fixed Distribution inverter finally (hopefull).  It no longer truncates the
 * distribution or reads from bogus memory.
 *
 * Added rk2 integrator (modified Euler) to rk.F.
 *
 * Various other fixes.
 *
 * Revision 1.1  2005/01/14 14:09:31  olsonse
 * Fixed documentation on memory.h, msh.h, options.h.
 * Moved new files Distribution.[hC] listutil.h readData.h from dsmc code.
 *
 * Revision 1.1.1.1  2005/01/08 04:57:25  olsonse
 * Initial CVS import (finally).
 *
 *
 */

/** \file
 * Generic distribution inverter.
 *
 * For some reason, this inverter class took me a long time to debug.  This
 * fully debugged (laugh) version is a result of abuse from various types of
 * distribution functions.
 *
 * Copyright 2004-2008 Spencer Olson.
 */

/** \example invertdistro/1D/testdist.C
 * Example of using the distribution inverter for a one-dimensional
 * distribution. */

/** \example invertdistro/2D/testdist.C
 * Example of using the distribution inverter for a two-dimensional
 * distribution. */

#ifndef olson_tools_distribution_Inverter_h
#define olson_tools_distribution_Inverter_h

#include <olson-tools/random/random.h>

namespace olson_tools {
  namespace distribution {

    /** A generic class to invert arbitrary distributions.
     * The user needs to provide a distribution Functor class that will return
     * the value of the (un)normalized distribution at a given independent
     * variable.  Note (again) the distribution Functor does not need to return
     * a normalized distribution--it will be automatically normalized anyway.  
     *
     * @see Inverter::Inverter constructor.
     * */
    class Inverter {
    private:
      /** memset function. */
      inline void copyLq(const int & that_L, const double * that_q);

    public:

      /** Copy constructor (copying from another Inverter). */
      inline Inverter(const Inverter & that);

      /** Copy onstructor (copying from a specific inverted distribution. */
      inline Inverter(const int & _qLen, const double * _q);

      /** Inverter constructor.
       * This is templated constructor to allow for various types of distribution
       * Functors.  A simple example of such a Functor that might be used to
       * define the distribution is
       * \verbatim
          struct ThermalDistrib {
            double operator() (const double & v) const {
              return v*v * exp(-0.5 * 87.0 * amu * v*v / ( K_B * 500.0 * uK ) );
            }
          };
       * \endverbatim
       *
       * @param distro
       *     Distribution Functor reference.
       *     distro.operator()(x) will be monotonically sampled from min to max
       *     (nbins+1) times.
       * @param min
       *     Minimum of independent variable to include in distribution
       *     inversion.
       * @param max
       *     Maximum of independent variable to include in distribution
       *     inversion.
       * @param nbins
       *     Number of bins to use in distribution inversion [Default  100].
       */
      template < typename DistroFunctor >
      inline Inverter( const DistroFunctor & distro,
                       const double & min, const double & max,
                       const int & nbins = 100 );

      /** Destructor frees memory for q-array. */
      inline ~Inverter();

      /** Get a random number from this distribution.
       * This calls leverarm(double).  This is now essentially the same function
       * as lever().
       * @see lever().
       * @see lever(double).
       */
      inline double operator() (void) const {
        return leverarm(random::MTRNGrand());
      }

      /** Sample the inverted distribution.
       * No bounds checking occurs here.  If you pass in rf>1 or rf<0, then you
       * will be reading from invalid/unallocated memory.  Therefore, be sure to
       * keep rf in range. 
       * @param rf
       *     A fraction in the range [0,1] (inclusive).
       */
      inline double leverarm(const double & rf) const {
        double r = rf * L * 0.99999999999;
        register int ri = int(r);
        return q[ri] + (q[ri+1] - q[ri]) * (r - ri);
      }

      /** Get a random number from this distribution.
       * Calls leverarm(double).
       */
      inline double lever() const {
        return leverarm(random::MTRNGrand());
      }

      /** Copy operator. */
      inline const Inverter & operator=(const Inverter & that);

      /** Returns the number of sampling points used to create the distribution.
       * Note that the length of the inverted distribution array is L+1.
       * */
      inline const int & nBins() const { return L; }

      /** Return a pointer to the invertedDistribution array.
       * @param len
       *     This is an output parameter:  length of the array.
       */
      inline const double * invertedDistribution(int & len) const {
        len = L+1;
        return q;
      }

    private:
      int L;
      double * q; /* length L + 1 */
    };

  }/* namespace olson_tools::distribution */
}/* namespace olson_tools */

#include <olson-tools/distribution/Inverter.cpp>

#endif // olson_tools_distribution_Inverter_h
