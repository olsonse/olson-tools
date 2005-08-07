// -*- c++ -*-
// $Id: Vector.h,v 1.1.1.1 2005/01/08 04:57:25 olsonse Exp $
/*
 * Copyright 2004 Spencer Olson
 *
 * $Log: Vector.h,v $
 * Revision 1.1.1.1  2005/01/08 04:57:25  olsonse
 * Initial CVS import (finally).
 *
 *
 */

/** \file
 * Generic Vector and SquareMatrix implementations that work together nicely.
 * I would hardly recommend using this stuff for time-sensitive operations
 * because of the inefficiency of using temporaries all over the place.  For
 * time-insensitive operations, these classes are really helpful as the code
 * is easier to write, looks cleaner, and much easier to read.
 * 
 * One <em>could</em> use parts of these classes in time-sensitive operations
 * if one is careful.  I don't want to go into all the caveats for
 * time-sensitive operations, but be assured they can be worked around.
 *
 * Copyright 2004 Spencer Olson
 */


#ifndef STUFF_H
#define STUFF_H

#include <string.h>
#include <ostream>

/** A simple define to help make it easier and cleaner to initialize
 * 3-Vectors. */
#define V3(a,b,c)       (*(const Vector<double,3>*)((const double[3]){double((a)),double((b)),double((c))}))

/** Another simple define to help casting static 3-element arrays to
 * 3-Vectors.
 */
#define V3C(a)          (*((Vector<double,3>*)(a)))

template <class T, unsigned int L>
class Vector {
  public:
    T val[L];

    inline Vector () {}

    inline Vector (const Vector & that) {*this = that;}

    inline Vector (const T that[L]) {*this = that;}

    inline Vector (const T that) {*this = that;}

    inline void zero () {
        memset (&this->val[0], 0, sizeof(T)*L);
    }

    inline T & operator[](const int & i) { return val[i]; }
    inline const T & operator[](const int & i) const { return val[i]; }

    inline const Vector & operator=(const Vector & that) {
        memcpy (&this->val[0], &that.val[0], sizeof(T)*L);
        return *this;
    }

    inline const Vector & operator=(const T that[L]) {
        memcpy (&this->val[0], &that[0], sizeof(T)*L);
        return *this;
    }

    inline const Vector & operator=(const T & that) {
        for (unsigned int i = 0; i < L; i++) this->val[i] = that;
        return *this;
    }

    inline T operator*(const Vector & that) const {
        T retval(0);
        for (unsigned int i = 0; i < L; i++) retval += this->val[i]*that.val[i];
        return retval;
    }

    inline Vector operator*(const T & that) const {
        Vector ret;
        for (unsigned int i = 0; i < L; i++) ret.val[i] = this->val[i] * that;
        return ret;
    }

    friend inline Vector operator*(const T & that, const Vector & v) {
        Vector ret;
        for (unsigned int i = 0; i < L; i++) ret.val[i] = v.val[i] * that;
        return ret;
    }

    inline const Vector & operator*=(const T & that) {
        for (unsigned int i = 0; i < L; i++) this->val[i] *= that;
        return *this;
    }

    inline Vector operator/(const T & that) const {
        Vector ret;
        for (unsigned int i = 0; i < L; i++) ret.val[i] = this->val[i] / that;
        return ret;
    }

    inline const Vector & operator/=(const T & that) {
        for (unsigned int i = 0; i < L; i++) this->val[i] /= that;
        return *this;
    }

    inline Vector operator-(const Vector& that) const {
        Vector retval;
        for (unsigned int i = 0; i < L; i++) retval.val[i] = this->val[i]-that.val[i];
        return retval;
    }

    inline Vector operator-(const T& that) const {
        Vector retval;
        for (unsigned int i = 0; i < L; i++) retval.val[i] = this->val[i]-that;
        return retval;
    }

    inline const Vector & operator-=(const T& that) {
        for (unsigned int i = 0; i < L; i++) this->val[i] -= that;
        return *this;
    }

    inline Vector operator+(const Vector& that) const {
        Vector retval;
        for (unsigned int i = 0; i < L; i++) retval.val[i] = this->val[i]+that.val[i];
        return retval;
    }

    inline Vector operator+(const T& that) const {
        Vector retval;
        for (unsigned int i = 0; i < L; i++) retval.val[i] = this->val[i]+that;
        return retval;
    }

    inline const Vector & operator+=(const Vector& that) {
        for (unsigned int i = 0; i < L; i++) this->val[i] += that.val[i];
        return *this;
    }

    inline const Vector & operator+=(const T& that) {
        for (unsigned int i = 0; i < L; i++) this->val[i] += that;
        return *this;
    }

    inline T abs () const {
        return sqrt ((*this) * (*this));
    }

};

template <class T>
inline Vector<T,3> cross (const Vector<T,3> & a, const Vector<T,3> b) {
    Vector<T,3> retval;
    retval.val[0] = a.val[1]*b.val[2] - a.val[2]*b.val[1];
    retval.val[1] = a.val[2]*b.val[0] - a.val[0]*b.val[2];
    retval.val[2] = a.val[0]*b.val[1] - a.val[1]*b.val[0];
    return retval;
}

template <class T, unsigned int L>
inline std::ostream & operator<<(std::ostream & output, const Vector<T,L> & v) {
    for (unsigned int i = 0; i < L; i++) output << v.val[i] << '\t';
    return output;
}

template <class T, unsigned int L>
class SquareMatrix {
  public:
    T val[L][L];

    inline SquareMatrix() {}

    inline SquareMatrix(const SquareMatrix & that) {
        *this = that;
    }

    inline SquareMatrix(const T that[L][L]) {
        *this = that;
    }

    inline const SquareMatrix & operator=(const SquareMatrix & that) {
        memcpy (&this->val[0][0], &that.val[0][0], sizeof(T)*L*L);
        return *this;
    }

    inline const SquareMatrix & operator=(const T that[L][L]) {
        memcpy (&this->val[0][0], &that[0][0], sizeof(T)*L*L);
        return *this;
    }

    inline const SquareMatrix & operator=(const T & that) {
        for (unsigned int i = 0; i < L; i++ ) {
            for (unsigned int j = 0; j < L; j++) {
                this->val[i][j] = that;
            }
        }
        return *this;
    }

    inline SquareMatrix operator *(const SquareMatrix & that) const {
        SquareMatrix result;

        for (unsigned int i = 0; i < L; i++ ) {
            for (unsigned int j = 0; j < L; j++) {
                result.val[i][j] = 0;
                for (unsigned int k = 0; k < L; k++ ) {
                    result.val[i][j] +=  this->val[i][k] * that.val[k][j];
                }
            }
        }

        return result;
    }

    inline SquareMatrix operator -(const SquareMatrix & that) const {
        SquareMatrix result;

        for (unsigned int i = 0; i < L; i++ ) {
            for (unsigned int j = 0; j < L; j++) {
                result.val[i][j] = this->val[i][j] - that.val[i][j];
            }
        }

        return result;
    }

    inline SquareMatrix operator +(const SquareMatrix & that) const {
        SquareMatrix result;

        for (unsigned int i = 0; i < L; i++ ) {
            for (unsigned int j = 0; j < L; j++) {
                result.val[i][j] = this->val[i][j] + that.val[i][j];
            }
        }

        return result;
    }

    inline Vector<T,L> operator *(const Vector<T,L> & that) const {
        Vector<T,L> result;

        for (unsigned int i = 0; i < L; i++ ) {
            result.val[i] = 0;
            for (unsigned int j = 0; j < L; j++) {
                result.val[i] += this->val[i][j] * that.val[j];
            }
        }

        return result;
    }

    inline SquareMatrix operator +(const T & that) const {
        SquareMatrix result;

        for (unsigned int i = 0; i < L; i++ ) {
            for (unsigned int j = 0; j < L; j++) {
                result.val[i][j] = this->val[i][j] + that;
            }
        }

        return result;
    }

    inline SquareMatrix operator -(const T & that) const {
        SquareMatrix result;

        for (unsigned int i = 0; i < L; i++ ) {
            for (unsigned int j = 0; j < L; j++) {
                result.val[i][j] = this->val[i][j] - that;
            }
        }

        return result;
    }

    inline SquareMatrix & operator *=(const T & that) {
        for (unsigned int i = 0; i < L; i++ ) {
            for (unsigned int j = 0; j < L; j++) {
                this->val[i][j] *= that;
            }
        }

        return *this;
    }

    inline SquareMatrix & operator -=(const T & that) {
        for (unsigned int i = 0; i < L; i++ ) {
            for (unsigned int j = 0; j < L; j++) {
                this->val[i][j] -= that;
            }
        }

        return *this;
    }

    inline SquareMatrix & operator +=(const T & that) {
        for (unsigned int i = 0; i < L; i++ ) {
            for (unsigned int j = 0; j < L; j++) {
                this->val[i][j] += that;
            }
        }

        return *this;
    }

    inline SquareMatrix & operator /=(const T & that) {
        for (unsigned int i = 0; i < L; i++ ) {
            for (unsigned int j = 0; j < L; j++) {
                this->val[i][j] /= that;
            }
        }

        return *this;
    }

    inline static SquareMatrix identity() {
        SquareMatrix retval;
        memset(&retval, 0, sizeof(retval));
        for (unsigned int i = 0; i < L; i++ ) {
            retval.val[i][i] = 1;
        }
        return retval;
    }
};

#endif // STUFF_H
