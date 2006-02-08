// -*- c++ -*-
// $Id$
/*
 * Copyright 2005 Spencer Olson
 *
 * $Log$
 *
 */

/** \file
 * Bfield namespace includes classes and function for calculating B-fields and
 * potentials.
 *
 * BField::BTrapDistribution2D is a cylindrical Distribution to be inverted
 * with InvCylindricalDist.  This Distribution is suitable for a wire-guide
 * type of trap.
 *
 * Copyright 2005 Spencer Olson.
 */

#ifndef BTRAP_DISTRO_H
#define BTRAP_DISTRO_H

#include "physical.h"
#include "bfield.h"
#include "logger.h"
#include "Vector.h"
#include "InvCylindricalDist.h"

namespace BField {

    template <class BSrc>
    class BTrap2DInit {
      public:
        BTrap2DInit () {
            T_perp = (150.0*physical::units::K);     /* default to 1.5cm */
            bsrc = NULL;
        }

        double T_perp;
        BSrc * bsrc;
    };


    /** A thermal distribution in a 2D trap.
    */
    template <class BSrc>
    class BTrapDistribution2D {
      public:
        /** Contructor for thermal distribution in a 2D trap.
         * @param init
         *     The BTrapInit class to use to initialize this distribution
         *     function.
         * @param i
         *     The dimension that is held constant throughout this distribution.
         * @param iVal_
         *     The constant value used for the ith dimension.
         * @param r_range_
         *     The range of the radial dimension.
         * @param phi_range_
         *     The range of the angular dimension.
         * @param dr_
         *     Radial increment (m) in sampling function.
         * @param ds_
         *     Circumferencial increment (m) in sampling function.
         * @param origin_
         *     Position Vector<double,3> that should be added before calculating
         *     the b-field.
         */
        template <class Binit>
        inline BTrapDistribution2D(const Binit & init,
                                 int i, double iVal_,
                                 const range_t & r_range_,
                                 const range_t & phi_range_,
                                 const double & dr_,
                                 const double & ds_,
                                 const Vector<double,3> & origin_ ) :
            bsrc(*init.bsrc),
            beta(1.0 / (physical::constants::K_B * init.T_perp * physical::units::K)),
            const_indx(i),
            iVal(iVal_),
            dr(dr_), ds(ds_),
            origin(origin_) {

            if (i < 0 || i > 2) throw std::runtime_error(FILE_LINE "invalid index");

            j = (i+1)%3;
            k = (i+2)%3;

            r_range = r_range_;
            phi_range = phi_range_;
        }

        BSrc & bsrc;
      private:
        double beta;
        int const_indx, j, k;
        double iVal;
        range_t r_range, phi_range;
        double dr,ds;
        const Vector<double,3> & origin;

      public:

        /** Evaluate something.
         * @param nphi_i
         *     Independent variable.
         */
        inline double distrib (const double & nphi_i) const {
            Vector<double,3> pos;
            pos[const_indx] = iVal;

            decoupleRhoPhi(nphi_i, r_range, phi_range, dr, ds, j, k, pos.val);

            pos += origin;

            return exp(-beta * bsrc.potentialNoG(pos) );
        }
    };

} /* namespace */

#endif // BTRAP_DISTRO_H
