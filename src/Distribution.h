// -*- c++ -*-
// $Id: Distribution.h,v 1.1 2005/01/14 14:09:31 olsonse Exp $
/*
 * Copyright 2004 Spencer Olson
 *
 * $Log: Distribution.h,v $
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
 * Generic distribution interpolator.
 * Copyright 2004 Spencer Olson.
 */


#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H


#include <stdexcept>
#if defined (BUILD_TOOLS)
#  include "random/random.h"
#else
#  include "random.h"
#endif
#include "physical.h"

/** A generic class to invert distributions.
 * The user needs to provide a class or struct that contains a 'distrib'
 * function that will return the value of the (un)normalized distribution at a
 * given independent variable.
 *
 * @see Distribution::Distribution constructor.
 * */
class Distribution {
  public:
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
            throw std::runtime_error("Distribution needs more than one bin.");
        }

        double dx = (max-min) / (L-1.0);

        double * qtmp = new double[L];

        qtmp[0] = distro.distrib(min);
        int i = 1;
        for (double x = min+dx; i < L; x+=dx, i++) {
            qtmp[i] = distro.distrib(x) + qtmp[i-1];
        }

        {   /* normalize the sum. */
            register double qmax = qtmp[L-1];
            for (i = 0; i < L; qtmp[i++] /= qmax);
        }

        q = new double[L];

        for (i = 0; i < L; i++) {
            /* the ith probability: */
            double prob = double(i)/L;

            /* find the probability location above this one. */
            int j = 0;
            while (qtmp[j] < prob) j++;

            /* now we use linear interpolation to determine the input value at
             * this probability.
             */

            q[i] = (min+(j-1)*dx) + ( (min+j*dx) - (min+(j-1)*dx) )
                                    * (prob    - qtmp[j-1])
                                    / (qtmp[j] - qtmp[j-1]);
        }

        /* cleanup */
        delete[] qtmp;
    }

    ~Distribution();

    /** Get a random number from this distribution.
     * This calls lever().
     * @see lever().
     */
    inline double operator() (void) const {
        //return q[(int) rint(MTRNGrand() * L) ];
        return lever();
    }

    /** Get a random number from this distribution. */
    inline double lever() const {
        double ti = MTRNGrand() * (L - 1);
        register int til = int(ti);
        return q[til] + (q[til+1] - q[til]) * (ti - til);
    }

  private:
    int L;
    double * q;
};

/** A 3D thermal distribution for use.
*/
class MaxwellianDistrib3D {
  public:
    /** Contructor for 3D Maxwell distribution.
     * @param b
     *    Set \f$ \beta \f$ for the distribution defined by 
     *    \f$ v^{2} {\rm e}^{-\beta v^{2}} \f$.
     */
    inline MaxwellianDistrib3D(const double & b) : beta(b) {}
    double beta;

    /** Evaluate \f$ v^{2} {\rm e}^{-\beta v^{2}} \f$.
     * @param v
     *     Independent variable.
     */
    inline double distrib (const double & v) const {
        return v*v * exp(-v*v * beta );
    }
};

/** A 2D thermal distribution for use.
*/
class MaxwellianDistrib2D {
  public:
    /** Contructor for 2D Maxwell distribution.
     * @param b
     *    Set \f$ \beta \f$ for the distribution defined by 
     *    \f$ v {\rm e}^{-\beta v^{2}} \f$.
     */
    inline MaxwellianDistrib2D(const double & b) : beta(b) {}
    double beta;

    /** Evaluate \f$ v {\rm e}^{-\beta v^{2}} \f$.
     * @param v
     *     Independent variable.
     */
    inline double distrib (const double & v) const {
        return v * exp(-v*v * beta );
    }
};

/** Gaussian distribution for use.
*/
class GaussianDistrib {
  public:
    /** Contructor for gaussian distribution.
     * @param b
     *    Set \f$ \beta \f$ for the distribution defined by 
     *    \f$ {\rm e}^{-\beta v^{2}} \f$.
     */
    inline GaussianDistrib(const double & b) : beta(b) {}
    double beta;

    /** Evaluate \f$ {\rm e}^{-\beta v^{2}} \f$.
     * @param v
     *     Independent variable.
     */
    inline double distrib (const double & v) const {
        return exp(-v*v * beta);
    }
};

#endif // DISTRIBUTION_H
