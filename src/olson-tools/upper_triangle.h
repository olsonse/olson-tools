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

#ifndef OLSON_TOOLS_UPPER_TRIANGLE_H
#define OLSON_TOOLS_UPPER_TRIANGLE_H

#include <vector>

namespace olson_tools {

struct SymmetryFix {
    void operator()(int & i, int & j) {
        if (j < i)
            std::swap(i,j);
    }
};

struct NullSymmetryFix {
    void operator()(int & i, int & j) {}
};

template <class T, class SymmetricFixer = NullSymmetryFix>
struct upper_triangle : std::vector<T> {
    typedef std::vector<T> super;

    upper_triangle(const int & n = 0) {
        resize(n);
    }

    void resize(const int & n) {
        this->n = n;
        super::resize((n*(n+1))/2);
    }

    inline int indx(int i, int j) const {
        SymmetricFixer()(i,j);
        return i*n - (i*(i+1))/2 + j;
    }

    T & operator()(const int & i, const int & j) {
        return this->operator[](indx(i,j));
    }

    const T & operator()(const int & i, const int & j) const {
        return this->operator[](indx(i,j));
    }
  private:
    int n;
};

} /* namespace olson_tools */

#endif // OLSON_TOOLS_UPPER_TRIANGLE_H
