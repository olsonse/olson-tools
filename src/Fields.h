// -*- c++ -*-
// $Id$
/*
 * Copyright 2004-2007 Spencer Olson
 *
 * $Log$
 */

#ifndef FIELDS_H
#define FIELDS_H

#include "Vector.h"

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

/** Adds VectorFields from two different sources. */
template <class VF0, class VF1>
class AddVectorField : public virtual BaseField, public VF0, public VF1 {
  public:
    typedef BaseField super0;

    AddVectorField() : super0(), VF0(), VF1() {}

    inline const AddVectorField & operator=(const AddVectorField & that) {
        super0::operator=(that);
        VF0::operator=(that);
        VF1::operator=(that);
        return *this;
    }

    /** Adds two fields together. */
    inline void operator()(Vector<double,3> & F, const Vector<double,3> & r) const {
        VF0::operator()(F,r);
        Vector<double,3> F2;
        VF1::operator()(F2,r);
        F += F2;
    }
};

/** Adds ScalarFields from two different sources. */
template <class SF0, class SF1>
class AddScalarField : public virtual BaseField, public SF0, public SF1 {
  public:
    typedef BaseField super0;

    AddScalarField() : super0(), SF0(), SF1() {}

    inline const AddScalarField & operator=(const AddScalarField & that) {
        super0::operator=(that);
        SF0::operator=(that);
        SF1::operator=(that);
        return *this;
    }

    /** Adds two fields together. */
    inline double operator()(const Vector<double,3> & r) const {
        return SF0::operator()(r) + SF1::operator()(r);
    }
};

/** Adds Fields from two different sources. 
 * I'm pretty sure that this class supersedes AddScalerField and
 * AddVectorField, so don't use those if possible.
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

    /** Adds two fields together. This should really be used by Scalar Fields.
     * This will not instantiate with Vector Fields. */
    inline double operator()(const Vector<double,3> & r) const {
        return F0::operator()(r) + F1::operator()(r);
    }

    /** Adds two fields together. This should really be used by Vector Fields.
     * This may not instantiate with all Scalar Fields that I've made. */
    template <class T>
    inline void operator()(T & F, const Vector<double,3> & r) const {
        F0::operator()(F,r);
        Vector<double,3> F2;
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


#endif // FIELDS_H
