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

/*
 * $Log: Vector.h,v $
 * Revision 1.1.1.1  2005/01/08 04:57:25  olsonse
 * Initial CVS import (finally).
 *
 */

/** \file
 * Generic SquareMatrix implementation that works nicely with the Vector class.
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


#ifndef olson_tools_SquareMatrix_h
#define olson_tools_SquareMatrix_h

#include <olson-tools/Vector.h>
#include <olson-tools/power.h>


#include <limits>
#include <sstream>
#include <ostream>

#include <cmath>
#include <cstring>
#include <cstdarg>
#include <stdint.h>


namespace olson_tools {

  /** Square matrix class.  The idea here is to provide a clean interface to
   * matrix calculations that are not too slow.  The size is compile time and
   * the compiler may opt to unroll loops and perform other optimizations.  
   */
  template <class T, unsigned int L>
  class SquareMatrix {
    /* TYPEDEFS */
  public:
    /** The length of the val array. */
    static const unsigned int length = L;

    /** The side length of the val matrix. */
    static const unsigned int side_length = L;



  /* MEMBER STORAGE */
  public:
    /** The internal storage array of the matrix elements. */
    T val[L][L];

    /** Default constructor does nothing. */
    inline SquareMatrix() {}

    /** SquareMatrix copy constructor--from another SquareMatrix. */
    inline SquareMatrix(const SquareMatrix & that) { *this = that; }

    /** SquareMatrix copy constructor--from a two dimensional C-array. */
    inline SquareMatrix(const T that[L][L]) { *this = that; }

    /** SquareMatrix assignment constructor.
     * Assigns all elements of the matrix to the given value that.
     */
    inline SquareMatrix(const T that) { *this = that; }

    /** Set all matrix elements to zero. */
    inline void zero () {
      memset (&this->val[0][0], 0, sizeof(T)*L*L);
    }

    /** Index operator--non-const version. */
    inline T & operator() (const int & i, const int & j) { return val[i][j]; }

    /** Index operator--const version. */
    inline const T & operator() (const int & i, const int & j) const { return val[i][j]; }

    /** Row extractor--non-const version. */
    inline Vector<T,L> & row(const int & i) { return VNCAST(T,L,val[i]); }

    /** Row extractor--const version. */
    inline const Vector<T,L> & row(const int & i) const { return VNCAST(T,L,val[i]); }

    /** Column extractor--const version (no non-const version exists). */
    inline Vector<T,L> col(const int & j) const {
      Vector<T,L> v;
      for (int i = 0; i < L; ++i) v[i] = val[i][j];
      return v;
    }

    /** Assignment operator--from SquareMatrix. */
    inline const SquareMatrix & operator= (const SquareMatrix & that) {
      memcpy (&this->val[0][0], &that.val[0][0], sizeof(T)*L*L);
      return *this;
    }

    /** Assignment operator--from two dimensional C-array. */
    inline const SquareMatrix & operator= (const T that[L][L]) {
      memcpy (&this->val[0][0], &that[0][0], sizeof(T)*L*L);
      return *this;
    }

    /** Assignment operator--from scalar. */
    inline const SquareMatrix & operator= (const T & that) {
      for (unsigned int i = 0; i < L; ++i ) {
        for (unsigned int j = 0; j < L; ++j) {
          this->val[i][j] = that;
        }
      }
      return *this;
    }

    /** Matrix inner product. */
    inline SquareMatrix operator* (const SquareMatrix & that) const {
      SquareMatrix result;

      for (unsigned int i = 0; i < L; ++i ) {
        for (unsigned int j = 0; j < L; ++j) {
          result.val[i][j] = 0;
          for (unsigned int k = 0; k < L; k++ ) {
            result.val[i][j] +=  this->val[i][k] * that.val[k][j];
          }
        }
      }

      return result;
    }

    /** Matrix subtraction. */
    inline SquareMatrix operator- (const SquareMatrix & that) const {
      SquareMatrix result;

      for (unsigned int i = 0; i < L; ++i ) {
        for (unsigned int j = 0; j < L; ++j) {
          result.val[i][j] = this->val[i][j] - that.val[i][j];
        }
      }

      return result;
    }

    /** Matrix addition. */
    inline SquareMatrix operator+ (const SquareMatrix & that) const {
      SquareMatrix result;

      for (unsigned int i = 0; i < L; ++i ) {
        for (unsigned int j = 0; j < L; ++j) {
          result.val[i][j] = this->val[i][j] + that.val[i][j];
        }
      }

      return result;
    }

    /** Matrix X Vector multiplication. */
    inline Vector<T,L> operator* (const Vector<T,L> & that) const {
      Vector<T,L> result;

      for (unsigned int i = 0; i < L; ++i ) {
        result.val[i] = 0;
        for (unsigned int j = 0; j < L; ++j) {
          result.val[i] += this->val[i][j] * that.val[j];
        }
      }

      return result;
    }

    /** Matrix + Scalar. */
    inline SquareMatrix operator+ (const T & that) const {
      SquareMatrix result;

      for (unsigned int i = 0; i < L; ++i ) {
        for (unsigned int j = 0; j < L; ++j) {
          result.val[i][j] = this->val[i][j] + that;
        }
      }

      return result;
    }

    /** Matrix - Scalar. */
    inline SquareMatrix operator- (const T & that) const {
      SquareMatrix result;

      for (unsigned int i = 0; i < L; ++i ) {
        for (unsigned int j = 0; j < L; ++j) {
          result.val[i][j] = this->val[i][j] - that;
        }
      }

      return result;
    }

    /** Matrix X Scalar immediate multiplication. */
    inline SquareMatrix & operator*= (const T & that) {
      for (unsigned int i = 0; i < L; ++i ) {
        for (unsigned int j = 0; j < L; ++j) {
          this->val[i][j] *= that;
        }
      }

      return *this;
    }

    /** Matrix - Scalar immediate subtraction. */
    inline SquareMatrix & operator-= (const T & that) {
      for (unsigned int i = 0; i < L; ++i ) {
        for (unsigned int j = 0; j < L; ++j) {
          this->val[i][j] -= that;
        }
      }

      return *this;
    }

    /** Matrix - Scalar immediate addition. */
    inline SquareMatrix & operator+= (const T & that) {
      for (unsigned int i = 0; i < L; ++i ) {
        for (unsigned int j = 0; j < L; ++j) {
          this->val[i][j] += that;
        }
      }

      return *this;
    }

    /** Matrix - Scalar immediate division. */
    inline SquareMatrix & operator/= (const T & that) {
      for (unsigned int i = 0; i < L; ++i ) {
        for (unsigned int j = 0; j < L; ++j) {
          this->val[i][j] /= that;
        }
      }

      return *this;
    }

    /** Transpose oeprator. */
    inline SquareMatrix & transpose() {
      for (unsigned int i = 0; i < L; ++i) {
        for (unsigned int j = 0; j < L; ++j) {
          T tmp = val[i][j];
          val[i][j] = val[j][i];
          val[j][i] = tmp;
        }
      }
      return *this;
    }

    /** component by component multiplication after type of that is converted
     * type of this.
     * @return reference to this (type SquareMatrix<T,L>).
     */
    template <class T2>
    inline const SquareMatrix & compMult(const SquareMatrix<T2,L>& that) {
      for (unsigned int i = 0; i < L; ++i)
        for (unsigned int j = 0; j < L; ++j)
          this->val[i][j] *= that.val[i][j];
      return *this;
    }

    /** component by component division after type of that is converted
     * type of this.
     * @return reference to this (type SquareMatrix<T,L>).
     */
    template <class T2>
    inline const SquareMatrix & compDiv(const SquareMatrix<T2,L>& that) {
      for (unsigned int i = 0; i < L; ++i)
        for (unsigned int j = 0; j < L; ++j)
          this->val[i][j] /= that.val[i][j];
      return *this;
    }

    /** Create (in temporary storage) an identity matrix equal in size to the
     * current SquareMatrix. */
    inline static SquareMatrix identity() {
      SquareMatrix retval;
      memset(&retval, 0, sizeof(retval));
      for (unsigned int i = 0; i < L; ++i ) {
        retval.val[i][i] = 1;
      }
      return retval;
    }


    /** Convert the SquareMatrix to a string with an optional delimiter between
     * columns (default: [tab]), and an optional delimiter between rows
     * (default: [newline]).
     *
     * @param col_delim
     *  Delimiter between adjacent columns [Default '\t'].
     *
     * @param row_delim
     *  Delimiter between adjacent rows [Default '\n'].
     * */
    inline std::string to_string( const char & col_delim = '\t',
                                  const char & row_delim = '\n' ) const {
      std::stringstream output;
      for (unsigned int i = 0; i < L; ++i) {
        for (unsigned int j = 0; j < L; ++j) {
          output << val[i][j] << col_delim;
        }
        output << row_delim;
      }
      return output.str( );
    }
  };

  template <class T, unsigned int L>
  inline SquareMatrix<T,L> transpose(const SquareMatrix<T,L> & m) {
    SquareMatrix<T,L> retval;
    for (unsigned int i = 0; i < L; ++i) {
      for (unsigned int j = 0; j < L; ++j) {
        retval(i,j) = m(j,i);
      }
    }
    return retval;
  }

  /** Component by component multiplication after type of that is converted
   * type of this.
   * @return reference to temporary (type SquareMatrix<T,L>).
   *
   * @see Vector::compDiv.
   */
  template <class T1, class T2, unsigned int L>
  inline const SquareMatrix<T1,L> compMult(const SquareMatrix<T1,L> & m1,
                                           const SquareMatrix<T2,L> & m2) {
    SquareMatrix<T1,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      for (unsigned int j = 0; j < L; ++j)
        retval.val[i][j] = m1.val[i][j] * m2.val[i][j];
    return retval;
  }

  /** Component by component division after type of that is converted
   * type of this.
   * @return reference to temporary (type SquareMatrix<T,L>).
   *
   * @see Vector::compMult.
   */
  template <class T1, class T2, unsigned int L>
  inline const SquareMatrix<T1,L> compDiv(const SquareMatrix<T1,L> & m1,
                                          const SquareMatrix<T2,L> & m2) {
    SquareMatrix<T1,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      for (unsigned int j = 0; j < L; ++j)
        retval.val[i][j] = m1.val[i][j] / m2.val[i][j];
    return retval;
  }

  /** Component by component power after type of that is converted
   * type of this.
   * @return reference to temporary (type SquareMatrix<T,L>).
   *
   * @see Vector::compMult.
   */
  template <class T1, class T2, unsigned int L>
  inline const SquareMatrix<T1,L> compPow(const SquareMatrix<T1,L> & m1,
                                          const SquareMatrix<T2,L> & m2) {
    SquareMatrix<T1,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      for (unsigned int j = 0; j < L; ++j)
        retval.val[i][j] = fast_pow(m1.val[i][j], m2.val[i][j]);
    return retval;
  }

  /** Component by component power after type of that is converted
   * type of this.
   * @return reference to temporary (type SquareMatrix<T,L>).
   *
   * @see Vector::compMult.
   */
  template <class T, unsigned int L>
  inline const SquareMatrix<T,L> compPow(const SquareMatrix<T,L> & m1,
                                         const double & e) {
    SquareMatrix<T,L> retval;
    for (unsigned int i = 0; i < L; ++i)
      for (unsigned int j = 0; j < L; ++j)
        retval.val[i][j] = fast_pow(m1.val[i][j], e);
    return retval;
  }

  /** Define SQR explicitly to be in the inner product of a SquareMatrix with its
   * self.  We define this specialization because the return value is not the
   * same as the arguments. */
  template <class T, unsigned int L>
  inline SquareMatrix<T,L> SQR(const SquareMatrix<T,L> & m) {
    return m*m;
  }

  /** Stream output operator. */
  template <class T, unsigned int L>
  inline std::ostream & operator<< (std::ostream & output, const SquareMatrix<T,L> & m) {
    for (unsigned int i = 0; i < L; ++i) {
      for (unsigned int j = 0; j < L; ++j) {
        output << m.val[i][j] << '\t';
      }
      output << '\n';
      //output << ";\t";
    }
    return output;
  }

  /** Stream input operator. */
  template <class T, unsigned int L>
  inline std::istream & operator>> (std::istream & input, SquareMatrix<T,L> & m) {
    for (unsigned int i = 0; i < L; ++i) {
      for (unsigned int j = 0; j < L; ++j) {
        input >> m.val[i][j];
      }
    }
    return input;
  }

}/* namespace olson_tools */

#endif // olson_tools_SquareMatrix_h
