// -*- c++ -*-
// $Id$
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2004-2008 Spencer Olson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *  
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *                                                                                 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.                                                                           .
 * 
 * Questions? Contact Spencer Olson (olsonse@umich.edu) 
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

#include <olson-tools/bfield.h>
#include <olson-tools/logger.h>
#include <olson-tools/Vector.h>
#include <olson-tools/InvCylindricalDist.h>

#include <physical/physical.h>

namespace olson_tools { namespace BField {

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
            beta(1.0 / (physical::constant::si::K_B * init.T_perp * physical::units::K)),
            const_indx(i),
            iVal(iVal_),
            dr(dr_), ds(ds_),
            origin(origin_) {

            if (i < 0 || i > 2) THROW(std::runtime_error,FILE_LINE "invalid index");

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
        inline double operator() (const double & nphi_i) const {
            Vector<double,3> pos;
            pos[const_indx] = iVal;

            decoupleRhoPhi(nphi_i, r_range, phi_range, dr, ds, j, k, pos.val);

            pos += origin;

            return exp(-beta * bsrc.potential(pos) );
        }
    };

}} /* namespace olson_tools::BField */

#endif // BTRAP_DISTRO_H
