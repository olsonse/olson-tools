// -*- c++ -*-
// $Id$
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2004-2008 Spencer E. Olson
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

/** \file
 * Scale a Force in a time dependent manner.
 *
 * Copyright 2004-2008 Spencer Olson
 *
 */



#ifndef olson_tools_ScaleForce_h
#define olson_tools_ScaleForce_h

#include <olson-tools/Forces.h>
#include <olson-tools/timing/Timing.h>

#include <limits>

namespace olson_tools {

  template < typename Force >
  struct ScaleForce : virtual BaseForce, Force {
    /* TYPEDEFS */
    typedef BaseForce super0;
    typedef Force F;

    /* NON-MEMBER STORAGE */
  private:
    /** Default timing element applys a unity scaling. */
    static timing::element::Exponential DefaultTiming;

    /* MEMBER STORAGE */
  public:
    /** Timing function for this Field scaling. */
    timing::Timing timing;


    /* MEMBER FUNCTIONS */
    /** Constructor adds default timing element to timing. */
    ScaleForce() : super0(), F(), timing() {
      /* default to having no timing effect. */
      timing.timings.push_back(&DefaultTiming);
      timing.set_time(0.0);
    }

    /** Assignment operator. */
    inline const ScaleForce & operator= ( const ScaleForce & that ) {
      super0::operator=(that);
      F::operator=(that);
      timing = that.timing;
      return *this;
    }

    /** Calculate acceleration. */
    inline void accel(       Vector<double,3> & a,
                       const Vector<double,3> & r,
                       const Vector<double,3> & v = V3(0,0,0),
                       const double & t = 0.0,
                       const double & dt = 0.0 ) const {
      F::accel(a,r,v,t,dt);
      a *= timing.getVal();
    }

    inline double potential( const Vector<double,3> & r,
                             const Vector<double,3> & v = V3(0,0,0),
                             const double & t = 0.0 ) const {
      return timing.getVal() * F::potential(r,v,t);
    }

    template <unsigned int ndim_>
    inline void applyStatisticalForce(Vector<double,ndim_> & particle,
                                      const double & t, const double & dt) const {
      F::applyStatisticalForce( particle, t, timing.getVal() * dt );
    }
  };

  template <class F>
  timing::element::Exponential ScaleForce<F>::DefaultTiming(
    -std::numeric_limits<double>::infinity(), 1.0, 1.0, 1.0
  );

}/* namespace olson_tools */

#endif //olson_tools_ScaleForce_h
