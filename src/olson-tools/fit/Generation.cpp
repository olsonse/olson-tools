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

#include <olson-tools/random/random.h>
#include <olson-tools/ompexcept.h>

#include <stdexcept>

#include <cstdlib>
#include <cassert>


namespace olson_tools{ namespace fit {

/* *****       stopGeneticAlg portion ******* */

///
void Generation::setStop(int inSignal) {
  stop = true;//stop is static, so we can do this from any where
}// GeneticAlg::stopGeneticAlg

bool Generation::stop = false;

/* *****   end    stopGeneticAlg portion ******* */

///
Individual ** new_Individual_list(const Gene & gn,
                                  long population,
                                  CREATE_IND_FUNC createind = NULL,
                                  void * meritfnc = NULL,
                                  void * obj_ptr = NULL );

///
void delete_Individual_list(Individual ** ilist);

///
merit_t Generation::merit(){
    // return the average of the top 4 merit functions,
    // assuming the list has already been sorted
    return ( member[0]->Merit()+
             member[1]->Merit()+
             member[2]->Merit()+
             member[3]->Merit() ) /4;
} // Generation::merit

merit_t Generation::total(){
  // return the total of the merit functions
  merit_t sum=0;
  for(int i=0; i<population; i++)
    sum += member[i]->Merit();
  return sum;
} // Generation::total

const Gene & Generation::bestgene() const {
  // we should not allow others to change this guy's information
  return gene;
} // Generation::bestgene

void Generation::sort(){
    int fit_max_individuals = int(population*local_fit_max_individuals_prctage);
    // Update merit functions, then sort list with best values first

    Histogram * hist = NULL;

    if (do_resource_competition) {
        hist = new Histogram(population, hist_cont_grid_cols, *member[0]);
    }

    for(int i=0; i<population; i++){
#ifdef HYBRID
        if(i<fit_max_individuals){
            // use fitting algorithm to maximize merit
            simplex_fit( member[i], local_fit_max_iteration, local_fit_tolerance);
        }
#endif
        if (do_resource_competition) {
            hist->update(*member[i]);
            merit_t mf = hist->meritfact(*member[i]);
            member[i]->multMerit(mf);
        }
    } // for i

    if (do_resource_competition) {
        delete hist;
    }

    /* Note that qsort will sort from least to greatest.  Since I want
     * from greatest to least, set comparison in mcomp so that it is
     * backwards.  Also, because member[i] is now a pointer to
     * an Individual, our qsort will only move around pointer
     * values (much faster).
     */
    qsort(member, population, sizeof(member[0]), mcomp);
    // now set gene of the generation to member[0].gene of the population
    gene = member[0]->DNA;
} // Generation::sort

Generation::Generation(int pop, const Gene & igene,
                       CREATE_IND_FUNC createind_fnc /* = NULL */,
                       void * meritfnc /* = NULL */,
                       void * obj_ptr /* = NULL */ ):
  population(pop),gene(igene), createind( createind_fnc ) {
  // using default values for the time being
  replace = 0.5;
  crossprob = 0.75;
  mutprob = 0.05;

  //Histogram defaults
  hist_cont_grid_cols = 100;
  do_resource_competition = false;

  // fitting algorithm defaults
  local_fit_max_individuals_prctage = .01;
  local_fit_max_iteration = 100;
  local_fit_tolerance = 0.01;
  member = new_Individual_list( gene, population, createind,
                                meritfnc, obj_ptr );
  assert(member); // member better be point to an Individual pointer
} // Generation constructor

Generation::Generation(const Generation& gen):
  local_fit_max_individuals_prctage(gen.local_fit_max_individuals_prctage),
  replace(gen.replace), crossprob(gen.crossprob),
  mutprob(gen.mutprob),local_fit_max_iteration(gen.local_fit_max_iteration),
  local_fit_tolerance(gen.local_fit_tolerance),
  do_resource_competition(gen.do_resource_competition),
  hist_cont_grid_cols(gen.hist_cont_grid_cols),
  population(gen.population), gene(gen.gene),
  createind( gen.createind ) {
  //cast the gene[-1] to a function of the correct kind
  member = new_Individual_list( gene, population, createind );
  for(int i=0; i<population;i++) *member[i] = *gen.member[i];
} // Generation copy constructor

Generation::Generation(const Generation& gen, int max_individuals):
  local_fit_max_individuals_prctage(gen.local_fit_max_individuals_prctage),
  replace(gen.replace), crossprob(gen.crossprob),
  mutprob(gen.mutprob), local_fit_max_iteration(gen.local_fit_max_iteration), 
  local_fit_tolerance(gen.local_fit_tolerance),
  do_resource_competition(gen.do_resource_competition),
  hist_cont_grid_cols(gen.hist_cont_grid_cols),
  population(gen.population), gene(gen.gene),
  createind( gen.createind ) {
    //cast the gene[-1] to a function of the correct kind
    member = new_Individual_list( gene, population, createind );
    for(int i=0; i<max_individuals;i++) *member[i] = *gen.member[i];
} // Generation partial copy constructor

void Generation::randinit(){//initialize the population with random genes
  for(int i=0; i<population; i++){
    member[i]->randinit();
  }
} // Generation::init

Generation::~Generation(){
    delete_Individual_list(member);
} // Generation destructor

void Generation::proportional(){
  // Use a proportional strategy with a steady state model to replace this
  // generation by the next one 
  merit_t rtot=total();
  // fill the children array with the best parents of the preceding
  // generation by making a partial copy of this generation
  int nchild = int( replace * population ); // number of children to create
  int ichild = population - nchild; // this is child index in new generation
  Generation chld( *this, ichild ); // make partial copy
  Individual **parent = new_Individual_list( gene, 2 , createind );
  // now for the new children
  while( ichild < population && !stop ){
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
    float p = MTRNGrand();
    if ( p <= crossprob )
      crossover( parent[0], parent[1] );
    p = MTRNGrand();
    if ( p <= mutprob )
      parent[0]->mutate();
    p = MTRNGrand();
    if ( p <= mutprob )
      parent[1]->mutate();
    // Add these children to the population
    *chld.member[ ichild++ ] = *parent[0];
    if( ichild < population )
      *chld.member[ ichild++ ] = *parent[1];
  } // for the rest of the child list
  /* Replace previous generation with this one.
   * ichild should only at most be equal to population.
  */
#ifdef DEBUG
  if ( ichild > population && debug_level == GA ) {
    gaerr << "Generation::proportional: ichild > population\n";
    pausel();
  } //if
#endif
  for ( int i=0; i<ichild; i++ ) {
    *member[i] = *chld.member[i];
  } // for i
  delete_Individual_list(parent);
} // Generation::proportional

void Generation::tournament() {
    /* Use a tournament strategy with a steady state model to replace this
     * generation by the next one 
     * fill the children array with the best parents of the preceding
     * generation by making a partial copy of this generation
     */
    int nchild = int(replace*population);// number of children to create
    int ichild=population-nchild; // this is the child index in the new generation
    Generation chld(*this,ichild);// make partial copy
    Individual **children = new_Individual_list( gene, 2, createind );

    // now for the new children
    while( ichild < population && !stop ) {
        // select parents by tournament
        int ip1 = int((population-0.001)*MTRNGrand());
        int ip2=ip1;
        while(ip2 == ip1) {
            ip2 = int((population-0.001)*MTRNGrand());
        } // while parents are identical

        // compare parents and keep the best one
        if (member[ip1]->Merit() < member[ip2]->Merit())
            ip1 = ip2; // second parent in tournament is better

        {// Now do it again for another parent
            ip2 = ip1;
            while(ip2==ip1)
                ip2 = int((population-0.001)*MTRNGrand());

            int ip3 = ip2;
            while((ip3 == ip2) || (ip3 == ip1)){
                ip3 = int((population-0.001)*MTRNGrand());
            } // while parents are identical

            // compare parents and keep the best one
            if (member[ip2]->Merit() < member[ip3]->Merit())
                ip2 = ip3; // second parent in tournament is better

        }// Okay, parents are selected (ip1 and ip2)
    
        // make a copy so crossover and mutation don't change original
        *children[0] = *member[ip1];
        *children[1] = *member[ip2];

        // breed by doing crossover and mutation on parents' DNA
        float p = MTRNGrand();
        if (p <= crossprob)
            crossover(children[0], children[1]);

        p = MTRNGrand();
        if (p <= mutprob)
            children[0]->mutate();

        p = MTRNGrand();
        if ( p <= mutprob )
            children[1]->mutate();

        // Add these children to the population
        *chld.member[ ichild++ ] = *children[0];
        if( ichild < population )
            *chld.member[ ichild++ ] = *children[1];

    } // for the rest of the child list

    /* Replace previous generation with this one.
     * ichild should only at most be equal to population.
    */
#ifdef DEBUG
    if ( ichild > population && debug_level == GA ) {
        gaerr << "Generation::tournament: ichild > population\n";
        pausel();
    } //if
#endif
    for ( int i=0; i<ichild; i++ ) {
        *member[i] = *chld.member[i];
    } // for i
    delete_Individual_list(children);
} // Generation::tournament

int Generation::tselect(merit_t rtot, int skip){
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
    while(ptot<p && ipar<population){
        // scan the list until ptot exceeds p
        if(++ipar != skip) // don't do this for the skipped element
            ptot += member[ipar]->Merit()/rtot;
    } // while ptot<p
    if(ipar>=population)
      return population-1;
    return ipar;
} // Generation::tselect

int Generation::seed(const Gene & seedgene, float seed_fraction){
    // insure inclusion of a specific solution in the set
    if (seed_fraction > 1.0) {
        THROW(std::runtime_error,"Generation::seed:  seed fraction must be <= 1.0");
    }

    int numseed = int(seed_fraction * population);
    for(int i=0;i<numseed; i++) {
        member[i]->regene(seedgene); 
    }//for
    return numseed;
} // Generation::seed

void Generation::hist(){
  // print a histogram of the random selections
  Histogram hist(population, hist_cont_grid_cols, *member[0]);
  for(int i=0; i<population; i++){
    hist.update(*member[i]);
  }
  gaout<<hist;
} // Generation::hist

#ifdef HYBRID

///
Gene_Simplex::Gene_Simplex(Allele_t ppar[], int nparams,Individual * ml):
  Fit<>(ppar,nparams),member(ml){
}//Gene_Simplex constructor

Gene_Simplex::~Gene_Simplex(){
}//Gene_Simplex destructor

merit_t Gene_Simplex::minfunc(Allele_t p[]){
  /* Update the layer thicknesses using the parameters and return the
   * new merit function.  Actually, I need to return minus the merit
   * function since simplex is set up to minimize, rather than
   * maximize a function.
  */
  return -member->test_Merit(p,ALLELE_DYNAMIC_CONT);
} // Gene_Simplex::minfunc

merit_t simplex_fit( Individual *member, int local_fit_max_iteration,
           float local_fit_tolerance ){
  // find a minimum of this population member
  // by doing local_fit_max_iteration steps using the simplex
  // algorithm
  int nalleles = member->DNA.numAlleles( ALLELE_DYNAMIC_CONT );
  if(!nalleles) {
    THROW(std::runtime_error,"simplex_fit: DNA has zero dynamic alleles(bad!).");
  }
  Allele_t *ppar    = new Allele_t[nalleles];
  Allele_t *minvals = new Allele_t[nalleles];
  Allele_t *maxvals = new Allele_t[nalleles];
  // Initialize fit parameters
  //Spencer Olson is going to get married next month to a 
  // very nice individual.  he seems pretty happy and is 
  // even willing to write about it in his code. -- Bob Bradford
  for(int i=0; i<nalleles; i++) {
    ppar[i] = member->DNA.Allele( i, ALLELE_DYNAMIC_CONT ).val;
    minvals[i] = member->DNA.Allele( i, ALLELE_DYNAMIC_CONT ).min;
    maxvals[i] = member->DNA.Allele( i, ALLELE_DYNAMIC_CONT ).max;
  }//for
  Gene_Simplex fit(ppar, nalleles, member);

  /* get member's merit before simplex.
   * NOTE:  I am obtaining bmerit and amerit
   * before and after applying the simplex,
   * respectively.  The reason for this is just
   * to keep the logic clear.  One could get these
   * values in any order (I think). */
  merit_t bmerit = member->Merit();

  fit.simplex(minvals,maxvals,local_fit_tolerance, local_fit_max_iteration);

  /* force a range onto the fit. */
  bool had_to_force_genes = false;
  for (int i = 0; i < nalleles; i++) {
      if (ppar[i] < minvals[i]) {
          ppar[i] = minvals[i];
          had_to_force_genes = true;
      } else if (ppar[i] > maxvals[i]) {
          ppar[i] = maxvals[i];
          had_to_force_genes = true;
      }
  }

  /* Now get the merit after having applied the simplex. */
  merit_t amerit = member->test_Merit(ppar,ALLELE_DYNAMIC_CONT);

  if (had_to_force_genes) {
      gaerr <<  "simplex_fit:  Simplex moved parameters outside of allowed "
                "ranges.  Coerced to extrema."
            << std::endl;
  }

  if( bmerit > amerit ) {//compare the two to see if the simplex worked
#ifdef DEBUG
   if ( debug_level == GA )
    gaerr<<"simplex_fit: Simplex went goofy\n"
         <<"\tmerit before= "<<bmerit
         <<"\n\tmerit after= "<<amerit<<std::endl;
#endif //DEBUG
  }
  else { //only regene if the simplex helped the design
    member->regene(ppar,ALLELE_DYNAMIC_CONT);
  }
  delete[] ppar; // only need this thing for this method
  delete[] minvals; // only need this thing for this method
  delete[] maxvals; // only need this thing for this method
  return amerit;
} // simplex_fit
#endif //HYBRID

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
  if(!population) return NULL;//will do nothing for this case
  Individual **ilist = new Individual *[population+1];
  if( !createind ) createind = create_Individual;
  for(long i = 1; i<(population+1);i++) {
    ilist[i] = createind( gn, meritfnc, obj_ptr );
  }//for
  ilist[0] = (Individual *)(population);
  return ++ilist;
} // new_population_list

void delete_Individual_list(Individual ** ilist) {
  --ilist;
  long population = (long)ilist[0];
  ilist++;
  for(long i = 0; i<population; delete ilist[i++]);
  delete[] --ilist;
} //delete_population_list

std::ostream & operator<<(std::ostream &output, const Generation & gen) {
    output<<"Top four Individuals in Generation\n";
    for(int i=0; i<4; i++){
        //output.width(2);
        output<<"member["<<i<<"]: "
              <<*gen.member[i]
              <<std::endl<<std::endl;
    } // for first items in list
  return output;
} // operator << Generation

}}/*namespace olson_tools::fit */
