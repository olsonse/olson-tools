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

#include <olson-tools/ompexcept.h>

#include <stdexcept>
#include <cstring>

namespace olson_tools {
  namespace distribution {


    inline void Inverter::copyLq(const int & that_L, const double * that_q) {
      if (q) delete[] q;
      L = that_L;
      q = new double[L + 1];
      memcpy (q, that_q, sizeof(double)*(L+1));
    }


    inline Inverter::Inverter(const Inverter & that)
      : L(0), q(NULL) {
      copyLq(that.L, that.q);
    }


    inline Inverter::Inverter(const int & _qLen, const double * _q)
      : L(0), q(NULL) {
      copyLq(_qLen-1, q);
    }


    template < typename DistroFunctor >
    inline Inverter::Inverter( const DistroFunctor & distro,
                               const double & min, const double & max,
                               const int & nbins )
      : L(nbins) {

      if (L <= 1) {
        THROW(std::runtime_error,"Inverter needs more than one bin.");
      }

      double dx = (max-min) / ((double)L);

      double * IntP = new double[L+1];

      /* integrate the distribution */
      IntP[0] = distro(min);
      int i = 1;
      for (double x = min+dx; i <= L; x+=dx, i++) {
        IntP[i] = distro(x) + IntP[i-1];
      }

      { /* normalize the sum. */
        register double qmax = IntP[L];
        for (i = 0; i <= L; IntP[i++] /= qmax);
      }

      /* now we invert the distribution by using the integral. */
      q = new double[L+1];
      q[0] = min;
      q[L] = max;
      
      /* make sure that dx is scaled appropriately for this distribution. 
       * FIXME:  we should probably include the end-points in q[].
       *         need to figure out how to do that without altering the
       *         distribution on the ends.  Man this is a pain to get this
       *         correct.
       */
      double dprob = (IntP[L] - IntP[0]) / ((double)L);

      for ( i = 1; i < L; ++i ) {
        /* the ith probability: */
        double prob = i*dprob + IntP[0];

        /* find the probability location above this one. */
        int j = 1;
        while (j <= L && IntP[j] < prob) j++;

        /* now we use linear interpolation to determine the input value at
         * this probability.
         */

        q[i] = (min+(j-1)*dx)
             + (dx / (IntP[j] - IntP[j-1]))
               * (prob    - IntP[j-1]);
      }

      /* cleanup */
      delete[] IntP;
    }


    inline Inverter::~Inverter() {
      delete[] q;
    }


    inline const Inverter & Inverter::operator=(const Inverter & that) {
      copyLq(that.L, that.q);
      return *this;
    }


  }/* namespace olson_tools::distribution */
}/* namespace olson_tools */
