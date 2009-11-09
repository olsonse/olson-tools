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
 * Gaussian distribtuion functor implementation.
 *
 * Copyright 2004-2008 Spencer Olson.
 */

#ifndef olson_tools_distribution_Gaussian_h
#define olson_tools_distribution_Gaussian_h

#include <olson-tools/power.h>

#include <cmath>

namespace olson_tools {
  namespace distribution {

    /** Gaussian distribution for use.  */
    struct Gaussian {
      /** Contructor for gaussian distribution.
       * @param b
       *    Set \f$ \beta \f$ for the distribution defined by 
       *    \f$ {\rm e}^{-\beta v^{2}} \f$.
       * @param offs
       *    the center of the Gaussian distribution.
       */
      Gaussian(const double & b, const double & offs = 0.0)
        : beta(b), v0(offs) {}
      double beta; /**< Exponent scaling of the distribution. */
      double v0;   /**< Center of the speed distribution. */

      /** Evaluate \f$ {\rm e}^{-\beta v^{2}} \f$.
       * @param v
       *     Independent variable.
       */
      double operator() (const double & v) const {
        using std::exp;
        return exp(-SQR(v-v0) * beta );
      }
    };

  }/* namespace olson_tools::distribution */
}/* namespace olson_tools */

#endif // olson_tools_distribution_Gaussian_h
