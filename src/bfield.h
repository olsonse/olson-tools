// -*- c++ -*-
// $Id$
/*
 * Copyright 2004 Spencer Olson
 *
 * $Log$
 *
 */

/** \file
 * Bfield namespace includes classes and function for calculating B-fields and
 * potentials.
 * Copyright 2004 Spencer Olson.
 */

#ifndef BFIELD_H
#define BFIELD_H

#include <vector>
#include <ostream>
#include <math.h>

#include "physical.h"
#include "Vector.h"
#include "power.h"

#ifndef X
#  define X 0
#elif defined(X) && (X != 0)
#  error X already defined and it is not == 0!
#endif

#ifndef Y
#  define Y 1
#elif defined(Y) && (Y != 1)
#  error Y already defined and it is not == 1!
#endif

#ifndef Z
#  define Z 2
#elif defined(Z) && (Z != 2)
#  error Z already defined and it is not == 2!
#endif

#ifndef VX
#  define VX 3
#elif defined(VX) && (VX != 3)
#  error VX already defined and it is not == 3!
#endif

#ifndef VY
#  define VY 4
#elif defined(VY) && (VY != 4)
#  error VY already defined and it is not == 4!
#endif

#ifndef VZ
#  define VZ 5
#elif defined(VZ) && (VZ != 5)
#  error VZ already defined and it is not == 5!
#endif

namespace BField {

    class  ThinCurrentElement {
      public:
        /** Beginning of current element. */
        Vector<double,3> pa;
        /** Ending of current element. */
        Vector<double,3> pb;
        /** Sign and magnitude of current flowing from pa to pb. */
        double I;

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

    class BaseArgs {
      public:
        /** Default constructor. */
        inline BaseArgs() : delta(10e-6) {
            gravity = 0.0;
            mass = 0; /* whoah! a massless particle! */
            mu = (-0.5) * (-1) * physical::constant::mu_B;
        }

        inline BaseArgs(const BaseArgs & that) {
            *this = that;
        }

        inline const BaseArgs & operator=(const BaseArgs & that) {
            delta = that.delta;
            gravity = that.gravity;
            mass = that.mass;
            mu = that.mu;
            return *this;
        }

        /* dx == dy == dz to use for computing gradient. */
        double delta;

        /** x,y,z components of gravity. */
        Vector<double,3> gravity;

        /** mass of a particle. */
        double mass;

        /**  \f$\mu\f$  == gF * mF * \f$ mu_{B} \f$.
         * Note that V = \f$\mu\f$ . B
         * and for dark trapped state (\f$ \left|F=1,m_{F}=-1\right> \f$)
         *  of \f$^{87}{\rm Rb} \f$,  gF = -1/2.
         * This defaults to \f$F=1\f$ and \f$m_{F}=-1\f$
         */
        double mu;

    };

    /* Add a static background field to a BField Src. */
    template <class BSrc>
    class AddBgSrc : public virtual BaseArgs, public BSrc {
      public:
        typedef BaseArgs super0;
        typedef BSrc     super1;

        AddBgSrc() : super0(), super1() {
            bg = 0.0;
        }

        inline const AddBgSrc & operator=(const AddBgSrc & that) {
            super0::operator=(that);
            super1::operator=(that);
            bg = that.bg;
            return *this;
        }

        /** Obtains BField from BSrc and then adds a static background field. */
        inline void getb(Vector<double,3> & B, const Vector<double,3> & r) const {
            super1::getb(B,r);
            B += bg;
        }

        Vector<double,3> bg;
    };

    /* Adds BFields from two different sources. */
    template <class BSrc0, class BSrc1>
    class AddSrc : public virtual BaseArgs, public BSrc0, public BSrc1 {
      public:
        typedef BaseArgs super0;
        typedef BSrc0    super1;
        typedef BSrc1    super2;

        AddSrc() : super0(), super1(), super2() {}

        inline const AddSrc & operator=(const AddSrc & that) {
            super0::operator=(that);
            super1::operator=(that);
            super2::operator=(that);
            return *this;
        }

        /** Obtains BField from BSrc and then adds a static background field. */
        inline void getb(Vector<double,3> & B, const Vector<double,3> & r) const {
            super1::getb(B,r);
            Vector<double,3> B2;
            super2::getb(B2,r);
            B += B2;
        }
    };

    /* Add a static background field to a BField Src. */
    template <class Functor>
    class BFunctor : public virtual BaseArgs, public Functor {
      public:
        typedef BaseArgs super0;
        typedef Functor  super1;

        BFunctor() : super0(), super1() {}

        inline const BFunctor & operator=(const BFunctor & that) {
            super0::operator=(that);
            super1::operator=(that);
            return *this;
        }

        /* the functor must implement the getb method. */
    };

    /** Container of all arguments needed to compute the magnetic fields and
     * potential and acceleration due to the magnetic field+gravity. */
    class ThinWireSrc : public virtual BaseArgs {
      public:
        typedef BaseArgs super;
        /** Default constructor. */
        inline ThinWireSrc() : currents(), rcut(1e-10) { }

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
        inline void getb(Vector<double,3> & B, const Vector<double,3> & r) const {
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

    template <class BSrc>
    class Bderivs {
      public:

        /** Compute phase-space derivatives for integrating methods (e.g.
         * runge-kutta.
         * Because this is a template, you will have to explicity instantiate this
         * function to get a pointer to pass into rk or the like.
         * Also, this function will have to be correctly cast to the rk type
         * (where srcP is a void *).
         */
        static void derivs(const double p[VZ+1], const double * time, double rkf[VZ+1], BSrc * srcP) {
            rkf[X]  = p[VX];
            rkf[Y]  = p[VY];
            rkf[Z]  = p[VZ];
            accel(V3C(rkf+VX), V3C(p), *srcP);
        }
    };

    template <class BSrc>
    inline void getGradB(Vector<double,3> & GradB, const Vector<double,3> & r, const BSrc & src ) {
        Vector<double,3> B1, B2, dr;
        for (int j=X; j <= Z; j++) {
            dr = r;

            dr[j] = r[j] - src.delta;
            src.getb(B1, dr);

            dr[j] = r[j] + src.delta;
            src.getb(B2, dr);

            /* now calculate the field gradient centered at the specified location. */
            GradB[j] = (B2.abs() - B1.abs()) / src.delta;
        }/* for */
    }

    template <class BSrc>
    inline void accel(Vector<double,3> & a, const Vector<double,3> & r, const BSrc & src) {
        getGradB(a, r, src);

        a *= -src.mu / src.mass;
        a +=  src.gravity;
    }

    /** Calculate the potential of \f$^{87}{\rm Rb}\f$ |F=1,mF=-1>.
     * Note that gF = -1/2
     * and that V = \f$\mu\f$ . B + m(g . r)
     * where \f$\mu\f$  == gF * mF * \f$ mu_{B} \f$
     * and gravitational energy is referenced to (0,0,0).
     *
     * @see potentialNoG(const Vector<double,3> & r, Bsrc & src).
     */
    template <class BSrc>
    inline double potential(const Vector<double,3> & r, const BSrc & src) {
        Vector<double,3> B;

        src.getb(B, r);

        return src.mu*B.abs() - src.mass * (src.gravity * r);
    }

    /** Calculate the potential of \f$^{87}{\rm Rb}\f$ |F=1,mF=-1> ignoring the
     * gravitational component.
     * Note that gF = -1/2
     * and that V = \f$\mu\f$ . B
     * where \f$\mu\f$  == gF * mF * \f$ mu_{B} \f$
     *
     * @see potential(const Vector<double,3> & r, Bsrc & src).
     */
    template <class BSrc>
    inline double potentialNoG(const Vector<double,3> & r, const BSrc & src) {
        Vector<double,3> B;
        src.getb(B, r);
        return src.mu*B.abs();
    }


} /* namespace */

#endif // BFIELD_H
