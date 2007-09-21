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

#include <sstream>
#include <stdexcept>
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


/** A NOOP class for RK5AdaptiveIntegrator.
 * @see RK5AdaptiveIntegrator.
 */
class NullRKTweak {
  public:
    /** The first rkTweak is called at the beginning of the RK driver. */
    template <unsigned int ndim_>
    inline void rkTweakFirst(Vector<double,ndim_> & x,
                        const double & t,
                        const double & dt_step_current,
                        double & dt_step_next) {}

    /** The first rkTweak is called at the end of the RK driver. */
    template <unsigned int ndim_>
    inline void rkTweakSecond(Vector<double,ndim_> & x,
                        const double & t,
                        const double & dt_step_current,
                        double & dt_step_next) {}
};

/** Integration done by an adaptive Runge-Kutta method yielding 5th order
 * accuracy.
 * TODO:  port the rest of the fortran code and then make derivs a template
 * parameter also.
 * @param RKTweak
 *      A hook to provide the user finer control over the rk integral driver.
 *      This also provides a mechanism for the user to apply a statistical
 *      force that can be separated from the normal forces.
 */
template <class RKTweak = NullRKTweak >
class RK5AdaptiveIntegrator : public RKIntegrator {
    typedef RKIntegrator super;
  public:
    RKTweak rkTweak;

    /** Default constructor sets errmax to 1e-5.
     * @see RKIntegrator.
     */
    inline RK5AdaptiveIntegrator() : RKIntegrator(), errmax(1e-5) {}

    /** Integrate using an adaptive Runge-Kutta which yields 5th order
     * accuracy.
     * This driver computes the integral x(ti+dt) = x(ti) + Int[f, ti, ti+dt].
     * @param x
     *      Dependent variables; input x(ti), output x(ti+dt).
     * @param ti
     *      The starting time in the integral.
     * @param dt
     *      The integration length.
     * @see rk_adapt_driver from rk.h .
     */
    template <unsigned int ndim_>
    inline void integrate(Vector<double,ndim_> & x,
                          const double & ti,
    			  const double & dt,
    			  double & dt_step,
			  const void * derivsArgs) {
#if 0
        int ndim = ndim_;
        rk_adapt_driver (x, &ndim, &ti, &dt, &dt_step, super::derivs, derivsArgs, &errmax);
#endif

        /* ensure that dt and dt_step have the same sign */
        dt_step = copysign(dt_step,dt);

        double  t = ti;
        double tf = ti + dt;

        /* direction of integration. */
        const double dir = copysign(1.0,dt);

        double  x_cal[ndim_], dxdt[ndim_];
        double  dt_step_next = dt_step,
                dt_step_current  = 0.0;

        int     truncated_step = 0;
        const double TINY = 1e-30;

        /* to call the fortran rqks, we need to use an address for ndim */
        int     ndim = ndim_;


        while ( (t*dir) < (tf*dir) ) {
            if ( ((t+dt_step)*dir) > (tf*dir) ) {
                 // If stepsize can overshoot, decrease.
                dt_step = copysign(tf-t,dt);

                truncated_step = 1;
            } else {
                truncated_step = 0;
            }

            dt_step_current = dt_step;
            rkTweak.rkTweakFirst(x, t, (const double&)dt_step_current, dt_step);

            derivs(x.val, &t, &dt_step, dxdt, (void*)derivsArgs);

            for (unsigned int i = 0; i < ndim_; i++) {
                /* Scaling used to monitor accuracy. This
                 * general-purpose choice can be modified if need be. */
                x_cal[i] = fabs(x[i]) + fabs( dt_step*dxdt[i] ) + TINY;
            }

            // time is accumulated in this function
            double told = t;
            rkqs(x.val,&ndim,dxdt,&t,&dt_step,&errmax,x_cal,&dt_step_current, &tf, derivs, derivsArgs);
            // write (*,'(X,F15.8,1X,F15.8,1X,F15.8,1X,F15.8)') x(1:3), dt_step_current

            /* Allow user to provide a call back to adjust the next time step if
             * needed.
             * The user should be WARNED that any increases could cause the next
             * call to rkqs to spend too much time decreasing it again (so as to
             * meet the requested error).
             * This function can also serve as the source of a statistical force.
             *
             * TODO:  If we wanted to apply the 'Strang' type of splitting
             * here, we would do two RKTweak applications of
             * time-step/2.0:  one before the call to derivs at the beginning
             * of the while loop and one right here.
             */
            rkTweak.rkTweakSecond(x, t, (const double&)dt_step_current, dt_step);

            if ( truncated_step == 0 || fabs(dt_step) < fabs(dt_step_current)) {
                /* don't increase the stepsize beyond the previous corrected value
                 * if we've truncated this last step.  This will likely occur after
                 * EVERY set of dt time steps. */
                dt_step_next = dt_step;
            }

            if((t*dir) >= (tf*dir)) {
                // We are now finished, so return
                dt_step = dt_step_next;
                return; // Normal exit.
            }

            if(fabs(t - told) <= fabs(1.5*t*M_EPS)) {
                std::stringstream pos;
                pos << x;
                log_severe(
                    "stepsize underrun (%g) at pos (%s) at t (%g) to tf (%g)",
                    dt_step, pos.str().c_str(), t, tf);
                throw std::runtime_error("stepsize underrun ("+to_string(dt_step)+")");
            }
        }
        log_severe("This code (in rk_driver_adapt) is supposed to be unreachable");
        throw std::runtime_error("unreachable code reached");
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
