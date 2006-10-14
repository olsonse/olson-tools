// -*- c++ -*-
// $Id: Individual.h,v 1.2 2005/06/09 09:12:08 olsonse Exp $
/*
 * Copyright 199?-1998 Robert Steven Turley --- All Rights Reserved
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: Individual.h,v $
 * Revision 1.2  2005/06/09 09:12:08  olsonse
 * Style changes.
 *
 * Revision 1.1  2005/06/07 20:38:11  olsonse
 * Fixed the old genetic algorithm files.  They compile.  Hopefully they work.
 *
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
 *
 * Revision 1.5  2000/05/06 20:37:18  olsons
 * Reorganized libfit: made it self-containing and use new definition of alleles.
 *
 * Revision 1.4  1999/10/18 18:56:34  olsons
 * Added documentation in comments to be parsed by doc++.
 *
 * Revision 1.3  1999/09/23 21:15:56  olsons
 * Just redoing constructors to use default values.  (not so much overloading)
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
 * Revision 1.6  1997/12/29  16:50:34  turley
 * using angular averages and file io
 *
 * Revision 1.6  1997/12/29  16:50:34  turley
 * using angular averages and file io
 *
 * Revision 1.4  1997/12/17  18:51:26  turley
 * fixed bug, added histogram and resource allocation for diversity
 *
 * Revision 1.3  1997/12/10  22:15:41  turley
 * bug fixes
 *
 * Revision 1.1  1997/12/02  20:04:42  turley
 * Initial revision
 *
*/

/** \file
 * Base class of an Individual in the genetic algorithm code.
 *
 * Copyright 199?-1998 Robert Steven Turley --- All Rights Reserved
 *
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 */


#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "Gene.h"
#include "../memory.h"
#include "io.h"
#include "merit_def.h"

typedef merit_t (*MERIT_FUNCTION)( const Gene &, void * );

/** The base class of the population that will be created for
 * the genetic algorithm.
 *@memo Members of GeneticAlg's population.
*/
class Individual{
  ///
  REDEF_NEW_DEL_H(Individual);
public:
  // constructor to create specific DNA
  // all will need this specific DNA because we need the range values
  // present in order to create something random.
  // Therefore, initialize the Individual with a specific gene,
  // then procede to call randinit(), which will randomize things.
  ///
  Individual( const Gene & gene, void * meritfptr = NULL,
              void * obj_ptr = NULL );
  ///
  Individual(const Individual &);
  /// clean things up
  virtual ~Individual();
  /// Update organism without changing DNA, return new merit function.
  merit_t test_Merit( Allele_t test_alleles[], unsigned char alleletype = 0 );
  /// redo the DNA
  void regene(const Gene &);
  /// update DNA
  void regene(Allele_t thick[],int alleletype);

  /** Multiply the merit of this individual by mf to create a new merit.
   */
  void multMerit(merit_t mf);

  /**@return Returns Merit unless genes have been updated;
   * in that case merit is set equal to the new meritfunction.
  */
  virtual merit_t Merit();

  /// inititialize things randomly
  void randinit();

  /// Assign operator.
  virtual const Individual & operator=(const Individual &);

  ///
  void mutate();
  ///The pointer to the merit function.
  MERIT_FUNCTION meritfnc;
  ///
  friend void crossover(Individual& i1, Individual& i2);
  ///
  friend void crossover(Individual *i1, Individual *i2);
  ///
  friend std::ostream & operator<<(std::ostream &, const Individual &);
  ///
  Chromosome DNA;
protected:

  /// boolean set to true(1) when DNA has been regened.
  bool updatemerit;

  ///
  merit_t merit;

  /// A pointer which can be set to point to any exterior object.
  void * exterior_pointer;

}; // Individual

/* Every derived class of Individual must provide
 * a create_DerivedClass() function
 * like the following: */
typedef Individual * (*CREATE_IND_FUNC)( const Gene &,
                                         void * meritfnc /* = NULL */,
                                         void * obj_ptr /* = NULL */ );

/** When you use the GeneticAlg class you can have the 
 * population of the algorithm be a population of any class
 * derived from the Individual class.  To do this, you
 * will need to pass in a pointer to a function similar
 * to create\_Individual(...).  Your function might be
 * something like the following:

YourDerivedClass * create\_YourDerivedClass( const Gene \& gn,
                                             void * meritfnc = NULL,
                                             void * obj\_ptr = NULL );\\

 * The (Gene \&) that is passed in is because all individuals
 * need to be initialized with some genetic info wrapped up
 * in a Gene.  You must use the c++ 'new' function to create
 * your class (duh!).  You should also make sure that your
 * derived class takes care of setting the merit-function pointer
 * to something more useful than NULL if nothing is given.
 *@memo Population creator.
 *@return Returns a pointer to the Individual created.
*/
Individual * create_Individual( const Gene & gn,
                                void * meritfnc = NULL,
                                void * obj_ptr = NULL );

#ifdef __SC__
  //
  int __cdecl mcomp(const void* l1, const void* l2);
#else
  //
  int mcomp(const void* l1, const void* l2);
#endif

///The Individual print function.
std::ostream & operator<<(std::ostream &, const Individual &);

#endif //INDIVIDUAL_H

