// -*- c++ -*-
// $Id: Generation.C,v 1.1 2005/06/14 02:28:18 olsonse Exp $
/*
 * Copyright 199?-1998 Robert Steven Turley --- All Rights Reserved
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: Generation.C,v $
 * Revision 1.1  2005/06/14 02:28:18  olsonse
 * Split out some files and renamed to be more consistent with olson-tools.
 *
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
 *
 * Revision 1.10  2000/10/25 18:15:54  olsons
 * Small fixes
 *
 * Revision 1.9  2000/06/15 19:31:00  olsons
 * Made debug_level act more like levels rather than codes.
 *
 * Revision 1.8  2000/05/06 20:37:18  olsons
 * Reorganized libfit: made it self-containing and use new definition of alleles.
 *
 * Revision 1.7  1999/10/18 18:56:34  olsons
 * Added documentation in comments to be parsed by doc++.
 *
 * Revision 1.6  1999/09/23 21:15:56  olsons
 * Just redoing constructors to use default values.  (not so much overloading)
 *
 * Revision 1.5  1999/09/09 15:53:21  olsons
 * added runtime debug level capability
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
 * Revision 1.2  1998/10/03 21:16:34  olsons
 * cvs fixes
 *
 * Revision 1.1.1.1  1998/10/03 16:07:52  olsons
 * Imported Genetic Algorithm
 *
 * Revision 1.8  1998/01/14  15:37:06  turley
 * added top oxide layers
 *
 * Revision 1.7  1997/12/29  16:50:34  turley
 * using angular averages and file io
 *
 * Revision 1.5  1997/12/19  22:10:35  turley
 * added true tournament selection
 *
 * Revision 1.4  1997/12/17  18:51:26  turley
 * Fixed bug, added histogram capability and resource competition
 *
 * Revision 1.3  1997/12/10  22:15:41  turley
 * bug fixes
 *
 * Revision 1.1  1997/12/02  19:56:36  turley
 * Initial revision
 *
*/


#include <olson-tools/fit/Generation.h>
#include <olson-tools/fit/debug.h>
#include <olson-tools/fit/Gene.h>

#ifdef USE_PTHREAD
#  include <olson-tools/fit/detail/EvalMeritFunctor.h>
#  include <olson-tools/fit/detail/LocalFitFunctor.h>
#  include <olson-tools/PThreadEval.h>
#endif


#include <olson-tools/random/random.h>
#include <olson-tools/ompexcept.h>

#include <stdexcept>
#include <set>

#include <cstdlib>
#include <cassert>


namespace olson_tools{
  namespace fit {

    /* *****       stopGeneticAlg portion ******* */

    ///
    template < typename optionsT >
    void Generation<optionsT>::setStop(int inSignal) {
      stop = true;//stop is static, so we can do this from any where
    }// GeneticAlg::stopGeneticAlg

    template < typename optionsT >
    bool Generation<optionsT>::stop = false;

    #ifdef USE_PTHREAD
    template < typename optionsT >
    olson_tools::PThreadCache
      Generation<optionsT>::thread_cache = olson_tools::PThreadCache();
    #endif

    /* *****   end    stopGeneticAlg portion ******* */

    ///
    Individual ** new_Individual_list(  const Gene & gn,
                                        long population,
                                        CREATE_IND_FUNC createind = NULL,
                                        void * meritfnc = NULL,
                                        void * obj_ptr = NULL );

    ///
    void delete_Individual_list(Individual ** ilist);

    ///
    template < typename optionsT >
    merit_t Generation<optionsT>::merit() {
      // return the average of the top 4 merit functions,
      // assuming the list has already been sorted
      return 0.25 * ( member[0]->Merit()+
                      member[1]->Merit()+
                      member[2]->Merit()+
                      member[3]->Merit() );
    } // Generation::merit

    template < typename optionsT >
    merit_t Generation<optionsT>::total() {
      // return the total of the merit functions
      merit_t sum=0;
      for(int i=0; i<options.population; ++i)
        sum += member[i]->Merit();
      return sum;
    } // Generation::total

    template < typename optionsT >
    const Gene & Generation<optionsT>::bestGene() const {
      // we should not allow others to change this guy's information
      return gene;
    } // Generation::bestGene

    template < typename optionsT >
    const merit_t & Generation<optionsT>::bestMerit() const {
      // we should not allow others to change this guy's information
      return bestmerit;
    } // Generation::bestMerit

    template < typename optionsT >
    void Generation<optionsT>::sort() {
      assert( options.population > 0 );

      unsigned int fit_max_individuals =
        int( options.population *
             std::max( 0.0f,
                       std::min(1.0f,options.local_fit_max_individuals_prctage)
             )
        );
      // Update merit functions, then sort list with best values first

      #ifdef USE_PTHREAD
      {
        /* We will evaluate Merit() for each individual in the population, each
         * evaluation in its own threads.  Doing so now will cache the result in
         * the individual.  
         */
        olson_tools::PThreadEval< detail::EvalMeritFunctor >
          evaluator(Generation::thread_cache);
        for ( int i = 0; i < options.population; ++i ) {
          evaluator.eval( detail::EvalMeritFunctor( *member[i] ) );
        }
        evaluator.joinAll();
      }
      #endif

      if ( options.local_fit_random ) {
        // use fitting algorithm to maximize merit
        std::set<int> fitted;

        while ( fitted.size() < fit_max_individuals ) {
          /* first choose which one to fit. */
          int i = 0;
          do {
            i = static_cast<int>(MTRNGrand() * options.population * .9999999);
          } while ( fitted.find(i) == fitted.end() );

          fitted.insert(i);

          /* now perform fit. */
          typename optionsT::LocalFit() ( member[i], options.localParam );
        }
      }

      /* encourage diversity if requested. */
      if (options.encourage_diversity) {
        Histogram * hist = new Histogram( options.population,
                                          options.diversity_grid_cols,
                                          *member[0] );

        for(int i=0; i<options.population; ++i) {
          Individual & ind = *member[i];

          hist->update(ind);
          /* make sure that we are doing the right thing for +- merits.
           * To do this we multiply merit by:  (1 +- ( 1 - hist->mf() ) )
           * where +sign is for merit< 0 and -sign is for merit> 0.
           */
          merit_t mf = 1 - ::copysign( 1 - hist->meritfact(ind), ind.Merit() );
          ind.multMerit(mf);
        } // for i

        delete hist;
      }

      /* Note that qsort will sort from least to greatest.  Since I want
       * from greatest to least, set comparison in mcomp so that it is
       * backwards.  Also, because member[i] is now a pointer to
       * an Individual, our qsort will only move around pointer
       * values (much faster).
       */
      qsort(member, options.population, sizeof(member[0]), mcomp);

      if ( !options.local_fit_random ) {
        /* This code should be better, I think, for applying the local fit.
         * I think that for the local fit, I should try one of two approaches:
         *   1.  apply the local fit to members randomly through the population until
         *       quota has been reached.
         *   2.  apply the local fit to only the best members.  Right now, I'm doing
         *       neither of these--I'm doing something like locally fitting some of
         *       those at the top and some randomly near the top and perhaps some
         *       even not near the top.    
         * There is only one problem that I can see by using this code:  The
         * histogramming stuff will not work quite as well as it will only encourage
         * diversity of the members BEFORE they are locally fitted.  */

        if (fit_max_individuals > 0) {
          #ifdef USE_PTHREAD
          {
            /* We will evaluate Merit() for each individual in the population, each
             * evaluation in its own threads.  Doing so now will cache the result in
             * the individual.  
             */
            using detail::LocalFitFunctor;
            using olson_tools::PThreadEval;

            typedef LocalFitFunctor< typename optionsT::LocalFit > Functor;
            PThreadEval<Functor> localFitter(Generation::thread_cache);
            for ( unsigned int i = 0; i < fit_max_individuals; ++i ) {
              Functor f( typename optionsT::LocalFit(),
                         options.localParam,
                         member[i] );
              localFitter.eval( f );
            }
            localFitter.joinAll();
          }
          #else
            for ( unsigned int i = 0; i < fit_max_individuals; ++i ) {
              typename optionsT::LocalFit() ( member[i], options.localParam );
            }
          #endif

          /* now resort the fitted individuals again. */
          qsort(member, fit_max_individuals, sizeof(member[0]), mcomp);
        }
      }/* if not local_fit_random */

      // now set gene of the generation to member[0].gene of the population
      gene = member[0]->DNA;
      bestmerit = member[0]->Merit();
    } // Generation::sort

    template < typename optionsT>
    Generation<optionsT>::Generation( const optionsT & options, const Gene & igene )
      : options(options), gene(igene), bestmerit(0) {
      member = new_Individual_list( gene, options.population, options.createind,
                                    options.meritfnc, options.exterior_pointer );
      assert(member); // member better be point to an Individual pointer

      #ifdef USE_PTHREAD
      /* we'll only set this option for non-copy constructors. */
      if ( options.num_pthreads > 0 )
        thread_cache.set_max_threads( options.num_pthreads );
      #endif
    } // Generation constructor

    template < typename optionsT>
    Generation<optionsT>::Generation(const Generation& gen)
      : options(gen.options), gene( gen.gene ), bestmerit( gen.bestmerit )  {
      //cast the gene[-1] to a function of the correct kind
      member = new_Individual_list( gene, options.population, options.createind );
      for( int i = 0; i < options.population; ++i )
        *member[i] = *gen.member[i];
    } // Generation copy constructor

    template < typename optionsT>
    Generation<optionsT>::Generation(const Generation& gen, int max_individuals)
      : options( gen.options), gene( gen.gene ), bestmerit( gen.bestmerit ) {
      //cast the gene[-1] to a function of the correct kind
      member = new_Individual_list( gene, options.population, options.createind );
      for( int i = 0; i < max_individuals; ++i )
        *member[i] = *gen.member[i];
    } // Generation partial copy constructor

    template < typename optionsT>
    void Generation<optionsT>::randinit(){//initialize the population with random genes
      for( int i = 0; i < options.population; ++i )
        member[i]->randinit();
    } // Generation::init

    template < typename optionsT>
    Generation<optionsT>::~Generation(){
      delete_Individual_list(member);
    } // Generation destructor

    template < typename optionsT>
    void Generation<optionsT>::proportional(){
      // Use a proportional strategy with a steady state model to replace this
      // generation by the next one 
      merit_t rtot=total();
      // fill the children array with the best parents of the preceding
      // generation by making a partial copy of this generation
      int nchild = int( options.replace * options.population ); // number of children to create
      int ichild = options.population - nchild; // this is child index in new generation
      Generation chld( *this, ichild ); // make partial copy
      Individual **parent = new_Individual_list( gene, 2 , options.createind );
      // now for the new children
      while( ichild < options.population && !stop ){
        // select parents
        int ip1 = tselect( rtot, -1 ); // first parent, all in list
        // make a copy so crossover and mutation don't change original
        *parent[0] = *member[ ip1 ];
        // remove merit of removed parent from total
        merit_t rtot2 = rtot - member[ ip1 ] -> Merit( );
        int ip2 = tselect( rtot2, ip1 );  // make second selection with
              // parent 1 removed
        // make a copy so crossover and mutation don't change original
        *parent[1] = *member[ ip2 ];

        // breed by doing crossover and mutation on parents
        crossover( parent[0], parent[1], options.crossprob );
        parent[0]->mutate( options.mutprob );
        parent[1]->mutate( options.mutprob );

        // Add these children to the population
        *chld.member[ ichild++ ] = *parent[0];
        if( ichild < options.population )
          *chld.member[ ichild++ ] = *parent[1];
      } // for the rest of the child list
      /* Replace previous generation with this one.
       * ichild should only at most be equal to population.
      */
    #ifdef DEBUG
      if ( ichild > options.population && debug_level == GA ) {
        gaerr << "Generation::proportional: ichild > population\n";
        pausel();
      } //if
    #endif
      for ( int i=0; i<ichild; ++i ) {
        *member[i] = *chld.member[i];
      } // for i
      delete_Individual_list(parent);
    } // Generation::proportional

    template < typename optionsT>
    void Generation<optionsT>::tournament() {
      /* Use a tournament strategy with a steady state model to replace this
       * generation by the next one 
       * fill the children array with the best parents of the preceding
       * generation by making a partial copy of this generation
       */
      int nchild = int(options.replace*options.population);// number of children to create
      int ichild=options.population-nchild; // this is the child index in the new generation
      Generation chld(*this,ichild);// make partial copy
      Individual **children = new_Individual_list( gene, 2, options.createind );

      // now for the new children
      while( ichild < options.population && !stop ) {
        // select parents by tournament
        int ip1 = static_cast<int>(MTRNGrand() * options.population * .9999999);
        int ip2=ip1;
        while(ip2 == ip1) {
          ip2 = static_cast<int>(MTRNGrand() * options.population * .9999999);
        } // while parents are identical

        // compare parents and keep the best one
        if (member[ip1]->Merit() < member[ip2]->Merit())
          ip1 = ip2; // second parent in tournament is better

        {// Now do it again for another parent
          ip2 = ip1;
          while(ip2==ip1)
            ip2 = static_cast<int>(MTRNGrand() * options.population * .9999999);

          int ip3 = ip2;
          while((ip3 == ip2) || (ip3 == ip1)){
            ip3 = static_cast<int>(MTRNGrand() * options.population * .9999999);
          } // while parents are identical

          // compare parents and keep the best one
          if (member[ip2]->Merit() < member[ip3]->Merit())
            ip2 = ip3; // second parent in tournament is better

        }// Okay, parents are selected (ip1 and ip2)
    
        // make a copy so crossover and mutation don't change original
        *children[0] = *member[ip1];
        *children[1] = *member[ip2];

        // breed by doing crossover and mutation on parents' DNA
        crossover( children[0], children[1], options.crossprob );
        children[0]->mutate( options.mutprob );
        children[1]->mutate( options.mutprob );

        // Add these children to the population
        *chld.member[ ichild++ ] = *children[0];
        if( ichild < options.population )
          *chld.member[ ichild++ ] = *children[1];

      } // for the rest of the child list

      /* Replace previous generation with this one.
       * ichild should only at most be equal to population.
      */

      #ifdef DEBUG
      if ( ichild > options.population && debug_level == GA ) {
        gaerr << "Generation::tournament: ichild > population\n";
        pausel();
      } //if
      #endif

      for ( int i=0; i<ichild; ++i ) {
        *member[i] = *chld.member[i];
      } // for i
      delete_Individual_list(children);
    } // Generation::tournament

    template < typename optionsT>
    int Generation<optionsT>::tselect(merit_t rtot, int skip) {
      // select a parent using tournament selection
      // rtot is the sum of the merit functions
      // if skip>=0, that parent will be skipped in the selection
      float p = MTRNGrand();
      merit_t ptot=0;
      int ipar=-1;
      if(skip){
        // I am not skipping the first element, begin there
        ipar = 0;
        ptot = member[0]->Merit()/rtot;
      } else {
        // I am skipping the first element, begin with the second
        ipar = 1;
        ptot = member[1]->Merit()/rtot;
      } // if skip!=0
      while(ptot<p && ipar<options.population){
        // scan the list until ptot exceeds p
        if(++ipar != skip) // don't do this for the skipped element
          ptot += member[ipar]->Merit()/rtot;
      } // while ptot<p
      if(ipar>=options.population)
        return options.population-1;
      else
        return ipar;
    } // Generation::tselect

    template < typename optionsT>
    int Generation<optionsT>::seed( const Gene & seedgene,
                                      float seed_fraction ) {
      // insure inclusion of a specific solution in the set
      if (seed_fraction > 1.0) {
        THROW(std::runtime_error,"Generation::seed:  seed fraction must be <= 1.0");
      }

      int numseed = int(seed_fraction * options.population);
      for(int i=0;i<numseed; ++i) {
        member[i]->regene(seedgene); 
      }//for
      return numseed;
    } // Generation::seed

    template < typename optionsT>
    void Generation<optionsT>::hist() {
      // print a histogram of the random selections
      Histogram hist(options.population, options.diversity_grid_cols, *member[0]);
      for(int i=0; i<options.population; ++i){
        hist.update(*member[i]);
      }
      gaout<<hist;
    } // Generation::hist

    ///
    Individual ** new_Individual_list(const Gene & gn, long population,
                                      CREATE_IND_FUNC createind /* = NULL */,
                                      void * meritfnc /* = NULL */,
                                      void * obj_ptr /* = NULL */ ) {
      /* With the following, we will create an array of pointers
       * each to its own new Individiual, except for the first pointer
       * in the array.  In ilist[0] we will store the value of population
       * after casting appropriately.  We will then return a pointer to
       * to the ilist[1] to the user.  We do this, so that, when deleting
       * the list, we can find the size of the population without
       * having it given explicitly to us again. (see delete_Individual_list)
       */
      if(!population)
        return NULL;//will do nothing for this case

      Individual **ilist = new Individual *[population+1];

      if( !createind )
        createind = create_Individual;

      for(long i = 1; i<(population+1);++i)
        ilist[i] = createind( gn, meritfnc, obj_ptr );

      ilist[0] = (Individual *)(population);
      return ++ilist;
    } // new_population_list

    void delete_Individual_list(Individual ** ilist) {
      --ilist;
      long population = (long)ilist[0];
      ++ilist;
      for(long i = 0; i<population; delete ilist[i++]);
      delete[] --ilist;
    } //delete_population_list

    template < typename T >
    std::ostream & operator<<(std::ostream &output, const Generation<T> & gen) {
      output<<"Top four Individuals in Generation\n";
      for(int i=0; i<4; ++i){
        //output.width(2);
        output<<"member["<<i<<"]: "
              <<*gen.member[i]
              << "\n\n" << std::flush;
      } // for first items in list
      return output;
    } // operator << Generation

  }/*namespace olson_tools::fit */
}/*namespace olson_tools */
