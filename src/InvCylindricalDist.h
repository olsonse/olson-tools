// -*- c++ -*-
// $Id$
/*
 * Copyright 2004 Spencer Olson
 *
 * $Log$
 *
 */

/** \file
 * Generic cylindrical distribution inverter.  This class uses the Generic
 * Distribution inverter to aid in inverting cylindrically symmetric
 * distributions.
 *
 * Copyright 2004 Spencer Olson.
 *
 */

#ifndef INVCYLINDRICALDIST_H
#define INVCYLINDRICALDIST_H

#include "Distribution.h"
#include "physical.h"
#include "logger.h"
#include "Vector.h"
#include "power.h"

#define RHO     X
#define PHI     Y

/** Simple class to keep range information. */
class range_t {
  public:
    double min, max;
    double length() const { return max - min; }
    const range_t & operator=(const range_t & that) {
        this->min = that.min;
        this->max = that.max;
        return *this;
    }
};

/** Decouple the 2D->1D mapping to j and k cartesian components.
 */
inline void decoupleRhoPhi(const double & nphi_i,
                           const range_t & r_range,
                           const range_t & phi_range,
                           const double & dr,
                           const double & ds,
                           const int & j,
                           const int & k,
                           double pos[3]
                          ) {
    /* matlab code for what I want to do:
    ri = (dr=.1):dr:10; ds = 0.1;
        % nphi(r_{i}) = pi r_{i}^2 / (ds dr)
        %       nphi is the integral number of phi values up to some phi point
        %       in the r_{i} shell.
    nphi_i = 0:.5:floor(pi*10^2/(ds*dr));
    rindx = floor(sqrt( nphi_i * ds * dr / pi ) / dr);
    rj = dr*(rindx + 0.5);
    rjm1 = dr*(rindx); % why??? 
    nphi_rjm1 = (pi*rj.^2 / (ds * dr));
    phi = (nphi_i - nphi_rjm1(1:length(nphi_i)) ) ./ (rj / ds) + pi;
    polar(phi,ri);

    and for the sake of supporting angle and radius ranges (for some really odd reason):
    amin = pi/4; amax = pi/2;
    rmin = 5;
    nvirt = ((amax-amin)/2)*rmin^2/(ds*deltar);
    rindx = floor(sqrt( (nphi_i+nvirt) * ds * dr / ((amax-amin)/2) ) / dr);
    rj = dr*(rindx + 0.5);
    rjm1 = dr*(rindx); % why ???
    nphi_rjm1 = (((amax-amin)/2)*rjm1.^2 / (ds * dr));
    phi = (nphi_i+nvirt - nphi_rjm1(1:length(nphi_i)) ) ./ (rj / ds) + amin;

    note that range_rho.max is taken care of by the max(nphi) set in the
    calling routine. 

    note that for phenomenelogical reasons, rmin>=.5 deltar.  Maybe I'll
    understand why some day.
    */

    register double nphi_virt = (0.5*phi_range.length()) * SQR(r_range.min) / (ds*dr);
    register double rindx = floor( sqrt((nphi_i+nphi_virt)* ds * dr / (0.5*phi_range.length()))/dr);
    register double rj = dr*(rindx + 0.5);
    register double rjm1 = dr*rindx; /* why ??? */
    register double nphi_rjm1 = ((0.5*phi_range.length()) * SQR(rjm1) / (ds * dr));
    register double phi = (nphi_i+nphi_virt - nphi_rjm1)*(ds / rj) + phi_range.min;


    register double cosphi;
    register double sinphi;
#if defined(__x86_64__) || defined(__amd64__) || defined(__i386__)
    asm ("fsincos" : "=t" (cosphi), "=u" (sinphi) : "0" (phi));
#else
    cosphi = cos(phi);
    sinphi = sin(phi);
#endif

    pos[j] = rj*cosphi;
    pos[k] = rj*sinphi;
}


/** just what is says;  nothing. */
typedef struct {} nothing;

/** A flat distribution for use.
*/
class FlatDistribution {
  public:
    /** A constructor to satisfy tempalte requirements further down. */
    template <class T>
    FlatDistribution(const T & dontcare) {}
    /** Return 0.5.
     */
    inline double distrib (const double & x) const {
        return 0.5;
    }
};

/** A generic class for inverting cylindrical type distributions such that
 * pixelation that occurs from 2D->1D mapping is in the radial direction.
 * @param Distrib2D
 *     The type of distribution function to use tor the transverse
 *     coordinates.  Note that this function should make use of the
 *     decoupleRhoPhi function to obtain pos[X] and pos[Y] where X and Y are
 *     the transverse coordinates for example.
 *     The constructor of this class must have the following prototype:
 \verbatim
       Distrib2D(init, ith, iVal, range[RHO], range[PHI], dr, ds, origin).
\endverbatim
 *     where:
 *     - init is the InitClass (below)
 *     - ith is the index of the axial coordinate
 *     - iVal is the value of the axial coordinate to use (will be midpoint)
 *     - range[RHO] and range[PHI] are the RHO and PHI 'range_t'es respectively
 *     - dr radial increment size to use for decoupleRhoPhi
 *     - ds curcumferencial increment size to use for decoupleRhoPhi
 *     - origin is a Vector<double,3> that should be used as an offset
 *       position for all calculations.  In other words, pos[j] and pos[k]
 *       returned from decoupleRhoPhi should be added to origin before
 *       calculating any position dependent functions.
 *     .
 * @param Distrib1D
 *     The type of distribution function to use for the axial coordinate
 *     [Default = FlatDistribution].
 * @param InitClass
 *     A user specified class that may be used to help initialize the
 *     distribution functions [Default = nothing].
 */
template <class Distrib2D,
          class Distrib1D = FlatDistribution,
          class InitClass = nothing>
class InvCylindricalDist {
  public:

    /** Default constructor sets default values.
     * What a novel idea.
     */
    inline InvCylindricalDist() {
        distrib_perp = distrib_long = NULL;

        nbins[RHO] = 1000;
        nbins[PHI] = -1;
        nbins[Z] = -1;

        range[RHO].min = 0;
        range[RHO].max = physical::units::cm;
        range[PHI].min = 0;
        range[PHI].max = 2.0*physical::constant::pi;
        range[Z].min = -physical::units::cm;
        range[Z].max = physical::units::cm;

        indx_long = Z;
        ith = jth = kth = 0;
        dr = 0;
        ds = 0;
    }

    /** Clean things up. */
    inline ~InvCylindricalDist(){
        cleanup();
    }

    /** Initialize the inverted distribution random number generator.
     * @param origin
     *     Vector<double,3> to indicate the offset origin to use.
     */
    inline void initialize(const Vector<double,3> & origin) {
        if (nbins[PHI] < 0) nbins[PHI] = nbins[RHO];
        if (nbins[Z] < 0) nbins[Z] = nbins[RHO];

        cleanup();

        ith = indx_long;
        jth = (ith+1)%3,
        kth = (ith+2)%3;

        dr = range[RHO].length() / (nbins[RHO]+1);

        /* ensure that the center starts >= 0.5*r */
        if (range[RHO].min < (0.5*dr)) range[RHO].min = 0.5*dr;

        /* get ds from the maximal radius assuming phi ranges from 0:2*pi. */
        ds = 2.0*physical::constant::pi * range[RHO].max / (nbins[PHI]+1);

        /* and now get the maximum total number of circumferencial bins. */
        double nphi_R = 0.5*range[PHI].length()
                      * ( SQR(range[RHO].max) - SQR(range[RHO].min) )
                      / (ds * dr);

        /* we'll have iVal be the midpoint int the Z range. */
        double iVal = range[Z].min + 0.5*range[Z].length();

        distrib_perp =
            new Distribution(
                    Distrib2D(init, ith, iVal, range[RHO], range[PHI], dr, ds, origin),
                    0, nphi_R, nbins[RHO] * nbins[PHI]
                );

        distrib_long = new Distribution(Distrib1D(init), range[Z].min, range[Z].max, nbins[Z]);
    }/* initialize */

    /** Return the next X coordinate from the random distribution.
     * WARNING:  These functions assume that x() is called before successive
     * calls to y() and z().
     */
    inline double x() { get_next_vals(); return next_vals[X]; }

    /** Return the next Y coordinate from the random distribution.
     * WARNING:  These functions assume that x() is called before successive
     * calls to y() and z().
     */
    inline double y() const { return next_vals[Y]; }

    /** Return the next Z coordinate from the random distribution.
     * WARNING:  These functions assume that x() is called before successive
     * calls to y() and z().
     */
    inline double z() const { return next_vals[Z]; }

    /** Ranges for RHO, PHI, and Z cylindrical coordinates.
     * Note:  the user should use the RHO, PHI, Z macros to access these.
     */
    range_t range[3];

    /** The number of bins for RHO, PHI, and Z dimensions.
     * nbins[PHI] is treated as the number of circumferencial bins to use AT
     * the maximum radius.
     */
    int nbins[3];

    /* the user must set this to the index of the direction that is to be used
     * for the flat longitudinal distribution.
     */
    int indx_long;

    /** The user specified class that may be used to initialize the
     * perpendicular and longitudinal distribution functions.
     */
    InitClass init;

  private:
    int ith, jth, kth;
    double ds, dr;

    inline void cleanup() {
        if (distrib_perp) delete distrib_perp;
        if (distrib_long) delete distrib_long;
    }

    inline void get_next_vals() {
        next_vals[ith] =  distrib_long->lever();
        double nphi_i = distrib_perp->lever();
        decoupleRhoPhi(nphi_i, range[RHO], range[PHI], dr, ds, jth, kth, next_vals);
    }

    double next_vals[3];

    Distribution * distrib_perp, * distrib_long;
};

#endif // INVCYLINDRICALDIST_H
