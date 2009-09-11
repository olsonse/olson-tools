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

#if defined(_OPENMP) && !defined(THREAD_SYS_DEFINED)
#  include <omp.h>
#  define IF_OMP(x) x
#  define THREAD_SYS_DEFINED
#else
#  define IF_OMP(x)
#endif

#if defined(USE_PTHREAD) && !defined(THREAD_SYS_DEFINED)
#  include <pthread.h>
#  include <errno.h>
#  define IF_PTHREAD(x) x
#  define THREAD_SYS_DEFINED
#else
#  define IF_PTHREAD(x)
#endif

#if defined(WIN32) && !defined(THREAD_SYS_DEFINED)
#  include <windows.h>
#  define IF_WIN32(x) x
#  define THREAD_SYS_DEFINED
#else
#  define IF_WIN32(x)
#endif


#if defined(THREAD_SYS_DEFINED)
#  define IF_THREADS(x,y) x
#else
#  define IF_THREADS(x,y) y
#endif


namespace olson_tools {

  /** SyncLock is a class to facilitate mutual exlusion locks for
   * multi-threaded code.  This should be specialized (by ifdefs) for the
   * specific type of mutex needed.
   *
   * Currently supported threading systems:
   * - OpenMP
   * - PThreads.
   * - Win32 (Critical Section stuff)
   * .
   *
   */
  class SyncLock {
  private:
    IF_OMP(omp_lock_t omplock;)
    IF_PTHREAD(pthread_mutex_t pthread_mutex;)
    IF_WIN32(CRITICAL_SECTION critical_section;)

  public:
    /** Constructor initializes relevant mutex object. */
    SyncLock() {
      IF_OMP(omp_init_lock(&omplock);)
      IF_PTHREAD(pthread_mutex_init(&pthread_mutex, NULL);)
      IF_WIN32(InitializeCriticalSection(&critical_section);)
    }

    /** Destructor destroys relevant mutex object. */
    ~SyncLock() {
      IF_OMP(omp_destroy_lock(&omplock);)
      IF_PTHREAD(pthread_mutex_destroy(&pthread_mutex);)
      IF_WIN32(DeleteCriticalSection(&critical_section);)
    }

    /** Locks relevant mutex object. */
    inline void lock() {
      IF_OMP(omp_set_lock(&omplock);)
      IF_PTHREAD(pthread_mutex_lock(&pthread_mutex);)
      IF_WIN32(EnterCriticalSection(&critical_section);)
    }

    /** Attempts to lock mutex, but does not block if unsuccessful.
     * @returns true if successful in locking the mutex.
     * @returns false if the mutex could not be locked.
     */
    inline bool tryLock() {
      IF_OMP(return omp_test_lock(&omplock);)
      IF_PTHREAD(return pthread_mutex_trylock(&pthread_mutex) == 0;)
      IF_WIN32(return TryEnterCriticalSection(&critical_section);)
      IF_THREADS(/*threads active*/,return true;)
    }

    /** Tests to see if relevant mutex object is locked. */
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

      IF_WIN32(
        if ( TryEnterCriticalSection(&critical_section) ) {
          unlock();
          return false;
        } else
          return true;
      )

      IF_THREADS(/*threads active*/,return false;)
    }

    /** Unlocks relevant mutex object. */
    inline void unlock() {
      IF_OMP(omp_unset_lock(&omplock);)
      IF_PTHREAD(pthread_mutex_unlock(&pthread_mutex);)
      IF_WIN32(LeaveCriticalSection(&critical_section);)
    }
  };

  /** An RAII type automatic locking/unlocking key for a SyncLock. 
   * The given SyncLock is locked upon construction, and automatically release
   * upon destruction.  
   */
  struct SyncKey {
    SyncLock & lock;

    SyncKey( SyncLock & lock ) : lock(lock) { lock.lock(); }
    ~SyncKey() { lock.unlock(); }
  };

  /** Automatic locking/unlocking facility with a typename scope. */
  template < typename T >
  struct Synchronize : SyncKey {
    /* STATIC STORAGE */
    static SyncLock lock;

    /* MEMBER FUNCTIONS */
    /** Default constructor locks sync object.
     * This uses the SyncKey on a static member.
     *
     * @see SyncKey
     */
    Synchronize() : SyncKey(Synchronize::lock) { }
  };

  template < typename T >
  SyncLock Synchronize<T>::lock = SyncLock();

  /** Synchronize the execution of a Functor class. */
  template < typename Functor >
  inline Functor synchronize( const Functor & f ) {
    Synchronize<Functor> sync;
    Functor fout(f);
    fout();
    return fout;
  }

} /*namespace olson_tools*/

#endif // olson_tools_SyncLock_h

