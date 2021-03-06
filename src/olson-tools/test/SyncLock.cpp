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

#define BOOST_TEST_MODULE  SyncLock

#include <olson-tools/SyncLock.h>

#include <boost/test/unit_test.hpp>

namespace {
  using olson_tools::Synchronize;
  using olson_tools::synchronize;
  using olson_tools::SyncLock;
  using olson_tools::SyncKey;

  struct AStruct {};

  struct Functor {
    int value;

    Functor() : value(0) { }

    void operator() () {
      value = 1;
      BOOST_CHECK_EQUAL( Synchronize<Functor>::lock.isLocked(),
                         IF_THREADS(true,false) );
    }
  };
}


BOOST_AUTO_TEST_SUITE( SyncLock_tests );

BOOST_AUTO_TEST_CASE( SyncLock_class ) {
  SyncLock lock;
  BOOST_CHECK_EQUAL( lock.isLocked(), false );

  lock.lock();
    BOOST_CHECK_EQUAL( lock.isLocked(), IF_THREADS(true,false) );
  lock.unlock();

  BOOST_CHECK_EQUAL( lock.isLocked(), false );

  lock.tryLock();
    BOOST_CHECK_EQUAL( lock.isLocked(), IF_THREADS(true,false) );
  lock.unlock();
}

BOOST_AUTO_TEST_CASE( SyncLock_class_RAII_Key ) {
  SyncLock lock;
  BOOST_CHECK_EQUAL( lock.isLocked(), false );
  {
    SyncKey key(lock);
    BOOST_CHECK_EQUAL( lock.isLocked(), IF_THREADS(true,false) );
  }

  BOOST_CHECK_EQUAL( lock.isLocked(), false );
}

BOOST_AUTO_TEST_CASE( Syncronize_RAII_class ) {
  {
    Synchronize<AStruct> sync;
    BOOST_CHECK_EQUAL( sync.lock.isLocked(), IF_THREADS(true,false) );
  }

  BOOST_CHECK_EQUAL( Synchronize<AStruct>::lock.isLocked(), false );
}

BOOST_AUTO_TEST_CASE( syncronize_Functor_access ) {
  BOOST_CHECK_EQUAL( Synchronize<Functor>::lock.isLocked(), false );
  Functor f = synchronize( Functor() );
  BOOST_CHECK_EQUAL( Synchronize<Functor>::lock.isLocked(), false );
  BOOST_CHECK_EQUAL( f.value, 1 );
}

BOOST_AUTO_TEST_SUITE_END();

