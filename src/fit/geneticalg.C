// $Id: geneticalg.C,v 1.1 2005/01/08 04:27:25 olsonse Exp $
/*
 * $Log: geneticalg.C,v $
 * Revision 1.1  2005/01/08 04:27:25  olsonse
 * Initial revision
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

#include "io.h"
#include "generation.h"
#include <math.h>
#include "geneticalg.h"
#include "random.h"
#include <signal.h> // This is so we can issue ^C to stop the ga in its tracks
#include <assert.h>

typedef void (*sighandler_t)(int);

GeneticAlg::GeneticAlg(Gene &gene, GeneticAlgArgs & ga_args ):
  args(ga_args),
  parents( ga_args.population, gene,
           ga_args.createind,
           ga_args.meritfnc,
           ga_args.exterior_pointer ),
  fitgene(&gene) {

  parents.replace = args.replace;
  parents.local_fit_max_individuals_prctage =
      args.local_fit_max_individuals_prctage;
  parents.local_fit_tolerance = args.local_fit_tolerance;
  parents.crossprob = args.crossprob;
  parents.mutprob = args.mutprob;

} // GeneticAlg constructor

merit_t GeneticAlg::fit( ostream * output = NULL, float tolerance = 0, merit_t maxmerit = -1e60 ) {
  if( output ) {
    assert(output);
    gaout = gaerr = *output;
  }
  if( ! tolerance ) {
    tolerance = args.tolerance;
    maxmerit = args.max_merit;
  }//if

  /* The following line reset the the 'memory buffer'
   * that will be kept for the Individual:: class.
   * We do this so that each allocation of a new generation
   * will be quicker.
   */
  Individual::freetoheap( args.population + 2 );
  initrand(args.randseed,args.randsize,args.generateseed);

  merit_t merit(0),lmerit(0);

  //set the SIGINT signal so we can stop
  sighandler_t old_signal_handler = signal(SIGINT, Generation::setStop);

  try {
    parents.stop = false; // reset the stop boolean
    parents.randinit(); // init with random population
    parents.sort();
    merit=parents.merit();
    lmerit=maxmerit;
    if(output) {
      (*output)<<"Generation 1\n";
      (*output)<<parents;
    }//if
    generation_iter=2;
    // Loop until merit function can be improved no further (less than tolerable)
    // or until the desired merit is reached
    while( ( fabs(merit-lmerit) >= tolerance ) && !parents.stop
          && ( merit<maxmerit ) && ( generation_iter < args.maxgeneration ) ){
      parents.tournament(); // provide darwinism and create new generation
      parents.sort();
      lmerit=merit;
      merit=parents.merit();
      if(output) {
       (*output)<<"Generation "<<generation_iter++<<endl;
       (*output)<<parents;
       (*output)<<"Average Merit of Top Four="<<merit<<endl<<endl;
      }//if
    } // while merit has decreased

    /* Now copy over the best gene in the population to the
     * gene that we are to return to the caller.
    */

    *fitgene = parents.bestgene();

  if(parents.stop)
    if(output) {
      (*output)<<"I was asked to stop; you only get what is best so far"<<endl;
    }//if

  }//try
  catch ( libfitError g ) {
    if(output) {
      (*output)<<"GenticAlg exception caught:\n"<< g.errcode <<endl;
    }//if
  }
  catch (...) {
    if(output) {
      (*output)<<"GenticAlg exception caught:\n"<< "Unknown error" <<endl;
    }//if
  }

  /* Now we take care of getting the system back to what it should be. */

  /* We'll return the memory buffer set up for the Individual class
   * and have the size of this buffer reset to its default value (0).
   */
  Individual::freetoheap();

  //Reset the signal so that it isn't passed to us.
  if( old_signal_handler == SIG_ERR ||
      old_signal_handler == Generation::setStop )
    signal(SIGINT, SIG_DFL);
  else signal(SIGINT, old_signal_handler);

  return merit;
} // ga

void GeneticAlgArgs::setParam (const string & name, double & var) {
	if (name == "population")
		population = (int)var;
	else if (name == "tolerance")
		tolerance = var;
	else if (name == "max_merit")
		max_merit = var;
	else if (name == "replace")
		replace = var;
	else if (name == "local_fit_max_individuals_prctage")
		local_fit_max_individuals_prctage = var;
	else if (name == "local_fit_tolerance")
		local_fit_tolerance = var;
	else if (name == "local_fit_max_iteration")
		local_fit_max_iteration = (int)var;
	else if (name == "crossprob")
		crossprob = var;
	else if (name == "mutprob")
		mutprob = var;
	else if (name == "maxgeneration")
		maxgeneration = (int)var;
	else if (name == "randseed")
		randseed = (unsigned int)var;
	else if (name == "randsize")
		randsize = (int)var;
	else if (name == "generateseed")
		generateseed = (bool)var;	
}

ostream & operator<<(ostream & output,const GeneticAlg & ga){
  output<<"Genetic Algorithm output:\n"
        <<"Generation "<<ga.generation_iter<<endl
        <<ga.parents<<endl;
  return output;
} // operator<< GeneticAlg

