// -*- c++ -*-
// $Id: Individual.C,v 1.2 2005/06/09 08:50:44 olsonse Exp $
/*
 * Copyright 199?-1998 Robert Steven Turley --- All Rights Reserved
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: Individual.C,v $
 * Revision 1.2  2005/06/09 08:50:44  olsonse
 * Style changes mostly.  Bug fixes in use of Histogram to encourage diversity.
 *
 * Revision 1.1  2005/06/07 20:38:11  olsonse
 * Fixed the old genetic algorithm files.  They compile.  Hopefully they work.
 *
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
 *
 * Revision 1.6  2000/05/06 20:37:18  olsons
 * Reorganized libfit: made it self-containing and use new definition of alleles.
 *
 * Revision 1.5  1999/11/03 23:29:38  olsons
 * Working with memory allocation.  Added capability of memory limit
 * in the memory manager file.
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
 * Revision 1.4  1997/12/29  16:50:34  turley
 * using angular averages and file io
 *
 * Revision 1.4  1997/12/29  16:50:34  turley
 * using angular averages and file io
 *
 * Revision 1.2  1997/12/17  18:51:26  turley
 * Fixed bug, added histogram capability and resource competition
 *
 * Revision 1.1  1997/12/02  19:56:36  turley
 * Initial revision
 *
 * Revision 1.1  1997/12/02  19:56:36  turley
 * Initial revision
 *
*/

#include <olson-tools/fit/Individual.h>
#include <olson-tools/fit/Gene.h>
#include <olson-tools/fit/debug.h>

namespace olson_tools{
  namespace fit {

    template < typename MF >
    inline Individual<MF>::Individual(const Individual & ind)
      : DNA( ind.DNA ),
        updatemerit( ind.updatemerit ),
        merit( ind.merit ),
        meritFunctor() {
    } // Individual copy constructor

    template < typename MF >
    inline Individual<MF>::Individual( const Gene & gene  )
      : DNA( gene ),
        updatemerit( true ),
        merit(0),
        meritFunctor() {
    } // Individual constructor

    template < typename MF >
    inline const Individual<MF> &
    Individual<MF>::operator= ( const Individual & ind ) {
      updatemerit = ind.updatemerit;
      DNA = ind.DNA;
      merit = ind.merit;
      meritFunctor = ind.meritFunctor;
      return *this;
    }//Individual<MF>::operator=


    template < typename MF >
    inline bool crossover( Individual<MF>& i1, Individual<MF>& i2,
                           const float & crossprob ) {
      // Do a crossover on the genes from i1 and i2 (other parts of multilayer are
      // not updated).  (Huh?? --spencer may12,1999)
      if ( crossover(i1.DNA, i2.DNA, crossprob) ) {
        i1.forceUpdate();
        i2.forceUpdate();
        return true;
      } else
        return false;
    } // crossover

    template < typename MF >
    inline void Individual<MF>::regene( const Gene &g ) {
      updatemerit = true;// we have updated the gene
      DNA = Chromosome(g);
    } // Individual<MF>::regene

    template < typename MF >
    inline void Individual<MF>::regene( const Gene &g, const merit_t & m ) {
      regene(g);
      merit = m;
      updatemerit = false;// we have updated the merit
    } // Individual<MF>::regene

    template < typename MF >
    inline void Individual<MF>::regene( Allele_t alleles[], int alleletype ) {
      updatemerit = true;// we have updated the gene
      DNA.regene(alleles,alleletype);//alleles better be the correct size, or we'll segfault
    } // Individual<MF>::regene

    template < typename MF >
    inline void Individual<MF>::regene( Allele_t alleles[],
                                        int alleletype,
                                        const merit_t & m ) {
      regene(alleles, alleletype);
      merit = m;
      updatemerit = false;// we have updated the gene
    } // Individual<MF>::regene

    /* The following function tests to see what the merit of this individual would
     * be if the DNA were defined by the values in the test_alleles[] array.  This
     * means that test_alleles[] must be the correct size, so that a segmentation
     * violation does not occur.
    */
    template < typename MF >
    template < typename AIter >
    inline
    merit_t Individual<MF>::test_Merit( AIter allele_iterator,
                                        unsigned char alleletype /* = 0 */ ){
      /* make a copy of original gene */
      Gene test_dna( DNA );

      /* Copy the new test_alleles over into the genes.
      */
      const unsigned int sz = DNA.size();
      for( unsigned int i = 0; i < sz; ++i ) {
        if ( TESTALLELETYPE( DNA[i].allele_type, alleletype) ) {
          test_dna[i].val = (*allele_iterator);
          ++allele_iterator;
        }
      }//for

      /* Evaluate and return the merit function. */
      return meritFunctor(test_dna);
    } // test_Merit

    template < typename MF >
    inline merit_t Individual<MF>::Merit(){
    // this function can be overridden by the inheriting programmer
      if (updatemerit) {
        updatemerit = false;//We only calculate meritfnc when needed
        return ( merit = meritFunctor( DNA ) );
      } else
        return merit;
    }// Individual<MF>::Merit()

    template < typename MF >
    inline void Individual<MF>::randinit(){
      DNA.randinit();
      updatemerit = true;
    }// randinit -- inititialize things randomly

    template < typename MF >
    inline void Individual<MF>::multMerit(merit_t mf){
      merit = Merit() * mf;
    }// Individual<MF>::multMerit

    template < typename MF >
    inline void
    Individual<MF>::setMerit( const merit_t & m,
                              const bool & do_not_updatemerit /* = true */ ) {
      merit = m;
      updatemerit = !do_not_updatemerit;
    }// Individual<MF>::setMerit

    template < typename MF >
    inline int Individual<MF>::mutate( const float & mutprob ) {
      int retval = DNA.mutate( mutprob );
      if ( retval > 0 )
        updatemerit = true;

      return retval;
    }//Individual<MF>::mutate

    template < typename MF >
    inline std::ostream & operator<<( std::ostream & out,
                                      const Individual<MF> & ind ) {
      out << "DNA:" << ind.DNA
          << "\nIndividual Merit: " << ind.getCurrentMerit();
      return out;
    } // operator << Individual

  }/*namespace olson_tools::fit */
}/*namespace olson_tools */

