// -*- c++ -*-
// $Id: memory.h,v 1.3 2005/01/26 01:25:00 olsonse Exp $
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

/*
 * $Log: memory.h,v $
 * Revision 1.3  2005/01/26 01:25:00  olsonse
 * Fixed possible scope issues with double use of 'i' in freetoheap(...).
 *
 * Revision 1.2  2005/01/14 14:09:31  olsonse
 * Fixed documentation on memory.h, msh.h, options.h.
 * Moved new files Distribution.[hC] listutil.h readData.h from dsmc code.
 *
 * Revision 1.1.1.1  2005/01/08 04:27:24  olsonse
 * Initial import
 *
 */

/** \file
 * Syncronization provided by .lock() and .unlock() function calls.
 *
 * Written by Spencer Eugene Olson <seo3@email.byu.edu>,<olsonse@umich.edu>
 *
 * Copyright 1998-2008 Spencer Eugene Olson --- All Rights Reserved
 *
 * */

#ifndef olson_tools_SyncLock_h
#define olson_tools_SyncLock_h

/* We'll only use pthread mutexes if pthread library is being used at all. */
#if defined(_OPENMP) && !defined(USE_PTHREAD)
#  include <omp.h>
#  define IF_OMP(x) x
#else
#  define IF_OMP(x)
#endif

#ifdef USE_PTHREAD
#  include <pthread.h>
#  include <errno.h>
#  define IF_PTHREAD(x) x
#else
#  define IF_PTHREAD(x)
#endif

#if defined(_OPENMP) || defined(USE_PTHREAD)
#  define IF_THREADS(x,y) x
#else
#  define IF_THREADS(x,y) y
#endif


namespace olson_tools {

  /** SyncLock is a class to facilitate mutual exlusion locks for
   * multi-threaded code.  This should be specialized (by ifdefs) for the
   * specific type of mutex needed.  Currently, ifdefs are provided for OpenMP
   * and PThreads.
   */
  class SyncLock {
  private:
    IF_OMP(omp_lock_t omplock;)
    IF_PTHREAD(pthread_mutex_t pthread_mutex;)

  public:
    SyncLock() {
      IF_OMP(omp_init_lock(&omplock);)
      IF_PTHREAD(pthread_mutex_init(&pthread_mutex, NULL);)
    }

    ~SyncLock() {
      IF_OMP(omp_destroy_lock(&omplock);)
      IF_PTHREAD(pthread_mutex_destroy(&pthread_mutex);)
    }

    inline void lock() {
      IF_OMP(omp_set_lock(&omplock);)
      IF_PTHREAD(pthread_mutex_lock(&pthread_mutex);)
    }

    inline bool isLocked() {
      IF_OMP(
        if ( omp_test_lock(&omplock) ) {
          unlock();
          return false;
        } else
          return true;
      )

      IF_PTHREAD(
        int v = pthread_mutex_trylock(&pthread_mutex);
        switch (v) {
          case EBUSY:
            return true;
          case 0:
            unlock();
          default:
            return false;
        }
      )

      IF_THREADS(/*threads active*/,return false;)
    }

    inline void unlock() {
      IF_OMP(omp_unset_lock(&omplock);)
      IF_PTHREAD(pthread_mutex_unlock(&pthread_mutex);)
    }
  };

  template < typename T >
  struct Syncronize {
    /* STATIC STORAGE */
    static SyncLock syncLock;

    /* MEMBER FUNCTIONS */
    /** Default constructor locks sync object. */
    Syncronize() {
      syncLock.lock();
    }

    /** Destructor releases lock. */
    ~Syncronize() {
      syncLock.unlock();
    }
  };

  template < typename T >
  SyncLock Syncronize<T>::syncLock = SyncLock();

  template < typename Functor >
  inline Functor synchronize( const Functor & f ) {
    Syncronize<Functor> sync;
    Functor fout(f);
    fout();
    return fout;
  }

} /*namespace olson_tools*/

#endif // olson_tools_SyncLock_h

