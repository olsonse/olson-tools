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

/** \example field/lookup/testfield.C
 * \input field/lookup/common.h
 *
 * Demonstrates the field lookup code by using a set of field/force data from
 * a previously calculated field file.  The data pertains to magnetic forces
 * on an atom in the trapping hyperfine ground-state.
 */

/** \example field/lookup/createfieldfile.C
 * \input field/lookup/common.h
 *
 * Demonstrates the field field writing utility that produces a file that can
 * be read in by the field-lookup code.  The data pertains to magnetic forces
 * on an atom in the trapping hyperfine ground-state.
 */

#ifndef FIELDS_H
#define FIELDS_H

#include "Vector.h"
#include "indices.h"

namespace olson_tools {
    using namespace indices;

#if 0
            //Gradient::gradient(a, *static_cast< magnitude_of<super1> *>((super1*)this), r);
#define MAGNITUDE_OF(T,CP) *static_cast< T * > ( (T*) CP )

template <class VectorField>
class magnitude_of : public VectorField {
  public:
    typedef VectorField super;

    inline double operator()(const Vector<double,3> & r) const {
        Vector<double,3> F1;
        super::operator()(F1, r);
        return F1.abs();
    }
};

#else
template <class VectorField>
class magnitude_of {
  public:
    magnitude_of(const VectorField & vf) : f(vf), delta(vf.delta) {}

    inline double operator()(const Vector<double,3> & r) const {
        Vector<double,3> F1;
        f(F1, r);
        return F1.abs();
    }

  private:
    const VectorField & f;
  public:
    const double & delta;
};
#endif

    /** Gradient of the magnitude of a vector field.
     * An example of where this gradient is useful is in computing
     * accelerations of atoms trapped in magnetic fields.  The assumption is
     * that the projection of the magnetic moment of the atom remains constant
     * (adiabatic following of the field).
     *
     * An alternative to this function is to use : 
     * magnitude_of(VectorField) to convert VectorField into a scalar field.
     * The speed of these various methods may depend on the compiler.  There
     * may also be no difference between these (for at least the Intel
     * compiler I think).
     *
     * Example:
     * Vector<double,3> g; gradient(g, magnitude_of<T>(f), r);
     */
    template <class VectorField>
    inline Vector<double,3> &
    gradient_of_magnitude(Vector<double,3> & GradFmag,
                          VectorField & f,
                          const Vector<double,3> & r) {
        Vector<double,3> F1, F2, dr;
        register double dxh = 0.5 * f.delta;
        for (int j=X; j <= Z; j++) {
            dr = r;

            dr[j] = r[j] - dxh;
            f(F1, dr);

            dr[j] = r[j] + dxh;
            f(F2, dr);

            /* now calculate the field gradient centered at the specified location. */
            GradFmag[j] = (F2.abs() - F1.abs()) / f.delta;
        }/* for */
        return GradFmag;
    }

    /** Gradient of a scalar field.
     * The ScalarField must have the operator()(const Vector<double,3>)
     * function defined to return the scalar value of the field.
     */
    template <class ScalarField>
    inline Vector<double,3> & gradient(Vector<double,3> & GradF,
                                       ScalarField & f,
                                       const Vector<double,3> & r) {
        Vector<double,3> dr;
        double V1, V2;
        register double dxh = 0.5 * f.delta;
        for (int j=X; j <= Z; j++) {
            dr = r;

            dr[j] = r[j] - dxh;
            V1 = f(dr);

            dr[j] = r[j] + dxh;
            V2 = f(dr);

            /* now calculate the field gradient centered at the specified location. */
            GradF[j] = (V2 - V1) / f.delta;
        }/* for */
        return GradF;
    }


/** Base of field type classes.
 * Scalar fields must implement the operator()(r) (returns doubles) and
 * Vector fields must implement the operator()(F,r) (void return value).
 *
 * AddField requires fields to typedef the base type as 'typedef T base' where
 * T is the type of the field (double/float/int/... for scalar and Vector<...>
 * for vector fields).
 *
 * By inheriting this class, a derived Field class gains the 'delta' member
 * which is used for computing gradients.  Inheritance of this class should
 * normally be done virtually. 
 */
class BaseField {
  public:
    BaseField() : delta(1e-6) {}

    inline const BaseField & operator=(const BaseField & that) {
        delta = that.delta;
        return *this;
    }

    /** Distance between points that field is calculated for computing
     * gradient.
     * Note that dx == dy == dz for computing gradient. */
    double delta;
};


/** A static background vector field.
 * @param T
 *     T is the type of Field : Vector<double,3>, double(scalar), etc.
 */
template <class T>
class BgField : public virtual BaseField {
  public:
    typedef BaseField super;
    typedef T         base; /* required by AddField. */

    BgField() : super() {
        bg = 0.0;
    }

    inline const BgField & operator=(const BgField & that) {
        super::operator=(that);
        bg = that.bg;
        return *this;
    }

    /** Returns static background field. */
    inline void operator()(T & B, const Vector<double,3> & r) const {
        B = bg;
    }

    /** Returns static background field. */
    inline const T & operator()(const Vector<double,3> & r) const {
        return bg;
    }

    T bg;
};


/** Adds Fields from two different sources. 
 * The sources can be both scalar, vector, as well as mixed type fields.  The
 * only caveat is that for mixed field types, F0 cannot be a scalar field as
 * any operation of the type Vector+scalar is defined to result in a Vector
 * (where the scalar addition was applied to each component of the vector). 
 */
template <class F0, class F1>
class AddField : public virtual BaseField, public F0, public F1 {
  public:
    typedef BaseField super0;

    AddField() : super0(), F0(), F1() {}

    inline const AddField & operator=(const AddField & that) {
        super0::operator=(that);
        F0::operator=(that);
        F1::operator=(that);
        return *this;
    }

    /** Adds two fields together.  
     * This function requires that F0::base is typedefed.  
     *
     * This should work both Vector and Scalar fields, although the user
     * should verify that the efficiency is not limited for the application
     * at hand.  This is ideally suited for Scalar fields, but should work
     * for both.  It might even work for mixed field types, although I'm not
     * sure why you would want to do this.  It is entirely possible that
     * this function will not instantiate in some cases.
     * */
    inline typename F0::base operator()(const Vector<double,3> & r) const {
        return F0::operator()(r) + F1::operator()(r);
    }

    /** Adds two fields together. This should really be used by Vector Fields. */
    inline void operator()(typename F0::base & F, const Vector<double,3> & r) const {
        F0::operator()(F,r);
        typename F1::base F2;
        F1::operator()(F2,r);
        F += F2;
    }
};


/** Turns a Functor class into a Field source. */
template <class Functor>
class FieldFunctor : public virtual BaseField, public Functor {
  public:
    typedef BaseField super0;
    typedef Functor  super1;

    FieldFunctor() : super0(), super1() {}

    inline const FieldFunctor & operator=(const FieldFunctor & that) {
        super0::operator=(that);
        super1::operator=(that);
        return *this;
    }

    /* Functor must implement the operator()(F,r) method for VectorFields and
     * it must implement the operator()(r) for ScalarFields. */
};

}/* namespace olson_tools */

#endif // FIELDS_H
