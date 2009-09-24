// -*- c++ -*-
// $Id: Vector.h,v 1.1.1.1 2005/01/08 04:57:25 olsonse Exp $
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

/*
 * $Log: Vector.h,v $
 * Revision 1.1.1.1  2005/01/08 04:57:25  olsonse
 * Initial CVS import (finally).
 *
 */

/** \file
 * Generic Vector implementation that works nicely with the SquareMatrix class.
 * I used to say:<br>
 * BEGIN OLD COMMENTS<br>
 * I would hardly recommend using this stuff for time-sensitive operations
 * because of the inefficiency of using temporaries all over the place.  For
 * time-insensitive operations, these classes are really helpful as the code
 * is easier to write, looks cleaner, and much easier to read.
 * 
 * One <em>could</em> use parts of these classes in time-sensitive operations
 * if one is careful.  I don't want to go into all the caveats for
 * time-sensitive operations, but be assured they can be worked around.
 *   END OLD COMMENTS<br>
 *
 * But, I've noticed that compilers are pretty smart at optimizing temporaries
 * away and unrolling the loops for sizes ~3 (which are mostly what I use this
 * for).  Thus, in most cases, these classes should perform just fine.  
 *
 * Copyright 2004-2008 Spencer Olson
 */


#ifndef olson_tools_Vector_h
#define olson_tools_Vector_h

#include <olson-tools/power.h>


#include <limits>
#include <sstream>
#include <ostream>

#include <cmath>
#include <cstring>
#include <cstdarg>
#include <stdint.h>


namespace olson_tools {

  /** Simple define to help casting static N-element arrays to
   * N-Vectors.
   */
  #define VNCAST(t,n,a)          (*((olson_tools::Vector<t,n>*)(a)))

  /** Another simple define to help casting static 3-element arrays to
   * 3-Vectors.
   */
  #define V3C(a)          VNCAST(double,3,a)

  /** Vector class of arbitrary type.  The idea here is to provide a clean
   * interface to vector calculations that are not too slow.  The size is
   * compile time and the compiler may opt to unroll loops and perform other
   * optimizations.   
   */
  template <typename T, unsigned int L>
  class Vector {
    /* TYPEDEFS */
  public:
    /** The length of the val array. */
    static const unsigned int length = L;

  public:
    /** The internal storage array of the vector elements. */
    T val[L];

    /** Default constructor does not do anything. */
    inline Vector () {}

    /** Copy constructor--from a Vector. */
    template < typename T2 >
    inline Vector (const Vector<T2,L> & that) {
      using std::copy;
      copy( that.val, that.val+L, val);
    }

    /** Copy constructor--from an array. */
    inline Vector (const T that[L]) {
      using std::copy;
      copy( that, that+L, val );
    }

    /** Assignment constructor.
     * Assigns all elements of the vector to the given value that.
     */
    inline Vector (const T that) {*this = that;}

    /** Zero the Vector. */
    inline const Vector & zero () {
      memset (&this->val[0], 0, sizeof(T)*L);
      return *this;
    }

    /** Index operator--non-const version. */
    inline T & operator[] (const int & i) { return val[i]; }

    /** Index operator--const version. */
    inline const T & operator[] (const int & i) const { return val[i]; }

    /** Assignment operator--from Vector of different type. */
    template < typename TR >
    inline const Vector & operator= (const Vector<TR,L>& that) {
      using std::copy;
      copy( that.val, that.val+L, val );
      return *this;
    }

    /** Assignment operator--from array of same type. */
    inline const Vector & operator= (const T that[L]) {
      using std::copy;
      copy( that, that+L, val );
      return *this;
    }

    /** Assignment operator--from scalar value.  All elements will be assigned
     * to this scalar.  */
    inline const Vector & operator= (const T & that) {
      using std::fill;
      fill( val, val+L, that );
      return *this;
    }


    /* ***** MATH TYPE OPERATIONS THAT ARE MEMBERS. */
    /** Apply std::abs(double) to all elements and save.  
     * std::abs(double) is called regardless of the actual type of the vector. */
    inline const Vector & save_fabs() {
      for (unsigned int i = 0; i < L; ++i) this->val[i] = std::abs(this->val[i]);
      return *this;
    }

    /** Apply sqrt(double) to all elements and save. */
    inline const Vector & save_sqrt() {
      for (unsigned int i = 0; i < L; ++i) this->val[i] = std::sqrt(this->val[i]);
      return *this;
    }

    /** Vector X Scalar immediate multiplication. */  
    inline const Vector & operator*= (const T & that) {
      for (unsigned int i = 0; i < L; ++i) this->val[i] *= that;
      return *this;
    }

    /** Vector X Scalar immediate division. */  
    inline const Vector & operator/= (const T & that) {
      for (unsigned int i = 0; i < L; ++i) this->val[i] /= that;
      return *this;
    }

    /** Vector - Scalar immediate subtraction. */  
    inline const Vector & operator-= (const T& that) {
      for (unsigned int i = 0; i < L; ++i) this->val[i] -= that;
      return *this;
    }

    /** Vector - Vector immediate subtraction. */  
    inline const Vector & operator-= (const Vector& that) {
      for (unsigned int i = 0; i < L; ++i) this->val[i] -= that.val[i];
      return *this;
    }

    /** Vector + Vector immediate subtraction. */  
    inline const Vector & operator+= (const Vector& that) {
      for (unsigned int i = 0; i < L; ++i) this->val[i] += that.val[i];
      return *this;
    }

    /** Vector + Scalar immediate subtraction. */  
    inline const Vector & operator+= (const T& that) {
      for (unsigned int i = 0; i < L; ++i) this->val[i] += that;
      return *this;
    }

    /** component by component multiplication.
     * @return reference to this (type Vector<T,L>).
     */
    template <typename T2>
    inline const Vector & compMult(const Vector<T2,L>& that) {
      for (unsigned int i = 0; i < L; ++i)
        this->val[i] *= that.val[i];
      return *this;
    }

    /** component by component division after type of that is converted
     * type of this.
     * @return reference to this (type Vector<T,L>).
     */
    template <typename T2>
    inline const Vector & compDiv(const Vector<T2,L>& that) {
      for (unsigned int i = 0; i < L; ++i)
        this->val[i] /= that.val[i];
      return *this;
    }

    /** Compute the magnitude of this vector. */
    inline T abs () const {
      return std::sqrt ((*this) * (*this));
    }



    /** The product of all components of the Vector.
    */
    inline T prod() const {
      T retval = static_cast<T>(1);
      for (unsigned int i = 0; i < L; ++i) {
        retval *= this->val[i];
      }
      return retval;
    }

    /** Return a type casted Vector from the original. */
    template <typename T2>
    inline Vector<T2,L> to_type() const {
      Vector<T2,L> retval;
      retval = *this;
      return retval;
    }


    /** Add a fraction of another Vector to this Vector.
     * This operation can certainly be done by just doing:
     *          (*this) += f * that;
     * and it is possible that the above operation could result in exactly
     * this code if loop-unrolling/merging is able to be done, but I provide
     * this function just in case you want to be sure that you use a more
     * optimized version.  (Note that the compiler can certainly go ahead and
     * optimize--i.e. loop unroll--this function as well.  It just might be
     * easier for the compiler to produce efficient code from unrolling this
     * version.)
     */
    template <typename T2>
    inline Vector<T,L> addFraction(const double & f, const Vector<T2,L> & that) {
      for (unsigned int i = 0; i < L; ++i)
        this->val[i] += f * that.val[i];
      return *this;
    }


    /** Convert the Vector to a string with an optional delimiter (default: [tab]).
     * */
    inline std::string to_string( const char & delim = '\t') const {
      std::stringstream streamOut;
      if (L>0)
        streamOut << val[0];
      for (unsigned int i = 1; i < L; ++i) streamOut << delim << this->val[i];
      return streamOut.str( );
    }

  };




  /* ************** COMPARISON OPERATORS *************** { */

  /** Cumulative comparison (gt): Prod(Vector > scalar).
   * Comparison between Vectors of different types are allowed to use default
   * promotion of types.
   */
  template < typename TL, typename TR, unsigned int L >
  inline bool operator> (const Vector<TL,L> & lhs, const TR & rhs) {
    bool retval = true;
    for (unsigned int i = 0; i < L; ++i)
      retval = retval && (lhs[i] > rhs);

    return retval;
  }

  /** Cumulative comparison (gteq): Prod(Vector >= scalar).
   * Comparison between Vectors of different types are allowed to use default
   * promotion of types.
   */
  template < typename TL, typename TR, unsigned int L >
  inline bool operator>= (const Vector<TL,L> & lhs, const TR & rhs) {
    bool retval = true;
    for (unsigned int i = 0; i < L; ++i)
      retval = retval && (lhs[i] >= rhs);

    return retval;
  }

  /** Cumulative comparison (gt) between Vector and a Vector.
   * Comparison between Vectors of different types are allowed to use default
   * promotion of types.
   * @return cumulative expression of component-wise comparison.
   */
  template < typename TL, typename TR, unsigned int L >
  inline bool operator>  (const Vector<TL,L> & lhs, const Vector<TR,L> & rhs) {
    bool retval = true;
    for (unsigned int i = 0; i < L; ++i)
      retval = retval && (lhs[i] > rhs[i]);

    return retval;
  }

  /** Cumulative comparison (gteq) between Vector and a Vector.
   * Comparison between Vectors of different types are allowed to use default
   * promotion of types.
   * @return cumulative expression of component-wise comparison.
   */
  template < typename TL, typename TR, unsigned int L >
  inline bool operator>= (const Vector<TL,L> & lhs, const Vector<TR,L> & rhs) {
    bool retval = true;
    for (unsigned int i = 0; i < L; ++i)
      retval = retval && (lhs[i] >= rhs[i]);

    return retval;
  }

  /** Cumulative comparison (lt) between Vector and a scalar.
   * Comparison between Vectors of different types are allowed to use default
   * promotion of types.
   * @return cumulative expression of component-wise comparison.
   */
  template < typename TL, typename TR, unsigned int L >
  inline bool operator< (const Vector<TL,L> & lhs, const TR & rhs) {
    bool retval = true;
    for (unsigned int i = 0; i < L; ++i)
      retval = retval && (lhs[i] < rhs);

    return retval;
  }

  /** Cumulative comparison (leq) Vector <= scalar.
   * Comparison between Vectors of different types are allowed to use default
   * promotion of types.
   * @return cumulative expression of component-wise comparison.
   */
  template < typename TL, typename TR, unsigned int L >
  inline bool operator<= (const Vector<TL,L> & lhs, const TR & rhs) {
    bool retval = true;
    for (unsigned int i = 0; i < L; ++i)
      retval = retval && (lhs[i] <= rhs);

    return retval;
  }

  /** Cumulative comparison (lt) between Vector and a Vector.
   * Comparison between Vectors of different types are allowed to use default
   * promotion of types.
   * @return cumulative expression of component-wise comparison.
  */
  template < typename TL, typename TR, unsigned int L >
  inline bool operator< (const Vector<TL,L> & lhs, const Vector<TR,L> & rhs) {
    bool retval = true;
    for (unsigned int i = 0; i < L; ++i)
      retval = retval && (lhs[i] < rhs[i]);

    return retval;
  }

  /** Cumulative comparison (leq) between Vector and a Vector.
   * Comparison between Vectors of different types are allowed to use default
   * promotion of types.
   * @return cumulative expression of component-wise comparison.
  */
  template < typename TL, typename TR, unsigned int L >
  inline bool operator<= (const Vector<TL,L> & lhs, const Vector<TR,L> & rhs) {
    bool retval = true;
    for (unsigned int i = 0; i < L; ++i)
      retval = retval && (lhs[i] <= rhs[i]);

    return retval;
  }

  /** Cumulative '==' comparison of Vector types.
   * Comparison between Vectors of different types are allowed to use default
   * promotion of types.
   */
  template <typename TL, typename TR, unsigned int L>
  inline bool operator== (const Vector<TL,L>& lhs,
                          const Vector<TR,L>& rhs) {
    bool retval = true;
    for (unsigned int i = 0; i < L; ++i)
      retval = retval && (lhs[i] == rhs[i]);

    return retval;
  }

  /** Cumulative '!=' comparison of Vector types.
   * Comparison between Vectors of different types are allowed to use default
   * promotion of types.
   */
  template <typename TL, typename TR, unsigned int L>
  inline bool operator!= (const Vector<TL,L>& lhs,
                          const Vector<TR,L>& rhs) {
    return !(lhs == rhs);
  }

  /** Cumulative '==' comparison of Vector types.
   * Specialization:  Comparison between Vectors of doubles.  Equivalence is
   * defined by less than 4*M_EPS % difference--this is pretty strict I know.
   */
  template <unsigned int L>
  inline bool operator== (const Vector<double,L> & lhs,
                          const Vector<double,L> & rhs) {
    return equals( lhs, rhs );
  }

  /** Default comparision for equals(Vector<double,L>, Vector<double,L>). */
  static const double M_EPS4 = 4 * std::numeric_limits<double>::epsilon();

  /** Cumulative '==' comparison of Vector types.
   * Specialization:  Comparison between Vectors of doubles.  Equivalence is
   * defined by less than tol [Default tol = 4*M_EPS].
   */
  template <unsigned int L>
   inline bool equals( const Vector<double,L> & lhs,
                       const Vector<double,L> & rhs,
                       const double & eps = M_EPS4 ) {
    bool retval = true;
    for (unsigned int i = 0; i < L; ++i)
      retval = retval && ( std::abs(lhs[i] - rhs[i]) <= std::abs(eps*lhs[i]) );

    return retval;
  }


  /* **** END COMPARISION OPERATIONS **** }*/





  /* **** BEGIN MATH OPERATIONS **** { */

  /** Inner product of two Vectors. */
  template < typename TL, typename TR, unsigned int L >
  inline TL operator* (const Vector<TL,L> & lhs, const Vector<TR,L> & rhs) {
    TL retval(0);
    for (unsigned int i = 0; i < L; ++i)
      retval += lhs[i] * rhs[i];
    return retval;
  }

  /** Vector * Scalar  multiplication. */  
  template < typename T, unsigned int L >
  inline Vector<T,L> operator* (const Vector<T,L> & lhs, const T & rhs) {
    Vector<T,L> ret;
    for (unsigned int i = 0; i < L; ++i)
      ret[i] = lhs[i] * rhs;
    return ret;
  }

  /** Scalar * Vector  multiplication. */  
  template < typename T, unsigned int L >
  inline Vector<T,L> operator* (const T & lhs, const Vector<T,L> & rhs) {
    Vector<T,L> ret;
    for (unsigned int i = 0; i < L; ++i)
      ret[i] = lhs * rhs[i];
    return ret;
  }

  /** Vector / Scalar division. */
  template < typename T, unsigned int L >
  inline Vector<T,L> operator/ (const Vector<T,L> & lhs, const T & rhs) {
    Vector<T,L> ret;
    for (unsigned int i = 0; i < L; ++i)
      ret.val[i] = lhs[i] / rhs;
    return ret;
  }

  /** Vector - Vector subtraction. */  
  template < typename TL, typename TR, unsigned int L >
  inline Vector<TL,L> operator- (const Vector<TL,L> & lhs,
                                 const Vector<TR,L> & rhs) {
    Vector<TL,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      retval[i] = lhs[i] - rhs[i];
    return retval;
  }

  /** Vector - Scalar subtraction. */  
  template < typename T, unsigned int L >
  inline Vector<T,L> operator- (const Vector<T,L> & lhs, const T & rhs) {
    Vector<T,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      retval[i] = lhs[i] - rhs;
    return retval;
  }

  /** Scalar - Vector subtraction. */  
  template < typename T, unsigned int L >
  inline Vector<T,L> operator- (const T & lhs, const Vector<T,L> & rhs) {
    Vector<T,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      retval[i] = lhs - rhs[i];
    return retval;
  }

  /** Vector + Vector addition. */  
  template < typename TL, typename TR, unsigned int L >
  inline Vector<TL,L> operator+ (const Vector<TL,L> & lhs,
                                 const Vector<TR,L> & rhs) {
    Vector<TL,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      retval[i] = lhs[i] + rhs[i];
    return retval;
  }

  /** Vector + Scalar addition. */  
  template < typename T, unsigned int L >
  inline Vector<T,L> operator+ (const Vector<T,L> & lhs, const T & rhs) {
    Vector<T,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      retval[i] = lhs[i] + rhs;
    return retval;
  }

  /** Scalar + Vector addition. */  
  template < typename T, unsigned int L >
  inline Vector<T,L> operator+ (const T & lhs, const Vector<T,L> & rhs) {
    Vector<T,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      retval[i] = lhs[i] + rhs;
    return retval;
  }


  /** Vector X Vector cross product returned via a given input buffer. */
  template <typename T>
  inline void cross (      Vector<T,3> & retval,
                     const Vector<T,3> & a,
                     const Vector<T,3> & b) {
    retval[0] = a[1]*b[2] - a[2]*b[1];
    retval[1] = a[2]*b[0] - a[0]*b[2];
    retval[2] = a[0]*b[1] - a[1]*b[0];
  }

  /** Vector X Vector cross product returned via a temporary Vector. */
  template <typename T>
  inline Vector<T,3> cross (const Vector<T,3> & a,
                            const Vector<T,3> & b) {
    Vector<T,3> retval;
    cross( retval, a, b );
    return retval;
  }

  /** Vector .* Vector (matlab-like component-wise operation).
   * @return reference to temporary (type Vector<T1,L>).
   *
   * @see Vector::compDiv.
   */
  template <typename T1, typename T2, unsigned int L>
  inline const Vector<T1,L> compMult(const Vector<T1,L> & v1,
                                     const Vector<T2,L> & v2) {
    Vector<T1,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      retval[i] = v1[i] * v2[i];
    return retval;
  }

  /** Vector ./ Vector (matlab-like component-wise operation).
   * @return reference to temporary (type Vector<T1,L>).
   *
   * @see Vector::compMult.
   */
  template <typename T1, typename T2, unsigned int L>
  inline const Vector<T1,L> compDiv(const Vector<T1,L> & v1,
                                    const Vector<T2,L> & v2) {
    Vector<T1,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      retval[i] = v1[i] / v2[i];
    return retval;
  }

  /** Vector .^ Vector (matlab-like component-wise operation).
   * @return reference to temporary (type Vector<T1,L>).
   *
   * @see Vector::compMult.
   */
  template <typename T1, typename T2, unsigned int L>
  inline const Vector<T1,L> compPow(const Vector<T1,L> & v1,
                                    const Vector<T2,L> & v2) {
    Vector<T1,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      retval[i] = fast_pow(v1[i], v2[i]);
    return retval;
  }

  /** Vector .^ Scaler (matlab-like component-wise operation).
   * @return reference to temporary (type Vector<T,L>).
   *
   * @see Vector::compMult.
   */
  template <typename T,unsigned int L>
  inline const Vector<T,L> compPow(const Vector<T,L> & v1, const double & e) {
    Vector<T,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      retval[i] = fast_pow(v1[i], e);
    return retval;
  }

  /** Compute the maximum value in the Vector.
   */
  template <typename T, unsigned int L>
  inline T max (const Vector<T,L> & v) {
    T retval = v[0];
    for (unsigned int i = 1; i < L; ++i) {
      T vi = v[i];
      if (retval < vi)
        retval = vi;
    }
    return retval;
  }

  /** Calculate the mean value of this vector.
   * Because of truncation, this function doesn't really mean anything unless
   * the calculation is done with at least floating point precision.  Therefore,
   * this function returns a double (doing everything with double precision).
   * The user should round and cast back appropriately.
   */
  template <typename T, unsigned int L>
  inline double mean (const Vector<T,L> & v) {
    return sum(v) / static_cast<double>(L);
  }

  /** Compute the sum of the elements of the Vector. */
  template <typename T, unsigned int L>
  inline T sum (const Vector<T,L> & v) {
    T retval = static_cast<T>(0);
    for (unsigned int i = 0; i < L; ++i) {
      retval += v[i];
    }
    return retval;
  }

  /** Define SQR explicitly to be in the inner product of a Vector with its
   * self.  We define this specialization because the return value is not the
   * same as the arguments. */
  template <typename T, unsigned int L>
  inline T SQR(const Vector<T,L> & v) {
    return v*v;
  }

  /** Stream output operator. */
  template <typename T, unsigned int L>
  inline std::ostream & operator<< (std::ostream & output, const Vector<T,L> & v) {
    for (unsigned int i = 0; i < L; ++i) output << v[i] << '\t';
    return output;
  }

  /** Stream input operator. */
  template <typename T, unsigned int L>
  inline std::istream & operator>> (std::istream & input, Vector<T,L> & v) {
    for (unsigned int i = 0; i < L; ++i) input >> v[i];
    return input;
  }

  /* *** END MATH OPERATIONS ON VECTOR *** } */



  /** Generic implementation of make_vector class.  Note that this uses va_args
   * and as such is not bug friendly.  Common used lengths will be implemented
   * seperately with absolute arguments.
   */
  template < typename T, unsigned int L >
  struct make_vector {
    Vector<T,L> operator() ( const T & t0, ...) {
      Vector<T,L> retval;
      retval[0] = t0;

      // Now get the remaining items
      va_list ap;
      va_start(ap,t0);
      for ( unsigned int i = 1; i < L; ++i )
        retval[i] = va_arg(ap,T);
      va_end(ap);

      return retval;
    }
  };

  /** Specific implementation of make_vector class for length=3.  Providing this
   * will help avoid introducing bugs which va_arg stuff is prone to. 
   */
  template < typename T >
  struct make_vector<T,3u> {
    Vector<T,3u> operator() ( const T & t0, const T & t1, const T & t2 ) {
      Vector<T,3u> retval;
      retval[0] = t0;
      retval[1] = t1;
      retval[2] = t2;
      return retval;
    }
  };

  /** Easier form of make_vector for 3-vector of arbitrary type. */
  template < typename T >
  inline Vector<T,3u> V3( const T & a, const T & b, const T & c ) {
    return make_vector<T,3u>()(a,b,c);
  }

}/* namespace olson_tools */

#endif // olson_tools_Vector_h
