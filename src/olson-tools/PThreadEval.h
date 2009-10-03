/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2006-2009 Spencer E. Olson
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

/** \file
 * A threaded merit evaluator--used if USE_PTHREAD is true.
 *
 * Copyright 2006-2009 Spencer Olson
 *
 */

#ifndef olson_tools_PthreadEval_h
#define olson_tools_PthreadEval_h

#include <olson-tools/PThreadCache.h>

namespace olson_tools {

  struct DefaultPThreadFunctor {
    template < typename Gatherer >
    void accept( Gatherer & gatherer ) const { }
  };

  /** Creates merit evaluation tasks that are then executed by
   * the default thread cache (or another if the option is given).
   */
  template < typename Functor >
  class PThreadEval {
    /* TYPEDEFS */
  private:
    typedef olson_tools::PThreadTaskSet PThreadTaskSet;
    typedef olson_tools::PThreadTask PThreadTask;

    /** Task for the PThread cache manager. */
    struct PThreadEvalTask : PThreadTask {
      /* MEMBER STORAGE */
      /** reference to the relevant child. */
      Functor f;


      /* MEMBER FUNCTIONS */
      /** Virtual NO-OP Destructor. */
      virtual ~PThreadEvalTask() {}

      /** Task constructor. */
      PThreadEvalTask( const Functor & f ) :
        PThreadTask(), f(f) { }

      /** Actually build the sub-tree. */
      virtual void exec() {
        f();
      }
    };

    /** Functor to sum up total leaves generated by a list of finished tasks. */
    template < typename Gatherer >
    struct deleteTasks : public std::unary_function<PThreadTask *, void> {
      /* MEMBER STORAGE */
      Gatherer & g;

      /* MEMBER FUNCTIONS */
      /** Constructor specifies a reference to the gatherer. */
      deleteTasks( Gatherer & g ) : g(g) { }

      /** Functor operation. */
      void operator() (PThreadTask * t) {
        /* Execute the user's gather code. */
        static_cast<PThreadEvalTask*>(t)->f.accept(g);

        /* we finally delete the actual memory of the task. */
        delete t;
      }
    };

    /** No-Op task gatherer. */
    struct NoOpGather {
      template < typename T >
      void update( const T & t ) { }
    };




    /* MEMBER STORAGE */
  private:
    /** The cache manager to use. */
    PThreadCache & cache;

    /** The tasks to evaluate. */
    PThreadTaskSet tasks;



    /* MEMBER FUNCTIONS */
  public:
    /** Constructor.
     * @param cache
     *    Specify the cache instance to use [default olson_tools::pthreadCache].
     */
    PThreadEval( PThreadCache & cache = olson_tools::pthreadCache )
      : cache(cache) { }

    /** Task scatterer.  */
    inline void eval( const Functor & f, bool self_if_none_avail = false ) {
      /* first wait for a thread to become available */
      PThreadTask * task = new PThreadEvalTask( f );
      tasks.insert(task);
      cache.addTask( task, self_if_none_avail );
    }

    /** Task gatherer.  This function makes sure that all of this
     * instance's tasks are completed before returning.
     */
    inline void joinAll() {
      joinAll( NoOpGather() );
    }

    /** Task gatherer.  This function makes sure that all of this
     * instance's tasks are completed before returning.
     */
    template < typename Gatherer >
    inline void joinAll( Gatherer & g ) {
      while (tasks.size() > 0) {
        PThreadTaskSet finished = cache.waitForTasks(tasks);

        /* Allow the user to do something with the task results. */
        for_each( finished.begin(), finished.end(), deleteTasks<Gatherer>(g) );

        /* Now make sure that we remove the finished tasks from the
         * tasks set.  We'll use the set operations to do this. */
        PThreadTaskSet tmp;
        std::set_difference( tasks.begin(), tasks.end(),
                             finished.begin(), finished.end(),
                             inserter(tmp, tmp.begin()) );
        tasks.swap(tmp);
      }
    }
  };

} /* namespace olson_tools */

#endif // olson_tools_PthreadEval_h