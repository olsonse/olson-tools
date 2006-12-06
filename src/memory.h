// -*- c++ -*-
// $Id: memory.h,v 1.3 2005/01/26 01:25:00 olsonse Exp $
/*
 * Copyright 1998-2004 Spencer Eugene Olson --- All Rights Reserved
 *
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
 * Redefinitions of new and delete operators for c++ for a specific class.
 * memory.h
 *
 * Written by Spencer Eugene Olson <seo3@email.byu.edu>,<olsonse@umich.edu>
 *
 * Copyright 1998-2004 Spencer Eugene Olson --- All Rights Reserved
 *
 * Because memory allocation can be very expensive time-wise, it is often
 * advisable to avoid extensive allocation/deallocation in your programs.  In spite of
 * this, it is also often very handy to be able to use the c++ new and delete
 * operators to manage memory usage.  This file provides a way to create a
 * cache of memory chunks so-as to avoid using the system alloc() and free()
 * functions.
 * 
 * */

#ifndef MEMORY_GOO
#define MEMORY_GOO

#include <stdlib.h>

#ifdef _OPENMP
#  include <omp.h>
#  define IFOMP(x) x
#else
#  define IFOMP(x)
#endif


/** MemGooLock is a class to facilitate mutual exlusion locks for
 * multi-threaded code.  This should be specialized (by ifdefs) for the
 * specific type of mutex needed.
 */
class MemGooLock {
  private:
    IFOMP(omp_lock_t omplock;)

  public:
    MemGooLock() {
        IFOMP(omp_init_lock(&omplock);)
    }

    ~MemGooLock() {
        IFOMP(omp_destroy_lock(&omplock);)
    }

    inline void lock() {
        IFOMP(omp_set_lock(&omplock);)
    }

    inline void unlock() {
        IFOMP(omp_unset_lock(&omplock);)
    }
};


/** Memory Storage Implementation.
 * This begins the overloading of the new and delete operators.  This must be
 * used inside the class declaration which matches the class parameter.
 *
 * The following are overloaded so that we can use a freestore
 * type list where unused already allocated individuals
 * can live to await the time when their use will be needed again
 *
 * @param class The name of the class for which the new and delete operators
 *     are being overloaded.
 *
 * @see REDEF_NEW_DEL_C_TEMPLATE.
 */
#define REDEF_NEW_DEL_H(class) \
  public: \
    inline void* operator new(size_t sz) { \
	/* get free node from freelist if any */ \
        memGooLock.lock(); \
	if (freelist) { \
	    /* cerr<< #class <<"::new: returning from free list\n"; */ \
	    class* p = freelist; \
	    freelist = freelist->_new_next; \
	    freelistsz--; \
            memGooLock.unlock(); \
	    /* cerr<<#class<<": freelistsz = "<<freelistsz<<endl; */ \
	    return p; \
	}   \
        memGooLock.unlock(); \
	/* call malloc() otherwise */ \
	/* cerr<< #class <<"::new: Allocating memory\n"; */ \
	return malloc(sz); \
    } \
 \
    inline void operator delete(void* vp) { \
	class* p = (class*)vp; \
        memGooLock.lock(); \
	if( freelistsz >= freelistlimit ) { \
	    free( p ); \
	}/* if */ \
	else { \
	    /* link freed node onto freelist */ \
	    /* cerr<< #class <<"::delete: linking to free list\n"; */ \
	    p->_new_next = freelist; \
	    freelist = p; \
	    freelistsz++; \
	    /* cerr<<#class<<": freelistsz = "<<freelistsz<<endl; */ \
	}/* else */ \
        memGooLock.unlock(); \
    } /* class::operator delete() */ \
 \
  /** frees freelist back to heap.  \
   * @param new_memlimit \
   *     Set freelistlimit (if ==-1, then memlimit \
   *     (below) is used). \
   */ \
  static void freetoheap( int new_memlimit = -1 ); \
  \
private: \
  static MemGooLock memGooLock; \
  static class *freelist; /* free-store for stuff already allocated */ \
  static int freelistsz; /* will store the size of the freelist */ \
  static int freelistlimit; /* to store limit for size of freelist */ \
  class *_new_next



/** Memory Storage Implementation.
 * This macro finishes the new/delete re-implementation by initializing
 * static class values as well as defining a way to really free all the
 * deleted classes to the heap.
 * This macro must be used ouside of the class declaration.
 *
 * @param template Any template declarations that are needed for the class.
 * @param class should be a class name.
 * @param memlimit is a default limit on the number of objects
 * of this class type to store on the heap.
*/
#define REDEF_NEW_DEL_C_TEMPLATE(template,class,memlimit) \
template class* class::freelist = 0; /*init to NULL */ \
template int class::freelistsz = 0; /* init to zero */ \
template int class::freelistlimit = memlimit; /* limit number of instances */ \
template MemGooLock class::memGooLock = MemGooLock(); \
 \
 \
/* The following function will delete \
 * the list of unused preallocated memory. \
*/ \
 \
template void class::freetoheap( int new_memlimit /* = -1 */) { \
  /* cerr<< #class <<"::freetoheap: This was called\n"; */ \
  memGooLock.lock(); \
  if(freelist) { \
    for(int i = 0;i<freelistsz;i++) { \
       class* tmp = freelist;freelist=freelist->_new_next; \
       free(tmp);tmp = NULL; \
    } \
    freelistsz = 0; \
  } \
  freelistlimit = ( new_memlimit < 0 ) ? memlimit : new_memlimit; \
  memGooLock.unlock(); \
} /* freetoheap */


/** Backwards compatible for those classes that don't use templates.
 * @see REDEF_NEW_DEL_C_TEMPLATE.
 */
#define REDEF_NEW_DEL_C(class,memlimit) REDEF_NEW_DEL_C_TEMPLATE(,class,memlimit)

#endif /* MEMORY_GOO */

