#ifdef DUMMY_IFDEF_FOR_FORTRAN
// $Id: rk.h,v 1.3 2005/05/12 04:27:35 olsonse Exp $
/*
 * Copyright 2002-2004 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: rk.h,v $
 * Revision 1.3  2005/05/12 04:27:35  olsonse
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
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
 *
 * Revision 1.1  2004/04/09 12:29:00  olsonse
 * many smaller improvements.
 *
 *
 */
#endif

/** \file
 * Runge-Kutta routines for generic integration.
 *
 * Copyright 2002-2004 Spencer Olson.
 *
 */

/** \example rk/testrk.cpp
 * Example use of Runge-Kutta routines.
 */

#ifndef RK_H
#define RK_H

#  if LANGUAGE_FORTRAN__ != 1


#ifdef __cplusplus
namespace olson_tools {
#endif 



/** Type of function needed for Runge-Kutta.
 * @param p
 *     The Particle that is being moved in Runge-Kutta; this is an array of
 *     doubles representing position and velocity.
 * @param t
 *     Absolute time.
 * @param dt
 *     Time-step used in the integration.  Note that this is not the smaller
 *     timestep used to calculate intermediate steps in the Runge-Kutta
 *     methods, but rather the total timestep.
 * @param F
 *     An array of doubles to which this function must write the derivatives
 *     of the respective elements in p.
 * @param args
 *     A pointer to an auxilliary argument passed into one of the Runge-Kutta
 *     methods needed for computing F.  
 * @see DSMCContainer::setForceRoutine(...)
 */
typedef void (*derivativesFunction)(const double *p, const double * t, const double * dt, double *F, void * args);

#    ifdef __cplusplus
extern "C" {
#    endif

#ifndef DOXYGEN_SKIP
    /* Macro to allow use with fortran code.
     */
#  define rk_adapt_driver rk_adapt_driver_
#endif // DOXYGEN_SKIP

    /** Adaptive 5th order runge-kutta.
     * This is using the Cash-Karp parameters for doing 5th order accuracy
     * adaptive Runge-Kutta.  
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
     */
#  define rk4step rk4step_
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
     */
#  define rk2step rk2step_
#endif // DOXYGEN_SKIP

    /** Simple 2nd order runge-kutta.
     * An improved Euler routine.
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
    void rk2step        (double * p,
			 const int * n,
                         const double * t,
    			 const double * dt,
			 const derivativesFunction getderivs,
			 const void * fargs);


#ifndef DOXYGEN_SKIP
    /* Macro to allow use with fortran code.
     * @see rk_adapt_driver
     */
#  define getMachineEPS getmachineeps_
#endif // DOXYGEN_SKIP

    /** Measure and record machine precision.
    * The machine precision is stored as M_EPS on the common block:  common/EPS/M_EPS.
    * NOTE:  M_EPS has NOT YET been included into rk_adapt_driver.  It was
    * intendend to be a method for determining how small a time step would be
    * allowed. 
    * getMachineEPS() MUST BE RUN AT LEAST ONCE before rk_adapt_driver is called.
    */
    double getMachineEPS ();


#ifndef DOXYGEN_SKIP
    /* Macro to allow use with fortran code.
     * @see rk_adapt_driver
     */
#  define rkqs rkqs_
#endif // DOXYGEN_SKIP
    void rkqs(double *x,int *n,double *dxdt,double *t,double *dt_step,const double *ERRMAX,double *x_cal, double *dt_step_did, double *tf, derivativesFunction derivs, const void * fargs);

#    ifdef __cplusplus
}

}/* namespace olson_tools */
#    endif


#  endif // end NON-FORTRAN CODE

#endif // RK_H
