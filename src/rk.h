#ifdef DUMMY_IFDEF_FOR_FORTRAN
// $Id: rk.h,v 1.1 2005/01/08 04:27:25 olsonse Exp $
/*
 * Copyright 2002-2004 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: rk.h,v $
 * Revision 1.1  2005/01/08 04:27:25  olsonse
 * Initial revision
 *
 * Revision 1.1  2004/04/09 12:29:00  olsonse
 * many smaller improvements.
 *
 *
 */
#endif

/** \file
 * Runge-Kutta routines for generic integration.
 * Copyright 2002-2004 Spencer Olson.
 */

#ifndef RK_H
#define RK_H

#  if LANGUAGE_FORTRAN__ != 1

#    if F77COMP_f95==1
   /* f95 from NAG doesn't put the extra _ at the end of the symbol when _
    * occurs in the un-altered name.
    */
#      define rk_adapt_driver__ rk_adapt_driver_
#    endif

/** Type of function needed for Runge-Kutta.
 * @param p
 *     The Particle that is being moved in Runge-Kutta; this is an array of
 *     doubles representing position and velocity.
 * @param t
 *     Absolute time.
 * @param F
 *     An array of doubles to which this function must write the derivatives
 *     of the respective elements in p.
 * @param args
 *     A pointer to a derivsfunct_t argument that Runge-Kutta doesn't know
 *     about.
 * @see DSMCContainer::setForceRoutine(...)
 */
typedef void (*derivativesFunction)(const double *p, const double * t, double *F, void * args);

#    ifdef __cplusplus
extern "C" {
#    endif

#ifndef DOXYGEN_SKIP
    /* Macro to allow use with fortran code.
     * @see rk_adapt_driver
     */
#    define rk_adapt_driver rk_adapt_driver__
#endif // DOXYGEN_SKIP

    /** Adaptive 5th order runge-kutta.
     * This is using the Cash-Karp parameters for doing 5th order accuracy
     * adaptive Runge-Kutta.  This is a modified implementation of that found
     * in Numerical Recipes.
     * @param p
     *     A pointer to the Particle who's position and velocity will be
     *     integrated.
     * @param n
     *     Number of elements in p.
     * @param t
     *     Absolute time of beginning of integration (unchanged on output).
     * @param dt
     *     The total integration time (unchanged on output).
     * @param dt_step
     *     Starts as the suggested small timestep to use; on exit,
     *     dt_step is the small timestep to use next time rk_adapt_driver is
     *     called.
     * @param getderivs
     *     The function that computes the derivatives of each of the
     *     respective position/velocity parameters; see derivativesFunction.
     * @param fargs
     *     A pointer to optional arguments that will be passed through to the
     *     specified getderivs function.
     * @param errmax 
     *     The maximum error to allow for the adaptive Runge-Kutta routine.
     * @see derivativesFunction, getMachineEPS().
     */
    void rk_adapt_driver(double * p,
			 const int * n,
                         const double * t,
    			 const double * dt,
    			 double * dt_step,
			 const derivativesFunction getderivs,
			 const void * fargs,
			 const double * errmax);

#ifndef DOXYGEN_SKIP
    /* Macro to allow use with fortran code.
     * @see rk4step
     */
#    define rk4step rk4step_
#endif // DOXYGEN_SKIP

    /** Simple 4th order runge-kutta.
     * Sometime I'll get a few more details as to which implementation of
     * 4th order Runge-Kutta this models.
     * @param p
     *     A pointer to the Particle who's position and velocity will be
     *     integrated.
     * @param n
     *     Number of elements in p.
     * @param t
     *     Absolute time of beginning of integration (unchanged on output).
     * @param dt
     *     The integration time (unchanged on output).
     * @param getderivs
     *     The function that computes the derivatives of each of the
     *     respective position/velocity parameters; see derivativesFunction.
     * @param fargs
     *     A pointer to optional arguments that will be passed through to the
     *     specified getderivs function.
     */
    void rk4step        (double * p,
			 const int * n,
                         const double * t,
    			 const double * dt,
			 const derivativesFunction getderivs,
			 const void * fargs);


#ifndef DOXYGEN_SKIP
    /* Macro to allow use with fortran code.
     * @see rk_adapt_driver
     */
#    define getMachineEPS getmachineeps_
#endif // DOXYGEN_SKIP

    /** Measure and record machine precision.
    * The machine precision is stored as M_EPS on the common block:  common/EPS/M_EPS.
    * NOTE:  M_EPS has NOT YET been included into rk_adapt_driver.  It was
    * intendend to be a method for determining how small a time step would be
    * allowed. 
    * getMachineEPS() MUST BE RUN AT LEAST ONCE before rk_adapt_driver is called.
    */
    const double getMachineEPS ();

#    ifdef __cplusplus
}
#    endif


#  endif // end NON-FORTRAN CODE

#endif // RK_H
