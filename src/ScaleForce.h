// -*- c++ -*-
// $Id$
/** \file
 * Scale a Force in a time dependent manner.
 *
 * Copyright 2004-2007 Spencer Olson
 *
 */



#ifndef SCALEFORCE_H
#define SCALEFORCE_H

#include "Forces.h"
#include "Timing.h"

namespace olson_tools {

template <class Force>
class ScaleForce : public virtual BaseForce,
                   public Force {
  public:
    typedef BaseForce      super0;
    typedef Force F;

    static ExpTimingElement DefaultTiming;

    ScaleForce () : super0(), F(), timing() {
        /* default to having no timing effect. */
        timing.timings.push_back(&DefaultTiming);
        timing.current_val = DefaultTiming.getValue(DBL_MAX);
    }

    inline const ScaleForce & operator=(const ScaleForce & that) {
        super0::operator=(that);
        F::operator=(that);
        timing = that.timing;
        return *this;
    }

    inline void accel(      Vector<double,3> & a,
                      const Vector<double,3> & r,
                      const Vector<double,3> & v = V3(0,0,0),
                      const double & t = 0.0,
                      const double & dt = 0.0) const {
        F::accel(a,r,v,t,dt);
        a *= timing.current_val;
    }

    inline double potential(const Vector<double,3> & r,
                            const Vector<double,3> & v = V3(0,0,0),
                            const double & t = 0.0) const {
        return timing.current_val * F::potential(r,v,t);
    }

    template <unsigned int ndim_>
    inline void applyStatisticalForce(Vector<double,ndim_> & particle,
                                      const double & t, const double & dt) const {
        F::applyStatisticalForce( particle, t, timing.current_val * dt );
    }


    Timing timing;
};

template <class F>
ExpTimingElement ScaleForce<F>::DefaultTiming(-DBL_MAX,1.0,1.0,1.0);

}/* namespace olson_tools */

#endif //SCALEFORCE_H
