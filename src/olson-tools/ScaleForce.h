// -*- c++ -*-
// $Id$
/** \file
 * Scale a Force in a time dependent manner.
 *
 * Copyright 2004-2007 Spencer Olson
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
