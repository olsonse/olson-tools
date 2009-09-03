// -*- c++ -*-
// $Id: GeneticAlg.C,v 1.3 2005/06/14 02:28:19 olsonse Exp $
/*
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: GeneticAlg.C,v $
 * Revision 1.3  2005/06/14 02:28:19  olsonse
 * Split out some files and renamed to be more consistent with olson-tools.
 *
 * Revision 1.2  2005/06/09 08:50:42  olsonse
 * Style changes mostly.  Bug fixes in use of Histogram to encourage diversity.
 *
 * Revision 1.1  2005/06/07 20:38:11  olsonse
 * Fixed the old genetic algorithm files.  They compile.  Hopefully they work.
 *
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
 *
 * Revision 1.9  2000/10/25 18:15:54  olsons
 * Small fixes
 *
 * Revision 1.8  2000/07/20 00:10:58  olsons
 * Changed debug scheme and default debug level to 0.
 * Added setParam function to be able to call from an interface.
 *
 * Revision 1.7  2000/06/15 19:31:00  olsons
 * Made debug_level act more like levels rather than codes.
 *
 * Revision 1.6  2000/05/06 20:37:18  olsons
 * Reorganized libfit: made it self-containing and use new definition of alleles.
 *
 * Revision 1.5  1999/10/18 18:56:34  olsons
 * Added documentation in comments to be parsed by doc++.
 *
 * Revision 1.4  1999/08/18 13:49:39  olsons
 * Fixed extra 'static class::freetoheap' declaration in memory.
 * Added an escape function to generation and geneticalg so
 * that the algorithm can be interrupted.
 *
 * Revision 1.3  1999/07/28 00:32:08  olsons
 * fixed and re-enabled class::freetoheap so that we can free the memory to the heap again.
 * found a bug and squashed it in simfit (it was in checking for minvals[] and maxvals[] -- my addition to simfit)
 *
 * Revision 1.2  1999/07/03 05:27:04  olsons
 * Genetic Algorithm is now a seperate entity from XUVMulti and is written so it
 * can be used for any purpose.
 * libgeneticalg.so was also added.
 *
 * Revision 1.1.1.1  1999/06/04 13:47:54  olsons
 * Imported as XUVMulti into cvs
 *
 *
 */

#include <olson-tools/fit/io.h>
#include <olson-tools/fit/Generation.h>
#include <olson-tools/fit/GeneticAlg.h>
#include <olson-tools/fit/Gene.h>
#include <olson-tools/fit/debug.h>

#include <olson-tools/random/random.h>
#include <olson-tools/ompexcept.h>

#include <stdexcept>
#include <ostream>
#include <limits>

#include <cassert>
#include <csignal> // This is so we can issue ^C to stop the ga in its tracks
#include <unistd.h>

namespace olson_tools {
  namespace fit {

    template < typename optionsT >
    GeneticAlg<optionsT>::GeneticAlg( Gene &gene, const optionsT & options ):
      options( options ),
      parents( options, gene ),
               fitgene( &gene ) { }

    template < typename optionsT >
    merit_t GeneticAlg<optionsT>::fit( std::ostream * output /* = NULL */,
                                       float tolerance /* = 0 */, 
                                       merit_t maxmerit /* = -INF */) {
      if( output ) {
        assert(output);
        gaout.tie(output);
        gaerr.tie(output);
      }

      if( ! tolerance )
        tolerance = options.tolerance;

      if ( maxmerit == -std::numeric_limits<merit_t>::infinity() )
        maxmerit = options.max_merit;

      /* The following line reset the the 'memory buffer'
       * that will be kept for the Individual:: class.
       * We do this so that each allocation of a new generation
       * will be quicker.
       */
      Individual::freetoheap( options.population + 2 );

      merit_t merit(0),lmerit(0);

      //set the SIGINT signal so we can stop
      typedef void (*SigHandler)(int);
      SigHandler old_signal_handler = signal(SIGINT, Generation<optionsT>::setStop);

      TRY {
          parents.stop = false; // reset the stop boolean
          parents.randinit(); // init with random population
          if (options.seed_fraction > 0) {
              /* seed a percentage of the parents with the given gene. */
              int seeded = parents.seed(*fitgene, options.seed_fraction);

              if (output) {
                  (*output) << "Number of parents seeded:  " << seeded << std::endl;
              }
          }
          parents.sort();
          merit=parents.merit();

          /* start with the running average of merit at the current merit -
           * divided. */
          lmerit=0.5*merit;

          if(output) {
              (*output)<<"Generation 1\n";
              (*output)<<parents;
          }//if
          generation_iter=2;
          // Loop until merit function can be improved no further (less than tolerable)
          // or until the desired merit is reached
          while(   ( fabs(merit-lmerit) >= tolerance )
                && !parents.stop
                && ( merit <= maxmerit )
                && ( generation_iter < options.maxgeneration ) ) {
              parents.tournament(); // provide darwinism and create new generation
              parents.sort();

              /* record the new exponential average of merit */
              lmerit *= 0.75;
              lmerit += 0.25 * merit;

              merit=parents.merit();
              if(output) {
                 (*output) << "Generation " << generation_iter++ << std::endl
                           << parents 
                           << "Average Merit of Top Four =" << merit << std::endl << std::endl 
                           << "Generation Merit Increase =" << (merit-lmerit) << std::endl << std::endl;
              }//if

              if (options.generation_delay < 0)
                pausel();
              else if (options.generation_delay > 0)
                usleep( static_cast<useconds_t>(options.generation_delay * 1e6) );
          } // while merit has decreased

          /* Now copy over the best gene in the population to the
           * gene that we are to return to the caller.
          */

          *fitgene = parents.bestgene();

          if(parents.stop && output) {
              (*output) << "I was asked to stop; you only get what is best so far"
                        << std::endl;
          }//if

      } CATCH( std::runtime_error & e,
          if(output) {
            (*output) << "GenticAlg exception caught:\n"
                      << e.what() << std::endl;
          }//if
      ) CATCH(...,
          if(output) {
              (*output)<<"GenticAlg exception caught:\n"<< "Unknown error" << std::endl;
          }//if
      )/* catch */

      /* Now we take care of getting the system back to what it should be. */

      /* We'll return the memory buffer set up for the Individual class
       * and have the size of this buffer reset to its default value (0).
       */
      Individual::freetoheap();

      //Reset the signal so that it isn't passed to us.
      if( old_signal_handler == SIG_ERR ||
          old_signal_handler == Generation<optionsT>::setStop )
          signal(SIGINT, SIG_DFL);
      else signal(SIGINT, old_signal_handler);

      return merit;
    } // ga

    template < typename T >
    std::ostream & operator<< ( std::ostream & output,
                                const GeneticAlg<T> & ga ) {
      output<< "Genetic Algorithm output:\n"
            << "Generation " << ga.generation_iter << '\n'
            << ga.parents << std::endl;
      return output;
    } // operator<< GeneticAlg

  }/*namespace olson_tools::fit */
}/*namespace olson_tools */
