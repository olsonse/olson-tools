// -*- c++ -*-
// $Id$
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

#ifndef olson_tools_Timer_h
#define olson_tools_Timer_h

#include <string>
#include <iostream>
#include <cassert>
#include <cstring>
#include <ctime>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>


namespace olson_tools {

  /** A simple timer class. 
   * This class tracks both wall clock time as well as cpu time (as reported by
   * the times() function.   This is a simple class that collects code that I
   * have been using in multiple locations for a long time.  */
  class Timer {
  public:
    /* TYPEDEFS */
    enum FUNCTION {
      AVERAGED, /* Tracks the averaged time between all pairs of start()/stop(). */
      CUMMULATIVE, /* Tracks the total time between all pairs of start()/stop(). */
      SIMPLE /* Only tracks the time between start() and stop(). */
    };


    /** Fraction/Number of seconds per each clock tick as reported by
     * sysconf(_SC_CLK_TCK).
     * This value is stored in a simple templated class so that we don't have to
     * have a .cpp file to initialize it. */
    template < unsigned int i = 0 >
    struct seconds_per_clock_tick {
      static const double value;
    };



    /* MEMBER STORAGE */
  private:
    /** Total number of calls to start(). */
    int N_start;

    /** Total number of calls to stop(). */
    int N_stop;

    /** Buffers for wall-clock-time measurements. */
    struct timeval tv[2];

    /** Buffers for cpu_time measurements. */
    struct tms ti, tf;

  public:
    /** Type of timer that this is. */
    enum FUNCTION function;

    /** The final result of the wall-clock-time measurement. */
    double dt;

    /** The final result of the cpu-time measurement. */
    double dt_cpu_time;

    /** Label for the wall time. */
    std::string wall_time_label;

    /** Label for the cpu time. */
    std::string cpu_time_label;


    /* MEMBER FUNCTIONS */
    /** Constructor defaults to a clean (zeroed) SIMPLE timer. */
    Timer(const enum FUNCTION & f = SIMPLE) { zero(); function = f; }

    /** Zero all timing buffers. This is not really necessary to do for SIMPLE
     * timers. */
    void zero() {
      std::memset(tv,0,2*sizeof(struct timeval));
      std::memset(&ti,0,sizeof(struct tms));
      std::memset(&tf,0,sizeof(struct tms));
      dt = 0;
      dt_cpu_time = 0;
      N_start = N_stop = 0;
    }

    /** Start the timer.  This essentially records the current time of day and
     * the current cpu usage time. */
    inline void start() {
      ++N_start;
      times(&ti);
      gettimeofday(&tv[0],NULL);
    }

    /** Stop the timer.  This records the current time of day and the current
     * cpu usage time.  the calcualte() function is then called which
     * subtracts the stored value from start() from the results stored in this
     * function. The final results of the timer are placed in dt and
     * dt_cpu_time by calculate(). */
    inline void stop() {
      gettimeofday(&tv[1],NULL);
      times(&tf);
      ++N_stop;
      calculate();
    }

    /** Time the execution of a functor object.
     * @param fun
     *    Input copy of Functor object.  Functor::operator() will be timed.
     * @returns
     *    Copy of the post-execution Functor object.
     */
    template < typename Functor >
    inline Functor run(Functor fun) {
      start();
      fun();
      stop();
      return fun;
    }

    /** Utility function to help subtract the total time in struct timeval.
     * Note that this function modifies tf.
     * @param ti
     *    Start-time structure.
     * @param tf
     *    End-time structure.
     * @param result
     *    The resulting value of the subtraction.
     */
    static void subtime( struct timeval &ti,
                         struct timeval &tf,
                         struct timeval &result ) {
      if (tf.tv_usec < ti.tv_usec) {
        tf.tv_usec += 1000000;
        tf.tv_sec--;
      }
      result.tv_sec = tf.tv_sec - ti.tv_sec;
      result.tv_usec = tf.tv_usec - ti.tv_usec;
    }

  private:
    /** Subtracts the stored values from start() from the stored values from
     * stop(). The final results of the timer are placed in dt and
     * dt_cpu_time. */ 
    void calculate() {
      struct timeval dtv;
      Timer::subtime(tv[0],tv[1],dtv);

      double a_dt = dtv.tv_sec + double(dtv.tv_usec)*1e-6L;
      double a_dt_cpu_time = 
           ( ( (tf.tms_utime + tf.tms_stime) - (ti.tms_utime + ti.tms_stime) )
             * seconds_per_clock_tick<>::value
           );

      assert( N_start == N_stop );
      switch (function) {
        case AVERAGED:
          dt += (a_dt - dt)/N_start;
          dt_cpu_time += (a_dt_cpu_time - dt_cpu_time)/N_start;
          break;

        case CUMMULATIVE:
          dt += a_dt;
          dt_cpu_time += a_dt_cpu_time;
          break;

        case SIMPLE:
        default:
          dt = a_dt;
          dt_cpu_time = a_dt_cpu_time;
          break;
      }
    }
  };

  /** Insertion operator for Timers to ease writing out to stream. */
  inline std::ostream & operator<< (std::ostream & out, const Timer & t ) {
    out << t.dt
              << (t.wall_time_label.size() > 0 ? " ":"")
              <<  t.wall_time_label
        << '\t'
        << t.dt_cpu_time
              << (t.cpu_time_label.size() > 0 ? " ":"")
              <<  t.cpu_time_label;
    return out;
  }

  template < unsigned int i >
  const double Timer::seconds_per_clock_tick<i>::value
    = 1.0 / sysconf(_SC_CLK_TCK);

}/* namespace olson_tools */

#endif // olson_tools_Timer_h
