/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2004-2008 Spencer E. Olson
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

#ifndef olson_tools_upper_triangle_h
#define olson_tools_upper_triangle_h

#include <vector>

namespace olson_tools {

  /** The symmetric fix for the order of indices for the upper_triangular class.
   * This class helps guarantee that the indices are in the right order for the
   * accessor functions of the upper_triangle class.
   *
   * @see upper_triangle
   */
  struct SymmetryFix {
    void operator()(int & i, int & j) {
      if (j < i)
        std::swap(i,j);
    }
  };

  /** The NO-OP fix for the order of indices for the upper_triangular class. */
  struct NullSymmetryFix {
    void operator()(int & i, int & j) {}
  };


  /** Upper triangular storage of a n x n square matrix.
   * WARNING:  This class inherits from std::vector, and doesn't currently mask
   * all of the operations of std::vector that can change the vector size.  The
   * behavior is therefore undefined if you change the size of the vector
   * without using upper_triangle::resize.  
   *
   * @tparam T
   *    The type to store in each cell of the matrix.
   *
   * @tparam SymmetricFixer
   *    The class to fix (or not) the order of the indices in the accessor
   *    functions.  The NullSymmetryFix will be ever slightly faster as it will
   *    be optimized away.  [Default:  NullSymmetryFix]
   *
   * @see NullSymmetryFix, SymmetryFix.
   */
  template < typename T, typename SymmetricFixer = NullSymmetryFix >
  class upper_triangle : public std::vector<T> {
    /* TYPEDEFS */
  private:
    typedef std::vector<T> super;

    /* MEMBER STORAGE */
  private:
    /** The side length of the n x n matrix, of which we store the upper
     * triangular portion. */
    int n;


    /* MEMBER FUNCTIONS */
  public:
    /** Default constructor accepts an optional size parameter (defaults: 0). */
    upper_triangle(const int & n = 0) {
      resize(n);
    }

    /** Return the edge size of the n x n matrix. */
    const int & edge_size() const { return n; }

    /** Resize so that the upper triangular portion of a n x n matrix can be
     * stored. */
    void resize(const int & n) {
      this->n = n;
      super::resize((n*(n+1))/2);
    }

    /** Calculate the index into the storage based on the row and column
     * indices.  If SymmetryFix is used, then the indices are guaranteed to be
     * in the right order. 
     */
    inline int indx(int i, int j) const {
      SymmetricFixer()(i,j);
      return i*n - (i*(i+1))/2 + j;
    }

    /** Non-const accessor function of the ith row and jth column. */
    T & operator()(const int & i, const int & j) {
      return this->operator[](indx(i,j));
    }

    /** Const accessor function of the ith row and jth column. */
    const T & operator()(const int & i, const int & j) const {
      return this->operator[](indx(i,j));
    }
  };

} /* namespace olson_tools */

#endif // olson_tools_upper_triangle_h
