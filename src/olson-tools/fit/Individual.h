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


#ifndef olson_tools_fit_Individual_h
#define olson_tools_fit_Individual_h

#include <olson-tools/fit/Gene.h>
#include <olson-tools/fit/io.h>
#include <olson-tools/fit/merit_def.h>

#include <olson-tools/memory.h>

namespace olson_tools{
  namespace fit {

    /** The base class of the population that will be created for
     * the genetic algorithm.
     *@memo Members of GeneticAlg's population.
    */
    template < typename _MeritFunctor >
    class Individual {
      /* TYPEDEFS */
    public:
      typedef _MeritFunctor MeritFunctor;

      /* MEMBER STORAGE */
    public:
      /** The DNA of this Individual. */
      Chromosome DNA;

    private:
      /// boolean set to true(1) when DNA has been regened.
      bool updatemerit;

      /** The cached merit value of this Individual. */
      merit_t merit;

      /** The merit functor. */
      MeritFunctor meritFunctor;


      /* MEMBER FUNCTIONS */
    public:
      /** Constructor to create specific DNA.
       * All will need this specific DNA because we need the range values
       * present in order to create something random.
       * Therefore, initialize the Individual with a specific gene,
       * then procede to call randinit(), which will randomize things.
       * FIXME:  an option to randomize via a given distribution would be nice.
       */
      inline Individual( const Gene & gene );

      /// Copy Constructor
      inline Individual(const Individual &);

      /// clean things up
      inline ~Individual() { }

      /// Assign operator.
      inline const Individual & operator= ( const Individual & );

      /// Update organism without changing DNA, return new merit function.
      template < typename AIter >
      inline merit_t test_Merit( AIter allele_iterator,
                                 unsigned char alleletype = 0 );

      /// redo the DNA
      inline void regene(const Gene &);

      /// redo the DNA and set the merit and updatemerit to false
      inline void regene( const Gene &, const merit_t & m );

      /// update DNA
      inline void regene( Allele_t thick[], int alleletype );

      /// update DNA and set the merit and updatemerit to false
      inline void regene( Allele_t thick[], int alleletype, const merit_t & m );

      /** Multiply the merit of this individual by mf to create a new merit.  */
      inline void multMerit(merit_t mf);

      /** Explicitly (and arbitrarily?) set the merit. */
      inline void setMerit( const merit_t & m, const bool & do_not_updatemerit = true );

      /** Set updatemerit to true. */
      inline void forceUpdate() { updatemerit = true; }

      /**@return Returns Merit unless genes have been updated;
       * in that case merit is set equal to the new meritfunction.
       */
      inline merit_t Merit();

      /**@return Returns Merit unless genes have been updated;
       * in that case merit is set equal to the new meritfunction.
       */
      inline merit_t getCurrentMerit() const { return merit; }

      /// inititialize things randomly
      inline void randinit();

      /** Mutate the DNA of this Individual.
       * @returns the number of alleles actually mutated.
       */
      inline int mutate( const float & mutprob );

    }; // Individual

    ///
    template < typename MF >
    inline bool crossover( Individual<MF> & i1, Individual<MF> & i2,
                           const float & crossprob );

    ///
    template < typename MF >
    inline bool crossover( Individual<MF> * i1, Individual<MF> * i2,
                           const float & crossprob ) {
      return crossover( *i1, *i2, crossprob );
    }

    ///The Individual print function.
    template < typename MF >
    inline std::ostream & operator<< ( std::ostream &, const Individual<MF> & );

  }/*namespace olson_tools::fit */
}/*namespace olson_tools */

#include <olson-tools/fit/Individual.cpp>

#endif //olson_tools_fit_Individual_h

