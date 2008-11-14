// -*- c++ -*-
// $Id$
/*
 * Copyright 2004-2005 Spencer Olson
 *
 * $Log$
 *
 */

/** \file
 * Bfield namespace includes classes and function for calculating B-fields and
 * potentials.
 * Copyright 2004-2005 Spencer Olson.
 */

#ifndef BFIELD_H
#define BFIELD_H

#include <vector>
#include <ostream>
#include <math.h>

#include "physical/physical.h"
#include "Vector.h"
#include "power.h"
#include "indices.h"
#include "Fields.h"
#include "Forces.h"

namespace BField {

    class  ThinCurrentElement {
      public:
        /** Beginning of current element. */
        Vector<double,3> pa;
        /** Ending of current element. */
        Vector<double,3> pb;
        /** Sign and magnitude of current flowing from pa to pb. */
        double I;

        const double & operator[](const int & i) const { return pa.val[i]; }
              double & operator[](const int & i)       { return pa.val[i]; }

        /** Constructor to set all elements. */
        inline ThinCurrentElement(
                    const double & pax,
                    const double & pay,
                    const double & paz,
                    const double & pbx,
                    const double & pby,
                    const double & pbz,
                    const double cur
                    ) {
            pa[X] = pax;
            pa[Y] = pay;
            pa[Z] = paz;
            pb[X] = pbx;
            pb[Y] = pby;
            pb[Z] = pbz;

            I = cur;
        }

        /** Constructor to set all elements.
         * @param v
         *     Expected to be a 7 element double array.
         */
        inline ThinCurrentElement(const double * v) {
            pa = V3C(&v[0]);
            pb = V3C(&v[3]);
            I  = v[6];
        }

        /** Constructor to set all elements. */
        inline ThinCurrentElement(const Vector<double,7> & v) {
            pa = V3C(&v.val[0]);
            pb = V3C(&v.val[3]);
            I  = v.val[6];
        }

        /** Copy constructor. */
        inline ThinCurrentElement(const ThinCurrentElement & that) {
            *this = that;
        }

        /** Copy operator. */
        inline const ThinCurrentElement & operator=(const ThinCurrentElement & that) {
            this->pa[X] = that.pa[X];
            this->pa[Y] = that.pa[Y];
            this->pa[Z] = that.pa[Z];

            this->pb[X] = that.pb[X];
            this->pb[Y] = that.pb[Y];
            this->pb[Z] = that.pb[Z];

            this->I = that.I;

            return *this;
        }

        /** Easy way to print out the wire definition. */
        inline std::ostream & print(std::ostream & output) const {
            output << pa[X] << '\t'
                   << pa[Y] << '\t'
                   << pa[Z] << '\t'
                   << pb[X] << '\t'
                   << pb[Y] << '\t'
                   << pb[Z] << '\t'
                   << I;
            return output;
        }
    };

    /** Container of all arguments needed to compute the magnetic fields and
     * potential and acceleration due to the magnetic field. */
    class ThinWireSrc : public virtual BaseField {
      public:
        typedef BaseField super;
        typedef Vector<double,3> base;

        /** Default constructor. */
        inline ThinWireSrc() : super(), currents(), rcut(1e-10) { }

        inline ThinWireSrc(const ThinWireSrc & that) {
            *this = that;
        }

        inline const ThinWireSrc & operator=(const ThinWireSrc & that) {
            super::operator=(that);

            currents = that.currents;
            rcut = that.rcut;
            return *this;
        }

        /** BField of thin wire segments.
        ! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        ! Calculate the B Field for a bunch of thin current elements
        ! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        */
        inline void operator()(Vector<double,3> & B, const Vector<double,3> & r) const {
            /* start with field vector at zero */
            B = 0.0;
        
            /*
             * integrate over the fields produced by the discrete
             * current elements. The implemented formula can be obtained by
             * analytically integrating the field produced by a linear 
             * current using Biot-Savart's law. 
             *
             * Some definitions:
             *  B(r) == returned bfield (where r is the passed in vector).
             *  rl   == length of thin wire segment (m).
             *  rn   == unit vector from point a to point b of thin wire segment.
            */
          
            for (int k = currents.size()-1; k >= 0; k--) {
                const ThinCurrentElement & cur = currents[k];

                Vector<double,3> rn = cur.pb; rn -= cur.pa;
         
                double rl = rn.abs();
                rn /= rl;

                Vector<double,3> y = r - cur.pa;

                double x1 = y * rn;

                /* x2 = (y * rn) - rl */
                double x2 = x1 - rl;

                /* rho = sqrt ( (y * y) - (y * rn)**2 ) */
                double rho = sqrt( y*y - x1*x1 );

                /* This IF avoids accidental zeros in denominators. Outside
                 * the coil packs, there is no effect.            
                 */
                if (rho < rcut) rho = rcut;
 
                double dB = 1e-7 * cur.I *(   x1/sqrt( SQR(x1) + SQR(rho) )
                                            - x2/sqrt( SQR(x2) + SQR(rho) )
                                          ) / rho;
        
                /* z = rn X y */
                Vector<double,3> dBxyz;
                cross(dBxyz, rn, y);
 
                double h1= dBxyz.abs();
 
                /* Increment the field vector B(3) by the field produced
                 * by the presently considered current element.         
                 */
                if (h1 > 0.0) {
                    dBxyz *= dB / h1;
                    B += dBxyz;
                }

            }/* for */
        }

        std::vector<ThinCurrentElement> currents;
        double rcut;
    };



    /* ************* FORCES *********** */

    /** Container for several calculations to be performed using a B-field
     * source.  Doing this all in a template fashion allows for function
     * inheritance and interface specification with compile-time optimizations
     * enabled (no v-table for virtual functions).
     * A user can use this class as an implementation for gradient, accel,
     * potential.  For example, the accel function (or similar)
     * is required for Bderivs::derivs.
     *
     * The user can certainly also implement smarter replacements for these
     * functions for some cases (using a lookup-table for example).
     *
     * @param BSrc
     *     Magnetic field; must be a derivation of the BaseField class.
     *
     * @see Derivs::derivs.
     */
    template <class BSrc>
    class BCalcs : public virtual BaseForce, public BSrc {
      public:
        typedef BaseForce super0;
        typedef BSrc      super1;

        /** Default constructor of BCalcs. */
        inline BCalcs() : super0(), super1() {
            mu = (-0.5) * (-1) * physical::constant::mu_B;
        }

        inline const BCalcs & operator=(const BCalcs & that) {
            mu = that.mu;
            return *this;
        }
    
        inline void accel(      Vector<double,3> & a,
                          const Vector<double,3> & r,
                          const Vector<double,3> & v = V3(0,0,0),
                          const double & t = 0.0,
                          const double & dt = 0.0 ) const {
            //gradient(a, magnitude_of<super1>((const super1&)*this), r);
            gradient_of_magnitude(a, (super1&)*this, r);
            a *= - mu / super0::mass;
        }
    
        /** Calculate the magnetic potential of \f$^{87}{\rm Rb}\f$ |F=1,mF=-1>.
         * Note that gF = -1/2
         * and that V = \f$\mu\f$ . B
         * where \f$\mu\f$  == gF * mF * \f$ mu_{B} \f$
         *
         */
        inline double potential(const Vector<double,3> & r,
                                const Vector<double,3> & v = V3(0,0,0),
                                const double & t = 0.0) const {
            Vector<double,3> B;
            super1::operator()(B, r);
            return mu * B.abs();
        }

        /**  \f$\mu\f$  == gF * mF * \f$ mu_{B} \f$.
         * Note that V = \f$\mu\f$ . B
         * and for dark trapped state (\f$ \left|F=1,m_{F}=-1\right> \f$)
         *  of \f$^{87}{\rm Rb} \f$,  gF = -1/2.
         * This defaults to \f$F=1\f$ and \f$m_{F}=-1\f$
         */
        double mu;
    };

} /* namespace */

#endif // BFIELD_H
