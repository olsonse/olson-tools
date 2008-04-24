// -*- c++ -*-
// $Id: Distribution.h,v 1.3 2005/05/12 04:27:29 olsonse Exp $
/*
 * Copyright 2004 Spencer Olson
 *
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
 * Generic distribution inverter.
 *
 * For some reason, this inverter class took me a long time to debug.  This
 * fully debugged (laugh) version is a result of abuse from various types of
 * distribution functions.
 *
 * Copyright 2004 Spencer Olson.
 */

/** \example invertdistro/1D/testdist.C
 * Example of using the distribution inverter for a one-dimensional
 * distribution. */

/** \example invertdistro/2D/testdist.C
 * Example of using the distribution inverter for a two-dimensional
 * distribution. */

#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H


#include <stdexcept>
#include <math.h>
#include "ompexcept.h"
#include "random/random.h"
#include "physical.h"
#include "power.h"

/** A generic class to invert distributions.
 * The user needs to provide a class or struct that contains a 'distrib'
 * function that will return the value of the (un)normalized distribution at a
 * given independent variable.
 *
 * @see Distribution::Distribution constructor.
 * */
class Distribution {
  public:

    /** copy constructor. */
    inline Distribution(const Distribution & that) : L(0), q(NULL) {
        *this = that;
    }

    /** constructor */
    inline Distribution(const int & _qLen, const double * _q) : L(0), q(NULL) {
        struct {
            int L;
            const double * q; /* length L + 1 */
        } D = {_qLen-1, _q};

        *this = (const Distribution &)D;
    }

    /** Distribution constructor.
     * This is templated constructor to allow for various types of classes or
     * structs to provide the distribution function.  A simple and basic
     * example of a struct that might be used to define the distribution is as
     * follows:
     * \verbatim
        struct ThermalDistrib {
            inline double distrib (const double & v) const {
                return v*v * exp(-0.5 * 87.0 * amu * v*v / ( K_B * 500.0 * uK ) );
            }
        };
     * \endverbatim
     *
     * @param distro
     *     Distribution class reference; the distribution class must have a
     *     function of type:  double distrib(const double & x) const.
     *     distro(...) will be monotonically sampled from min to max (nbins+1)
     *     times.
     * @param min
     *     Minimum of independent variable to include in distribution
     *     inversion.
     * @param max
     *     Maximum of independent variable to include in distribution
     *     inversion.
     * @param nbins
     *     Number of bins to use in distribution inversion.
     */
    template <class T>
    inline Distribution( const T & distro,
                         const double & min, const double & max,
                         const int & nbins = 100 ) : L(nbins) {

        if (L <= 1) {
            THROW(std::runtime_error,"Distribution needs more than one bin.");
        }

        double dx = (max-min) / ((double)L);

        double * IntP = new double[L+1];

        /* integrate the distribution */
        IntP[0] = distro.distrib(min);
        int i = 1;
        for (double x = min+dx; i <= L; x+=dx, i++) {
            IntP[i] = distro.distrib(x) + IntP[i-1];
        }

        {   /* normalize the sum. */
            register double qmax = IntP[L];
            for (i = 0; i <= L; IntP[i++] /= qmax);
        }

        /* now we invert the distribution by using the integral. */
        q = new double[L+1];
        q[0] = min;
        q[L] = max;
        
        /* make sure that dx is scaled appropriately for this distribution. 
         * FIXME:  we should probably include the end-points in q[].
         *         need to figure out how to do that without altering the
         *         distribution on the ends.  Man this is a pain to get this
         *         correct.
         */
        double dprob = (IntP[L] - IntP[0]) / ((double)L);

        for (i = 1; i < L; i++) {
            /* the ith probability: */
            double prob = i*dprob + IntP[0];

            /* find the probability location above this one. */
            int j = 1;
            while (j <= L && IntP[j] < prob) j++;

            /* now we use linear interpolation to determine the input value at
             * this probability.
             */

            q[i] = (min+(j-1)*dx)
                 + (dx / (IntP[j] - IntP[j-1]))
                   * (prob    - IntP[j-1]);
        }

        /* cleanup */
        delete[] IntP;
    }

    ~Distribution();

    /** Get a random number from this distribution.
     * This calls leverarm(double).  This is now essentially the same function
     * as lever().
     * @see lever().
     * @see lever(double).
     */
    inline double operator() (void) const {
        return leverarm(MTRNGrand());
    }

    /** Sample the inverted distribution.
     * No bounds checking occurs here.  If you pass in rf>1 or rf<0, then you
     * will be reading from invalid/unallocated memory.  Therefore, be sure to
     * keep rf in range. 
     * @param rf
     *     A fraction in the range [0,1] (inclusive).
     */
    inline double leverarm(const double & rf) const {
        double r = rf * L * 0.99999999999;
        register int ri = int(r);
        return q[ri] + (q[ri+1] - q[ri]) * (r - ri);
    }

    /** Get a random number from this distribution.
     * Calls leverarm(double).
     */
    inline double lever() const {
        return leverarm(MTRNGrand());
    }

    /** Copy operator. */
    inline const Distribution & operator=(const Distribution & that) {
        if (q) delete[] q;

        L = that.L;
        q = new double[L + 1];
        memcpy (q, that.q, sizeof(double)*(L+1));

        return *this;
    }

    /** Returns the number of sampling points used to create the distribution.
     * Note that the length of the inverted distribution array is L+1.
     * */
    inline const int & nBins() const { return L; }

    /** Return a pointer to the invertedDistribution array.
     * @param len
     *     This is an output parameter:  length of the array.
     */
    inline const double * invertedDistribution(int & len) const {
        len = L+1;
        return q;
    }

  private:
    int L;
    double * q; /* length L + 1 */
};

/** A 3D thermal distribution for use.
*/
class MaxwellianDistrib3D {
  public:
    /** Contructor for 3D Maxwell distribution.
     * @param b
     *    Set \f$ \beta \f$ for the distribution defined by 
     *    \f$ v^{2} {\rm e}^{-\beta v^{2}} \f$.
     * @param offs
     *    the center of the distribution.
     */
    inline MaxwellianDistrib3D(const double & b, const double & offs = 0.0) :
        beta(b), v0(offs) {}
    double beta;
    double v0;

    /** Evaluate \f$ v^{2} {\rm e}^{-\beta v^{2}} \f$.
     * @param v
     *     Independent variable.
     */
    inline double distrib (const double & v) const {
        return v*v * exp(-SQR(v-v0) * beta );
    }
};

/** A 2D thermal distribution for use.
 * This distribution is useful for the longitudinal speed distribution of a
 * beam of particles, such as that exiting a small orafice of an oven.  This
 * is also practicable as the perpendicular speed distribution of particles
 * diffusely reflected off a thermal surface.
*/
class MaxwellianDistrib2D {
  public:
    /** Contructor for 2D Maxwell distribution.
     * @param b
     *    Set \f$ \beta \f$ for the distribution defined by 
     *    \f$ v {\rm e}^{-\beta v^{2}} \f$.
     * @param offs
     *    the center of the distribution.
     */
    inline MaxwellianDistrib2D(const double & b, const double & offs = 0.0) :
        beta(b), v0(offs) {}
    double beta;
    double v0;

    /** Evaluate \f$ v {\rm e}^{-\beta v^{2}} \f$.
     * Note that the return value is actually 
     * \f$ abs(v {\rm e}^{-\beta v^{2}}) \f$
     * to more easily allow up and downstream distributions (seperately of
     * course!).
     * @param v
     *     Independent variable.
     */
    inline double distrib (const double & v) const {
        return fabs(v) * exp(-SQR(v-v0) * beta );
    }
};

/** The fraction of average flux of a thermalized group of particles that are
 * moving up-stream.
 * The trivial case for v0 == 0 gives 0.5.
 *
 * @param v0
 *     v0 must be > 0.0; just call the correct function to get the correct
 *     fraction.
 * @param beta
 *     m/(2kT).
 *
 * Note that return value of this function is negative.  Also note that
 * upstream_fraction + downstream_fraction == 1.
 *
 * @see downstream_fraction.
 */
inline double upstream_fraction(const double & v0, const double & beta) {
    double x = v0 * sqrt(beta);
    if (x == 0) return 0.5;
    return 0.5 * ( -exp(-SQR(x))/(sqrt(M_PI) * x) + (1.0 - erf(x)) );
}

/** The fraction of average flux of a thermalized group of particles that are
 * moving down-stream.
 * The trivial case for v0 == 0 gives 0.5.
 *
 * @param v0
 *     v0 must be > 0.0; just call the correct function to get the correct
 *     fraction.
 * @param beta
 *     m/(2kT).
 *
 * Note that return value of this function is negative.  Also note that
 * upstream_fraction + downstream_fraction == 1.
 *
 * @see upstream_fraction.
 */
inline double downstream_fraction(const double & v0, const double & beta) {
    double x = v0 * sqrt(beta);
    if (x == 0) return 0.5;
    return 0.5 * (  exp(-SQR(x))/(sqrt(M_PI) * x) + (1.0 + erf(x)) );
}

/** Gaussian distribution for use.
*/
class GaussianDistrib {
  public:
    /** Contructor for gaussian distribution.
     * @param b
     *    Set \f$ \beta \f$ for the distribution defined by 
     *    \f$ {\rm e}^{-\beta v^{2}} \f$.
     * @param offs
     *    the center of the Gaussian distribution.
     */
    inline GaussianDistrib(const double & b, const double & offs = 0.0) :
        beta(b), v0(offs) {}
    double beta;
    double v0;

    /** Evaluate \f$ {\rm e}^{-\beta v^{2}} \f$.
     * @param v
     *     Independent variable.
     */
    inline double distrib (const double & v) const {
        return exp(-SQR(v-v0) * beta );
    }
};

#endif // DISTRIBUTION_H
