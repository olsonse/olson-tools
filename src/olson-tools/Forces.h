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

#ifndef FORCES_H
#define FORCES_H

#include "Vector.h"
#include "indices.h"
#include "Fields.h"

namespace olson_tools {

/** Container for a static function 'derivs' mainly for use with
 * integrating functions that accept a pointer to a derivative function.
 *
 * This version allows for a velocity dependence in the force. 
 * @see rk4step and company.
 */
template <class Force>
class Derivs {
  public:

    /** Compute phase-space derivatives for integrating methods (e.g.
     * runge-kutta.
     * Because this is a template, you will have to explicity instantiate this
     * function to get a pointer to pass into rk or the like.
     * Also, this function will have to be correctly cast to the rk type
     * (where f is a void *).
     */
    static void derivs( const double p[VZ+1],
                        const double * time,
                        const double * dt,
                        double rkf[VZ+1],
                        Force * f) {
        using namespace indices;
        rkf[X]  = p[VX];
        rkf[Y]  = p[VY];
        rkf[Z]  = p[VZ];
        f->accel(V3C(rkf+VX), V3C(p), V3C(p+VX), *time, *dt);
    }
};

/** Base of all force classes.  This class provides mass. */
class BaseForce {
  public:
    /** mass of a particle. */
    double mass;

    inline BaseForce() {
        mass = 0; /* whoah! a massless particle! */
    }

    inline const BaseForce & operator=(const BaseForce & that) {
        mass = that.mass;
        return *this;
    }

    /** All forces need to implement the accel function. */
    //inline void accel(Vector<double,3> & a, const Vector<double,3> & r, const double & t, const double & dt) const = 0;

    /** All forces need to implement the potential function. */
    //inline double potential(const Vector<double,3> & r, const double & t) const = 0;
};

class Gravity : public virtual BaseForce, public BgField< Vector<double,3> > {
  public:
    typedef BaseForce super0;
    typedef BgField< Vector<double,3> > super1;

    Gravity() : super0(), super1() {
        bg = V3(0.,0.,-9.81);
    }

    inline void accel(      Vector<double,3> & a,
                      const Vector<double,3> & r = V3(0.,0.,0.),
                      const Vector<double,3> & v = V3(0.,0.,0.),
                      const double & t = 0.0,
                      const double & dt = 0.0 ) const {
        a = super1::bg;
    }

    /** Calculate the potential of \f$^{87}{\rm Rb}\f$ |F=1,mF=-1>.
     * Gravitational energy is referenced to (0,0,0).
     */
    inline double potential(const Vector<double,3> & r,
                            const Vector<double,3> & v = V3(0.,0.,0.),
                            const double & t = 0.0) const {
        return - super0::mass * (super1::bg * r);
    }

    template <unsigned int ndim_>
    inline void applyStatisticalForce(Vector<double,ndim_> & particle,
                                      const double & t, const double & dt) const {}
};

/** Adds Forces to get a total acceleration/ potential energy.
 * Note that this is only really helpful for physically disjoint forces.  It
 * will not be physically correct to add two forces due to magnetic fields for
 * example.  In this case, one must add the fields together and then compute
 * the forces with BCalcs.
 *
 * An an example use of this class, let's add the forces each due to gravity and
 * magnetic fields:
 *      typedef AddForce< BCalcs< BSrc >, Gravity > myForce;
 *      myForce force;
 *      force.mass = 87.0*amu; // both forces use the BaseForce::mass value.
 *
 * Note that if F0 and F1 are from fields that use BaseField, this template
 * class will NOT cause their BaseField::delta values to be shared.
 *
 * @see BField::BCalcs.
 */
template <class _F0, class _F1>
class AddForce : public virtual BaseForce, public _F0, public _F1 {
  public:
    typedef BaseForce super0;
    typedef _F0 F0;
    typedef _F1 F1;

    AddForce() : super0(), F0(), F1() {}

    inline const AddForce & operator=(const AddForce & that) {
        super0::operator=(that);
        F0::operator=(that);
        F1::operator=(that);
        return *this;
    }

    inline void accel(      Vector<double,3> & a,
                      const Vector<double,3> & r,
                      const Vector<double,3> & v = V3(0,0,0),
                      const double & t = 0.0,
                      const double & dt = 0.0) const {
        F0::accel(a,r,v,t,dt);
        Vector<double,3> a2;
        F1::accel(a2,r,v,t,dt);
        a +=  a2;
    }

    inline double potential(const Vector<double,3> & r,
                            const Vector<double,3> & v = V3(0,0,0),
                            const double & t = 0.0) const {
        return F0::potential(r,v,t) + F1::potential(r,v,t);
    }

    template <unsigned int ndim_>
    inline void applyStatisticalForce(Vector<double,ndim_> & particle,
                                      const double & t, const double & dt) const {
        F0::applyStatisticalForce( particle, t, dt );
        F1::applyStatisticalForce( particle, t, dt );
    }
};


/** A wrapper class for a statistical force for using with RK5 driver. */
template <class _StatisticalForce>
class StatisticalForceRKWrapper {
  public:
    StatisticalForceRKWrapper() : statisticalForcePtr(NULL) {}
    _StatisticalForce * statisticalForcePtr;

    inline void rkTweakFirst(Vector<double,6> & particle,
               const double & t,
               const double & dt_step_current,
               double & dt_step_next) {
        statisticalForcePtr->applyStatisticalForce(particle, t, 0.5*dt_step_current);
    }

    inline void rkTweakSecond(Vector<double,6> & particle,
               const double & t,
               const double & dt_step_current,
               double & dt_step_next) {
        statisticalForcePtr->applyStatisticalForce(particle, t, 0.5*dt_step_current);
    }
};


}/* namespace olson_tools */

#endif // FORCES_H
