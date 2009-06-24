// -*- c++ -*-
// $Id$
/** \file
 * Scale a Field in a time dependent manner.
 *
 * Copyright 2004-2007 Spencer Olson
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
