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
 * Scale a Field in a time dependent manner.
 *
 * Copyright 2004-2008 Spencer Olson
 *
 */



#ifndef olson_tools_ScaleField_h
#define olson_tools_ScaleField_h

#include <olson-tools/Fields.h>
#include <olson-tools/timing/Timing.h>
#include <olson-tools/timing/element/Exponential.h>

#include <limits>

namespace olson_tools {

  /** Apply a timed scaling to a Field. */
  template < typename Field >
  struct ScaleField : virtual BaseField, Field {
    /* TYPEDEFS */
    typedef BaseField super0;
    typedef Field F0;

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
    ScaleField() : super0(), F0(), timing() {
      /* default to having no timing effect. */
      timing.timings.push_back(&DefaultTiming);
      timing.set_time(0.0);
    }

    /** Assignment operator. */
    inline const ScaleField & operator= ( const ScaleField & that ) {
      super0::operator=(that);
      F0::operator=(that);
      timing = that.timing;
      return *this;
    }

    /** Return the timed value of the Scalar field together.
     * This will not instantiate with Vector Fields.
     * @param r
     *     The position to evaluate the field.
     * */
    inline double operator() ( const Vector<double,3> & r ) const {
      return timing.getVal() * F0::operator()(r);
    }

    /** Return the timed value of the Vector field together.
     * This may not instantiate with Scalar Fields.
     * @param r
     *     The position to evaluate the field.
     * */
    template <class T>
    inline void operator() ( T & F, const Vector<double,3> & r ) const {
      F0::operator()(F,r);
      F *= timing.getVal();
    }
  };

  template < typename F >
  timing::element::Exponential ScaleField<F>::DefaultTiming(
    -std::numeric_limits<double>::infinity(), 1.0, 1.0, 1.0
  );

}/* namespace olson_tools */

#endif //olson_tools_ScaleField_h
