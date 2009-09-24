/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 1998-2008 Spencer Olson
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

#define BOOST_TEST_MODULE  Vector

#include <olson-tools/Vector.h>

#include <boost/test/unit_test.hpp>

namespace {
  using olson_tools::Vector;
  using olson_tools::V3;
  using olson_tools::make_vector;
}

BOOST_AUTO_TEST_SUITE( Vector_testsuite );

  BOOST_AUTO_TEST_CASE( index_operator ) {
    Vector<double,3> v;
    v[0] = 9;
    v[1] = 8;
    v[2] = 7;
    double * array = v.val;

    BOOST_CHECK_EQUAL( array[0], v[0] );
    BOOST_CHECK_EQUAL( array[1], v[1] );
    BOOST_CHECK_EQUAL( array[2], v[2] );
  }

  BOOST_AUTO_TEST_CASE( V3_double_fun ) {
    Vector<double,3> v = V3(11.42,12.42,13.42);
    BOOST_CHECK_EQUAL( v[0], 11.42 );
    BOOST_CHECK_EQUAL( v[1], 12.42 );
    BOOST_CHECK_EQUAL( v[2], 13.42 );
  }

  BOOST_AUTO_TEST_CASE( V3_int_fun ) {
    Vector<int,3> v = V3(-11,-12,-13);
    BOOST_CHECK_EQUAL( v[0], -11 );
    BOOST_CHECK_EQUAL( v[1], -12 );
    BOOST_CHECK_EQUAL( v[2], -13 );
  }

  BOOST_AUTO_TEST_CASE( V3_uint_fun ) {
    Vector<unsigned int,3> v = V3(11u,12u,13u);
    BOOST_CHECK_EQUAL( v[0], 11u );
    BOOST_CHECK_EQUAL( v[1], 12u );
    BOOST_CHECK_EQUAL( v[2], 13u );
  }

  BOOST_AUTO_TEST_CASE( V3_bool_fun ) {
    Vector<bool,3> v = V3(true,false,true);
    BOOST_CHECK_EQUAL( v[0], true  );
    BOOST_CHECK_EQUAL( v[1], false );
    BOOST_CHECK_EQUAL( v[2], true  );
  }

  template < typename T >
  T fun( const Vector<T,3u> & v = make_vector<T,3u>()(0, 1, 0) ) {
    return v[0];
  }

  bool fun_bool( const Vector<bool,3u> & v = V3(false, false, true) ) {
    return v[0];
  }

  double fun_double( const Vector<double,3u> & v = V3(42.,2.,3.) ) {
    return v[0];
  }

  BOOST_AUTO_TEST_CASE( function_default_args ) {
    BOOST_CHECK_EQUAL( fun<double>(), 0 );
    BOOST_CHECK_EQUAL( fun<bool>(), false );
    BOOST_CHECK_EQUAL( fun_bool(), false );
    BOOST_CHECK_EQUAL( fun_double(), 42 );
  }

  BOOST_AUTO_TEST_CASE( V2_fun ) {
    Vector<double,2> v = make_vector<double,2u>()(1., 2.);
    BOOST_CHECK_EQUAL( v[0], 1. );
    BOOST_CHECK_EQUAL( v[1], 2. );
  }

  BOOST_AUTO_TEST_CASE( V_mult ) {
    Vector<double,3> v1 = V3(1, 2, 3),
                     v2 = V3(9, 8, 7);
    Vector<bool,3>   v3 = V3(true, false, true);
    Vector<unsigned int,3> v4 = V3(10u, 20u, 30u);
    Vector<int,3>    v5 = V3(1, 2, 3),
                     v6 = V3(-1, -2, -3);

    BOOST_CHECK_EQUAL( v1 * v2, 46. );
    BOOST_CHECK_EQUAL( v1 * 1., v1 );
    BOOST_CHECK_EQUAL( 1. * v1, v1 );
    BOOST_CHECK_EQUAL( v1*v3, 4. );
    BOOST_CHECK_EQUAL( v3*v1, true );
    BOOST_CHECK_EQUAL( v1*v4/3, 46.66666666666666666666 );
    BOOST_CHECK_EQUAL( v4*v1/3, 46u );
    BOOST_CHECK_EQUAL( v4*v5/3, 46u );
    BOOST_CHECK_EQUAL( v5*v4/3, 46u );
    BOOST_CHECK_EQUAL( v4*v6, static_cast<unsigned int>(-140u) );
    BOOST_CHECK_EQUAL( v6*v4, -140 );
  }

  BOOST_AUTO_TEST_CASE( V_lt ) {
    Vector<double,3> v1 = V3(1, 2, 3),
                     v2 = V3(9, 8, 7),
                     v3 = V3(9, 8, 7);
    BOOST_CHECK_EQUAL( v1 <  v2, true );
    BOOST_CHECK_EQUAL( v1 <= v2, true );
    BOOST_CHECK_EQUAL( v2 <= v3, true );
    BOOST_CHECK_EQUAL( v2 <  v1, false );
    BOOST_CHECK_EQUAL( v2 <= v1, false );
    BOOST_CHECK_EQUAL( v1 <  1., false );
  }

  BOOST_AUTO_TEST_CASE( V_gt ) {
    Vector<double,3> v1 = V3(1, 2, 3),
                     v2 = V3(9, 8, 7),
                     v3 = V3(9, 8, 7);
    BOOST_CHECK_EQUAL( v1 >  v2, false );
    BOOST_CHECK_EQUAL( v1 >= v2, false );
    BOOST_CHECK_EQUAL( v2 >= v3, true );
    BOOST_CHECK_EQUAL( v2 >  v1, true );
    BOOST_CHECK_EQUAL( v2 >= v1, true );
    BOOST_CHECK_EQUAL( v1 >  1., false );
  }

  BOOST_AUTO_TEST_CASE( V_eq ) {
    Vector<double,3> v1 = V3(1, 2, 3),
                     v2 = V3(9, 8, 7),
                     v3 = V3(9, 8, 7);
    BOOST_CHECK_EQUAL( v1 == v2, false );
    BOOST_CHECK_EQUAL( v2 == v1, false );
    BOOST_CHECK_EQUAL( v2 == v3, true );
  }


BOOST_AUTO_TEST_SUITE_END();

