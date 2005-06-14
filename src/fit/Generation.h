// -*- c++ -*-
// $Id: Generation.h,v 1.1 2005/06/14 02:28:19 olsonse Exp $
/*
 * $Log: Generation.h,v $
 * Revision 1.1  2005/06/14 02:28:19  olsonse
 * Split out some files and renamed to be more consistent with olson-tools.
 *
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
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
 * Revision 1.1.1.1  1998/10/03 16:07:52  olsons
 * Imported Genetic Algorithm
 *
 * Revision 1.5  1997/12/29  16:50:34  turley
 * using angular averages and file io
 *
 * Revision 1.5  1997/12/29  16:50:34  turley
 * using angular averages and file io
 *
 * Revision 1.4  1997/12/19  22:10:48  turley
 * added true tournament selection
 *
 * Revision 1.3  1997/12/17  18:51:26  turley
 * fixed bug, added histogram and resource allocation for diversity
 *
 * Revision 1.2  1997/12/10  22:15:41  turley
 * bug fixes
 *
 * Revision 1.1  1997/12/02  20:04:42  turley
 * Initial revision
 *
*/

#ifndef GENERATION_H
#define GENERATION_H

// If the following is defined, a hybrid algorithm is used (simplex in
// conjunction with ga)
#define HYBRID

#include "io.h" // out and input
#include "error.h" // error definitions
#include "Individual.h"
#include "Histogram.h"

///
class Generation{
public:
  /// only storage with non initialization
  Generation(int pop, const Gene & igene,
             CREATE_IND_FUNC createind = NULL,
             void * meritfnc = NULL,
             void * obj_ptr = NULL );
  /// will we need to redo this?
  Generation(const Generation& gen);
  /// only copy so many
  Generation(const Generation&, int max_individuals);
  ///
  ~Generation();

  /// intialize with random population members
  void randinit();
  ///
  merit_t merit();
  /// sort and set gene to member[0]->gene
  void sort();
  ///
  void tournament();
  ///
  void proportional();
  /// start with some good solutions
  void seed(const Gene &,float seedprob);
  /// histogram to check random number generation
  void hist();
  ///
  const Gene & bestgene() const;

  /**Some algorithm parameters which are changeable.
  */
  //@{
    ///Percentage of population to use fitting algorithm on.
  float local_fit_max_individuals_prctage;

    ///Fraction of each generation to be replaced(.5).
  float replace;

    ///Crossover probability(.75).
  float crossprob;

    ///Mutation probability(.05).
  float mutprob;

    // fitting algorithm parameters
    /// maximum number of iterations of fitting algorithms
  int local_fit_max_iteration;

    /// tolerance of local fitting algorithms
  float local_fit_tolerance;
  //@}

  /** This is for stopping the algorithm during operation and
   * should be called by a signal handler from the OS.
   *@memo If true, then STOP!!
  */
  static void setStop(int inSignal);
  //?? Can't remember
  static bool stop;
  ///
  friend std::ostream & operator<<(std::ostream &,const Generation &);

  /** Do Histogramming. 
   * If true, the program tries to improve diversity by penalizing
   * too many individuals of the same species in the population.
   */
  bool do_resource_competition;

  // Histogram parameters
  /** This is the grid spacing of coordinate-system type genes in
   * histogramming. */
  int hist_cont_grid_cols;

private:
  /// return the total of the merit functions
  merit_t total();
  ///
  int tselect(merit_t rtot, int skip);
  ///
  int population;
  /// copy of initial ranges and will hold the best gene of the population
  Gene gene;

  /// Creator function for the Individuals.
  CREATE_IND_FUNC createind;

  ///
  Individual **member;
}; // Generation class

#ifdef HYBRID
#include "simfit.h"

/** This is an implemented class of the simplex algorithm
 * which will nicely find the local extrema of the
 * merit function which will be passed in through the Individual's
 * Gene.
 *@memo Simplex for Genes.
*/
class Gene_Simplex: public Fit<merit_t,merit_t> {
public:
  /**@param ppar[] should be an array of type Allele\_t of all of
   * the alleles which are of type: ALLELE\_DYNAMIC\_COORD.
   *@param nparams should be the size of ppar[].
   *@param ml should then be a pointer to the appropriate Individual object.
   *@memo Gene\_Simplex constructor.
  */
  Gene_Simplex(Allele_t ppar[], int nparams, Individual * ml);
  ///
  virtual ~Gene_Simplex();
  ///
  virtual merit_t minfunc(Allele_t p[]);
private:
  ///
  Individual * member;
}; // Gene_Simplex class

/** simplex_fit(...) is a wrapper for the Gene\_Simplex.  It will create
 * a Gene\_Simplex and pass to it all pertinant arrays and information.
 * Perhaps we should combine this function into the constructor
 * of the Gene\_Simplex.
 *@memo Runs simplex algorithm on an Individual.
*/
merit_t simplex_fit( Individual * member,
                     int local_fit_max_iteration,
                     float local_fit_tolerance);

#endif //HYBRID

///The Generation print function.
std::ostream & operator<<(std::ostream &, const Generation &);

#endif // GENERATION_H

