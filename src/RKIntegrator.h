// -*- c++ -*-
// $Id: RKIntegrator.h,v 1.1 2005/04/19 17:23:21 olsonse Exp $
/*
 * Copyright 2005 Spencer Olson
 *
 * $Log: RKIntegrator.h,v $
 * Revision 1.1  2005/04/19 17:23:21  olsonse
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
 *
 */

/** \file
 * Some c++ abstracting for integrating functions, starting of course with
 * Runge-Kutta.
 *
 * Copyright 2005 Spencer Olson
 */

#ifndef RKINTEGRATOR_H
#define RKINTEGRATOR_H

#include "rk.h"

class RKIntegrator {
  public:
    /** Default constructor doesn't do anything exciting except init derivs to
     * NULL. */
    inline RKIntegrator() : derivs(0x0) {}

    /** The function intended to be used by the Runge-Kutta routines to
     * calculate the phase-space derivatives.
     * @see rk.h and rk.F for more details on the adaptive Runge-Kutta method.
     */
    derivativesFunction derivs;
};


/** Integration done by an adaptive Runge-Kutta method yielding 5th order
 * accuracy.
 * @see rk_adapt_driver in rk.h.
 */
class RK5AdaptiveIntegrator : public RKIntegrator {
    typedef RKIntegrator super;
  public:
    /** Default constructor sets errmax to 1e-5.
     * @see RKIntegrator.
     */
    inline RK5AdaptiveIntegrator() : RKIntegrator(), errmax(1e-5) {}

    /** Integrate using an adaptive Runge-Kutta which yields 5th order
     * accuracy.
     * @see rk_adapt_driver from rk.h .
     */
    template <unsigned int ndim_>
    inline void integrate(Vector<double,ndim_> & p,
                          const double & t,
    			  const double & dt,
    			  double & ddt,
			  const void * fargs) {
        int ndim = ndim_;
        rk_adapt_driver (p.val, &ndim, &t, &dt, &ddt, super::derivs, fargs, &errmax);
    }

    /** Error tolerance used for adaptive Runge-Kutta.
     * @see rk_adapt_driver, rk.h, and rk.F for more details on the adaptive
     *     Runge-Kutta method.
     */
    double errmax;
};

class RK4Integrator : public RKIntegrator {
    typedef RKIntegrator super;
  public:
    inline RK4Integrator() : RKIntegrator() {}

    template <unsigned int ndim_>
    inline void integrate(Vector<double,ndim_> &p,
                          const double & t,
    			  const double & dt,
    			  const double & dt_step,
			  const void * fargs) {

        int ndim = ndim_;
        rk4step (p.val, &ndim, &t, &dt, super::derivs, fargs);
    }
};

class RK2Integrator : public RKIntegrator {
    typedef RKIntegrator super;
  public:
    inline RK2Integrator() : RKIntegrator() {}

    template <unsigned int ndim_>
    inline void integrate(Vector<double,ndim_> & p,
                          const double & t,
    			  const double & dt,
    			  const double & dt_step,
			  const void * fargs) {

        int ndim = ndim_;
        rk2step (p.val, &ndim, &t, &dt, super::derivs, fargs);
    }
};

#endif // RKINTEGRATOR_H
