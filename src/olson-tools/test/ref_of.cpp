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

#define BOOST_TEST_MODULE  ref_of

#include <olson-tools/ref_of.h>

#include <boost/test/unit_test.hpp>
#include <iostream>


BOOST_AUTO_TEST_SUITE( ref_of );

BOOST_AUTO_TEST_CASE( ref_deref ) {
  int a = 10;
  int & b = a;
  int & c = olson_tools::ref_of(a);
  int & d = olson_tools::ref_of(b);

  BOOST_CHECK_EQUAL( c, d );
}

BOOST_AUTO_TEST_CASE( pointer_deref ) {
  int a = 10;
  int * a_ptr = &a;
  int & b = olson_tools::ref_of(a_ptr);
  int & c = olson_tools::ref_of(&a);

  BOOST_CHECK_EQUAL( b, c );
}

BOOST_AUTO_TEST_SUITE_END();

