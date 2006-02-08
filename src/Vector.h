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


#ifndef VECTOR_H
#define VECTOR_H

#include <string.h>
#include <ostream>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>

/** A simple define to help make it easier and cleaner to initialize
 * 3-Vectors (of a certain type).
 * Because this uses va_arg stuff: single byte types are only possible with
 * specific implementations of stdarg.h (such as Intel's).  GCC will issue a
 * warning and then barf if the code is actually executed.
 * Because of this inportability, you cannot use single byte types with this
 * macro (or Vector<T,L>::Vector(Vector::list*,...) either for that matter).
 *
 * For single byte types, V3t_ can be used.
 */
#define V3t(type,a,b,c)       __CONCAT(vect3,type)(__CONCAT(vect3,type)::list::dummy(),((type)(a)),((type)(b)),((type)(c)))

/** Another simple define to help make it easier and cleaner to init 3-vectors
 * (of a given type).  This macro can usually (compiler dependent) only
 * helpful if the arguments are primitive constants.
 */
#define V3t_(type,a,b,c)       (*(Vector<type,3>*)((const type[3]){((type)(a)),((type)(b)),((type)(c))}))

/** A simple define to help make it easier and cleaner to initialize
 * 3-Vectors (of doubles). */
#define V3(a,b,c)       V3t(double,a,b,c)

/** Another simple define to help casting static 3-element arrays to
 * 3-Vectors.
 */
#define V3C(a)          (*((Vector<double,3>*)(a)))

template <class T, unsigned int L>
class Vector {
  public:
    class list {
      public:
        static const list * dummy() {return NULL;}
    };

    T val[L];

    inline Vector () {}

    inline Vector (const Vector & that) {*this = that;}

    inline Vector (const T that[L]) {*this = that;}

    inline Vector (const T that) {*this = that;}

    /** Copy list of values into Vector.
     * @param dummy
     *     a bogus pointer (casting NULL appropriately is fine).
     */
    inline Vector (const Vector::list * dummy, ...) {
        va_list ap;
        va_start(ap,dummy);
        for (unsigned int i = 0; i < L; i++) this->val[i] = va_arg(ap,T);
        va_end(ap);
    }

    inline void zero () {
        memset (&this->val[0], 0, sizeof(T)*L);
    }

    inline void save_fabs() {
        for (unsigned int i = 0; i < L; i++) this->val[i] = fabs(this->val[i]);
    }

    inline void save_sqrt() {
        for (unsigned int i = 0; i < L; i++) this->val[i] = sqrt(this->val[i]);
    }

    inline T & operator[](const int & i) { return val[i]; }
    inline const T & operator[](const int & i) const { return val[i]; }

    template <class T2>
    inline const Vector & operator=(const Vector<T2,L>& that) {
        for (unsigned int i = 0; i < L; i++) this->val[i] = (T)that.val[i];
        return *this;
    }

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

    inline const Vector & operator-=(const Vector& that) {
        for (unsigned int i = 0; i < L; i++) this->val[i] -= that.val[i];
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

    /** component by component multiplication after type of that is converted
     * type of this.
     * @return reference to this (type Vector<T,L>).
     */
    template <class T2>
    inline const Vector & compMult(const Vector<T2,L>& that) {
        for (unsigned int i = 0; i < L; i++) this->val[i] *= (T)that.val[i];
        return *this;
    }

    /** component by component division after type of that is converted
     * type of this.
     * @return reference to this (type Vector<T,L>).
     */
    template <class T2>
    inline const Vector & compDiv(const Vector<T2,L>& that) {
        for (unsigned int i = 0; i < L; i++) this->val[i] /= (T)that.val[i];
        return *this;
    }


    inline T abs () const {
        return sqrt ((*this) * (*this));
    }


    /* ************** COMPARISION OPERATORS *************** */

    /** Cummulative comparison (gt) between Vector and a scalar.
    */
    inline bool operator>(const T& that) {
        bool retval = true;
        for (unsigned int i = 0; i < L; i++)
            retval = retval && (this->val[i] > that);

        return retval;
    }

    /** Cummulative comparison (gt) between Vector and a Vector.
     * @return cummulative expression of component-wise comparision.
    */
    template <class T2>
    inline bool operator>(const Vector<T2,L>& that) {
        bool retval = true;
        for (unsigned int i = 0; i < L; i++)
            retval = retval && (this->val[i] > (T)that.val[i]);

        return retval;
    }

    /** Cummulative comparison (lt) between Vector and a scalar.
    */
    inline bool operator<(const T& that) {
        bool retval = true;
        for (unsigned int i = 0; i < L; i++)
            retval = retval && (this->val[i] < that);

        return retval;
    }

    /** Cummulative comparison (lt) between Vector and a Vector.
     * @return cummulative expression of component-wise comparision.
    */
    template <class T2>
    inline bool operator<(const Vector<T2,L>& that) {
        bool retval = true;
        for (unsigned int i = 0; i < L; i++)
            retval = retval && (this->val[i] < (T)that.val[i]);

        return retval;
    }

    /** The product of all components of the Vector.
    */
    inline T prod() const {
        T retval = (T)1;
        for (unsigned int i = 0; i < L; i++) {
            retval *= this->val[i];
        }
        return retval;
    }

    /** Return a type casted Vector from the original. */
    template <class T2>
    inline Vector<T2,L> to_type() const {
        Vector<T2,L> retval;
        retval = *this;
        return retval;
    }


    /** Add a fraction of another Vector to this Vector. */
    template <class T2>
    inline Vector<T,L> addFraction(const double & f, const Vector<T2,L> & that) {
        for (unsigned int i = 0; i < L; i++) this->val[i] += (T)(f*that.val[i]);
        return *this;
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

template <class T>
inline void cross (Vector<T,3> &retval, const Vector<T,3> & a, const Vector<T,3> b) {
    retval.val[0] = a.val[1]*b.val[2] - a.val[2]*b.val[1];
    retval.val[1] = a.val[2]*b.val[0] - a.val[0]*b.val[2];
    retval.val[2] = a.val[0]*b.val[1] - a.val[1]*b.val[0];
}

/** component by component multiplication after type of that is converted
 * type of this.
 * @return reference to this (type Vector<T,L>).
 *
 * @see Vector::compMult.
 */
template <class T1, class T2, unsigned int L>
inline const Vector<T1,L> compMult(const Vector<T1,L> & v1, const Vector<T2,L>& v2) {
    Vector<T1,L> retval;
    for (unsigned int i = 0; i < L; i++)
        retval.val[i] = v1.val[i] * (T1)v2.val[i];
    return retval;
}

/** component by component division after type of that is converted
 * type of this.
 * @return reference to (type Vector<T,L>).
 *
 * @see Vector::compDiv.
 */
template <class T1, class T2, unsigned int L>
inline const Vector<T1,L> compDiv(const Vector<T1,L> & v1, const Vector<T2,L>& v2) {
    Vector<T1,L> retval;
    for (unsigned int i = 0; i < L; i++)
        retval.val[i] = v1.val[i] / (T1)v2.val[i];
    return retval;
}

template <class T, unsigned int L>
inline T max (const Vector<T,L> & v) {
    T retval = v[0];
    for (unsigned int i = 1; i < L; i++) {
        T vi = v[i];
        if (retval < vi) retval = vi;
    }
    return retval;
}

template <class T, unsigned int L>
inline T mean (const Vector<T,L> & v) {
    return sum(v) / ((T)L);
}

template <class T, unsigned int L>
inline T sum (const Vector<T,L> & v) {
    T retval = (T)0;
    for (unsigned int i = 0; i < L; i++) {
        retval += v[i];
    }
    return retval;
}

template <class T, unsigned int L>
inline std::ostream & operator<<(std::ostream & output, const Vector<T,L> & v) {
    for (unsigned int i = 0; i < L; i++) output << v.val[i] << '\t';
    return output;
}

template <class T, unsigned int L>
inline std::istream & operator>>(std::istream & input, Vector<T,L> & v) {
    for (unsigned int i = 0; i < L; i++) input >> v.val[i];
    return input;
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

/* these are kludgy types to be used with the kludgy V3t macro. */
typedef Vector<double,3> vect3double;
typedef Vector<float,3> vect3float;
typedef Vector<int,3> vect3int;
typedef Vector<uint32_t,3> vect3uint32_t;

#endif // VECTOR_H
