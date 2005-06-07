
#ifndef BFIELD_H
#define BFIELD_H

#include <vector>
#include <ostream>
#include <math.h>
#include "physical.h"

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

    template <class T>
    inline T DOTP(const T a[3],const T b[3]) {
        return a[X]*b[X] + a[Y]*b[Y] + a[Z]*b[Z];
    }

    template <class T>
    inline void CROSSP(T result[3], const T a[3], const T b[3]) {
        result[X] = a[Y] * b[Z]  -  a[Z] * b[Y];
        result[Y] = a[Z] * b[X]  -  a[X] * b[Z];
        result[Z] = a[X] * b[Y]  -  a[Y] * b[X];
    }

    template <class T>
    inline T SQR(const T & x) {
        return x*x;
    }

    class  ThinCurrentElement {
      public:
        /** Beginning of current element. */
        double pa[3];
        /** Ending of current element. */
        double pb[3];
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

    /** Container of all arguments needed to compute the magnetic fields and
     * potential and acceleration due to the magnetic field+gravity. */
    class Args {
      public:
        /** Default constructor. */
        inline Args() : delta(10e-6), currents(), rcut(1e-10) {
            bg[X] = 0;
            bg[Y] = 0;
            bg[Z] = 0;
            gravity[X] = 0;
            gravity[Y] = 0;
            gravity[Z] = 0;
            mass = 0; /* whoah! a massless particle! */
        }
        double delta;
        std::vector<ThinCurrentElement> currents;
        double rcut;
        /** x,y,z components of background magnetic field. */
        double bg[3];
        /** x,y,z components of gravity. */
        double gravity[3];
        /** mass of a particle. */
        double mass;
    } __attribute__((packed));

    void derivs(const double p[VZ+1], const double * time, double rkf[VZ+1], void * f);

    /** Calculate the magnetic field for a bunch of thin current elements.
     * @param B
     *     Output variable contains the magnetic field at r.
     * @param r
     *     Location of interest to calculate the field.
     * @param f
     *     BField::Args which describe the thin current elements and some
     *     calculation parameters.
     * @returns
     *     The magnetic field (in B) at r for given thin current elements.
     *
     * @see BField::Args
     */
    void thgetb(double B[4], const double r[3], const Args * f);

    inline void getGradB(double GradB[3], const double r[3], const Args * f ) {
        double B1[4], B2[4];
        double rdelta[3];
        for (int j=X; j <= Z; j++) {
            rdelta[X] = r[X];
            rdelta[Y] = r[Y];
            rdelta[Z] = r[Z];

            rdelta[j] = r[j] - f->delta;
            thgetb(B1, rdelta, f);
            rdelta[j] = r[j] + f->delta;
            thgetb(B2, rdelta, f);

            /* now calculate the field gradient centered at the specified location. */
            GradB[j] = (B2[Z+1] - B1[Z+1]) / f->delta;
        }/* for */
    }

    static const double mu = (-0.5) * (-1) * physical::constant::mu_B;

    inline void accel(double a[3], const double r[3], const Args * f) {
        getGradB(a, r, f);

        /* evaluating rkf(PX) and rkf(PY) are easy */
        a[X] = -mu * a[X]/f->mass + f->gravity[X];
        a[Y] = -mu * a[Y]/f->mass + f->gravity[Y];
        a[Z] = -mu * a[Z]/f->mass + f->gravity[Z];
    }

    /** Calculate the potential of \f$^{87}{\rm Rb}\f$ |F=1,mF=-1>.
     * Note that gF = -1/2
     * and that V = \f$\mu\f$ . B + m(g . r)
     * where \f$\mu\f$  == gF * mF * \f$ mu_{B} \f$
     * and gravitational energy is referenced to (0,0,0).
     *
     * @see potentialNoG(const double r[3], Args * f).
     */
    inline double potential(const double r[3], Args * f) {
        double B[4];

        thgetb(B, r, f);

        return BField::mu*B[3] - f->mass*BField::DOTP(f->gravity,r);
    }

    /** Calculate the potential of \f$^{87}{\rm Rb}\f$ |F=1,mF=-1> ignoring the
     * gravitational component.
     * Note that gF = -1/2
     * and that V = \f$\mu\f$ . B
     * where \f$\mu\f$  == gF * mF * \f$ mu_{B} \f$
     *
     * @see potential(const double r[3], Args * f).
     */
    inline double potentialNoG(const double r[3], Args * f) {
        double B[4];

        thgetb(B, r, f);

        return BField::mu*B[3];
    }


} /* namespace */

#endif // BFIELD_H
