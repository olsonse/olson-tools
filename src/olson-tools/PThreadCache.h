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
 * Implementation of a generally useful PThreads cache and associated task
 * manager.
 *
 * Copyright 2006-2009 Spencer Olson
 *
 * */

#ifndef olson_tools_PThreadCache_h
#define olson_tools_PThreadCache_h



#include <pthread.h>
#include <set>
#include <queue>
#include <algorithm>

#include <cstdio>

namespace olson_tools {

  /** The basic pthread task structure.  Inheriting classes must implement the
   * exec() function. */
  class PThreadTask {
    public:
      PThreadTask() {}
      virtual ~PThreadTask() {}
      virtual void exec() = 0;
  };

  /** A set of pointers to tasks. */
  typedef std::set<PThreadTask *> PThreadTaskSet;

  /** A PThreads threads cache and associated tasks manager. */
  class PThreadCache {
  private:
    int max_threads;    /* protected by max_threads_spinlock */
    int active_threads; /* protected by max_threads_spinlock (yes, same one) */

    pthread_attr_t  pthread_attr;
    pthread_mutex_t task_queue_mutex; /* mutex to protect the queue items. */
    pthread_cond_t  task_ready_cond;  /* condition for task queue additions. */
    pthread_cond_t  task_finished_cond;
    pthread_mutex_t task_finished_mutex;/* mutex to protect the finished set. */
    pthread_spinlock_t max_threads_spinlock;/* mutex to protect max_threads. */

    pthread_t * threads; /* do we actually need to keep thread ids? */


    std::queue<PThreadTask *> task_queue; /* a linked list of tasks to do. */
    PThreadTaskSet finished_tasks; /* a linked list of finished tasks. */

    bool slavesQuit;

    inline void signalSlavesQuit() {
      /* We'll use the task_queue_mutex, just because each slave will exit the
       * cond_wait() holding this mutex.  So, if we already hold it,
       * they can't read the quit variable. */
      pthread_mutex_lock(&task_queue_mutex);

      slavesQuit = true;
      /* now send the message to all slaves. */
      pthread_cond_broadcast(&task_ready_cond);

      pthread_mutex_unlock(&task_queue_mutex);
    }

    inline void resetSlavesQuit() {
      pthread_mutex_lock(&task_queue_mutex);
      slavesQuit = false;
      pthread_mutex_unlock(&task_queue_mutex);
    }


    inline PThreadTask * getTask() {
      PThreadTask * task = NULL;
      pthread_mutex_lock(&task_queue_mutex);

      while (!slavesQuit) {
        task = task_queue.empty() ? NULL : task_queue.front();
        if (task == NULL){
          pthread_cond_wait(&task_ready_cond, &task_queue_mutex);
        } else {
          task_queue.pop();
          break;
        }
      }

      pthread_mutex_unlock(&task_queue_mutex);
      return task;
    }

    /** Add this task to the finished tasks vector and signal the waiting
     * task owners that a task has finished. */
    inline void signalTaskFinished(PThreadTask * task) {
      /* add the task to the finished tasks. */
      pthread_mutex_lock(&task_finished_mutex);

      finished_tasks.insert(task);
      pthread_cond_broadcast(&task_finished_cond);

      pthread_mutex_unlock(&task_finished_mutex);
    }

    /** Executes all tasks placed in the task queue. */
    static void taskSlave(PThreadCache * cache) {
      /* check queue, if we get NULL back, that means that we were
       * requested to terminate. */
      PThreadTask * task;
      while ((task = cache->getTask()) != NULL) {
        cache->incrementActiveThreads(); /* inc active    */
        task->exec();                    /* execute task. */
        cache->decrementActiveThreads(); /* dec active    */
        cache->signalTaskFinished(task); /* signal finish */
      }
    }

    /** Increment the active threads counter--this should only ever be called by
     * the taskSlave function. */
    void incrementActiveThreads() {
      pthread_spin_lock(&max_threads_spinlock);
      ++active_threads;
      pthread_spin_unlock(&max_threads_spinlock);
    }

    /** Decrement the active threads counter--this should only ever be called by
     * the taskSlave function. */
    void decrementActiveThreads() {
      pthread_spin_lock(&max_threads_spinlock);
      --active_threads;
      pthread_spin_unlock(&max_threads_spinlock);
    }





  public:
    /** Constructor.
     * Sets the initial value of max_threads from the environment variable
     * NUM_PTHREADS.  If this variable is not set, then the default will be to
     * NOT create any threads for executing tasks given this this cache
     * manager.
     */
    PThreadCache() : max_threads(0),
                     active_threads(0),
                     threads(NULL),
                     task_queue(),
                     finished_tasks() {
      pthread_attr_init(&pthread_attr);
      pthread_mutex_init(&task_queue_mutex, NULL);
      pthread_mutex_init(&task_finished_mutex,NULL);
      pthread_spin_init(&max_threads_spinlock,0);
      pthread_cond_init(&task_ready_cond, NULL);
      pthread_cond_init(&task_finished_cond, NULL);

      slavesQuit = false;

      char * max_thread_str = getenv("NUM_PTHREADS");
      int mxth = 1;

      if ( max_thread_str == NULL ||
         sscanf(max_thread_str, "%d", &mxth) != 1 ) {

        /* we'll default to one thread only */
        mxth = 1;
      }

      /* call this function to set up each of the threads. */
      set_max_threads(mxth);
    }

    /** Destructor. */
    ~PThreadCache() {
      /* make sure that all threads are dead. */
      set_max_threads(0);
      pthread_attr_destroy(&pthread_attr);
      pthread_mutex_destroy(&task_queue_mutex);
      pthread_mutex_destroy(&task_finished_mutex);
      pthread_spin_destroy(&max_threads_spinlock);
      pthread_cond_destroy(&task_ready_cond);
      pthread_cond_destroy(&task_finished_cond);
    }

    /** Add a task to the thread cache task queue.
     * @param task
     *   The task to add the the task queue.
     * @param self_if_none_avail
     *   Whether to perform the work by self if no threads are currently
     *   available [Default false].
     */
    void addTask( PThreadTask * task, bool self_if_none_avail = false ) {
      pthread_spin_lock(&max_threads_spinlock);
        bool serial = max_threads <= 1 ||
                      ( self_if_none_avail && 
                        ( max_threads - active_threads ) == 0 );

        if ( !serial ) {
          pthread_mutex_lock(&task_queue_mutex);      /* locked queue */

          task_queue.push(task);
          /* now we should probably signal some thread that there is a task
           * ready for execution. */
          pthread_cond_signal(&task_ready_cond);

          pthread_mutex_unlock(&task_queue_mutex);    /* unlocked queue */
        }
      pthread_spin_unlock(&max_threads_spinlock);

      if ( serial ) {
        task->exec();
        signalTaskFinished(task);
      }
    }

    /** This function waits for some tasks to finish, if there are any
     * executing, and then returns the particular task(s) that finished.
     *
     * NOTE:  If the tasks in callers_tasks are not actually executing or
     * queued to execute, then this function could cause indefinite deadlock.
     */ 
    inline PThreadTaskSet waitForTasks(const PThreadTaskSet & callers_tasks) {
      /* Only return once one of the callers tasks have actually completed. */
      PThreadTaskSet retval;

      pthread_mutex_lock(&task_finished_mutex);

      /* The order of the following is to avoid race condtions with the
       * pthread_cond_broadcast/wait commands. */
      do {
        /* Find all finished tasks that the caller owns. */
        std::set_intersection(callers_tasks.begin(), callers_tasks.end(),
                              finished_tasks.begin(), finished_tasks.end(),
                              inserter(retval, retval.begin()));

        if (retval.size() != 0) break;

        pthread_cond_wait(&task_finished_cond, &task_finished_mutex);
      } while (true);

      /* We have successfully finished one of the callers tasks.  
       * Now make sure that we remove the finished tasks from the
       * finished_tasks set.  We'll use the set operations to do this. */
      PThreadTaskSet tmp;
      std::set_difference(finished_tasks.begin(), finished_tasks.end(),
                          callers_tasks.begin(), callers_tasks.end(),
                          inserter(tmp, tmp.begin()));
      finished_tasks.swap(tmp);

      pthread_mutex_unlock(&task_finished_mutex);

      return retval;
    }

    /** Change/Set the number of threads used to execute tasks. */
    inline int set_max_threads(int mx) {
      pthread_spin_lock(&max_threads_spinlock);

      if( max_threads != mx ) {
        if(max_threads > 1) {
          /* we previously set up threads, so lets tell them to stop. */
          signalSlavesQuit();

          /* join each of the threads. */
          for (int i = 0; i < max_threads; i++) {
            pthread_join(threads[i],NULL);
          }

          resetSlavesQuit();

          /* free up the old list of thread ids. */
          delete[]threads;
          threads = NULL;
        }/* if we have to join some threads */

        max_threads = mx > 1 ? mx : 1;

        /* only create new threads IF more than one are requested. 
         * If there is only one thread, the tasks will be executed serially at
         * the point of addTask(). */
        if(max_threads > 1) {
          /* we are instructed to prepare for threaded processing. */
          threads = new pthread_t[max_threads];

          for (int i = 0; i < max_threads; i++) {
            pthread_create(&threads[i], &pthread_attr, (void*(*)(void*))taskSlave, this);
          }
        }/* if more than one thread requested */
      }/* if the request is a change */

      mx = max_threads;
      pthread_spin_unlock(&max_threads_spinlock);
      return mx;
    }

    /** Get the maximum number of threads that will be used to execute tasks.
     * */
    inline int get_max_threads() {
      pthread_spin_lock(&max_threads_spinlock);
      int mx = max_threads;
      pthread_spin_unlock(&max_threads_spinlock);
      return mx;
    }

    /** Get both maximum number of threads that will be used to execute tasks as
     * well as the number of currently active threads.
     * */
    inline std::pair<int,int> get_active_threads() {
      pthread_spin_lock(&max_threads_spinlock);
      std::pair<int,int> retval( max_threads, active_threads );
      pthread_spin_unlock(&max_threads_spinlock);
      return retval;
    }
  };

  /** The default thread cache. */
  static PThreadCache pthreadCache;

  /** A function to satisfy OpenMP/PThread mixed code. */
  static inline int get_max_threads() {
      return pthreadCache.get_max_threads();
  }

}/* namespace olson_tools */

#endif // olson_tools_PThreadCache_h
