// -*- c++ -*-
// $Id: gene.h,v 1.1 2005/01/08 04:27:25 olsonse Exp $
/*
 * $Log: gene.h,v $
 * Revision 1.1  2005/01/08 04:27:25  olsonse
 * Initial revision
 *
 * Revision 1.6  2000/06/15 19:31:00  olsons
 * Made debug_level act more like levels rather than codes.
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
 * Revision 1.1.1.1  1998/10/03 16:07:52  olsons
 * Imported Genetic Algorithm
 *
 * Revision 1.7  1998/01/14  15:37:06  turley
 * added top oxide layers
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

#ifndef GENE_H
#define GENE_H
#include "error.h" // libfitError definition
#include "io.h" // in and output

/// Alleles are stored in memory as datatype Allele_t; currently as float.
#define Allele_t float

/**@name Allele types
 * Allele types are catagorized as follows:
 * static alleles are negative;
 * dynamic alleles are positive;
 * ZERO indicates all alleles.
*/
//@{
/// Tests true for all types of alleles.
const unsigned char ALLELE_ALL = 0;
///
const unsigned char ALLELE_DYNAMIC = 1;
// Those which are dynamic and can be used as parameter space coordinates.
const unsigned char ALLELE_CONTINUOUS = 4;
///
const unsigned char ALLELE_STATIC = ~ALLELE_DYNAMIC;
///
const unsigned char ALLELE_DYNAMIC_CONT = ALLELE_DYNAMIC | ALLELE_CONTINUOUS;
//@}

/**Allele Implementation
 */
//@{
typedef struct Allele_struct {
  Allele_t min;
  Allele_t val;
  Allele_t max;
  unsigned char allele_type;
  Allele_struct( Allele_t i = 0, unsigned char at = ALLELE_STATIC ):
    min(i), val(i), max(i), allele_type(at) {}
};
//@}

// Choose crossover type (CBIT or CREAL).  This will effect the algorithms
// used for both crossover and mutation
#define CBIT 0
#define CREAL 1
#define CROSSOVER CREAL

/** The Gene class is the class in which all of the genetic information
 * is actually stored.
 *@memo class Gene
*/
class Gene{
 public:
   ///
   Gene( int nalleles = 0, Allele_struct alls[] = 0 );
   ///
   Gene(const Gene& gn);
   ///
   ~Gene();

   ///
   void randinit();

   /// Whether alleles are inside ranges.
   bool valid() const;

   /// Write function.
   Allele_struct & operator[](int i);

   /// Read function.
   const Allele_struct & operator[]( int i ) const;

   /// Gene copy operator.
   const Gene & operator=( const Gene & g2 );

   /// Gene add-to operator
   const Gene & operator+=( const Gene & g2 );

   /// Gene add-to operator
   const Gene & operator+=( const Allele_struct & a );

   ///
   int Numalleles() const;

   /// Returns num of certain types of alleles (see above for types).
   int Numalleles(unsigned char alleletype) const ;

   /// Returns the ith value of alleletype alleles.
   Allele_struct & Allele(int i, unsigned char alleletype);

   /// Returns the ith value of alleletype alleles.
   const Allele_struct & Allele(int i, unsigned char alleletype) const;

   /// Get a random gene index where Max(i)-Min(i)>0.
   int randgene() const ;

   /**@memo Assumes that sizeof(newalleles)
    * is appropriate for alleles.
    * Also, we need to know if we're just
    * updated ALLELE_DYN_COORD or ALLELE_DYN etc.
   */
   void regene(const Allele_t newalleles[], unsigned char alleletype);

   void copy_alleles_from_Gene( const Gene &, int a = 0, int b = 65355 );

 private:

   ///number of alleles in genes
   int numalleles;

   /// The alleles.
   Allele_struct *alleles;

}; // class gene

/** This is basically just a wrapper for the Gene class.
 * It also contains functions for the algorithm such as mutate.
 * Crossover is executed on Chromosomes.  In the future, we should
 * probably just change the Chromosome class so that it inherits from
 * Gene.  This would give us a bit more sleekness in the layout of the
 * program and will allow us to not have to have wrapper functions 
 * contained in Chromosome for all of the functions of Gene.
 *@memo Gene Wrapper
*/
class Chromosome : public Gene {
public:
  /// create chromosome with this gene make-up
  Chromosome(const Gene& cgene);
  /// Chromosome destructor
  ~Chromosome();
  /// mutate gene internally
  void mutate();
}; // Chromosome

///
ostream & operator<<(ostream &, const Gene &);


///Chromosome crossover machine.
void crossover(Chromosome& c1, Chromosome& c2);

/** TESTALLELETYPE is a macro to be used to test whether
 * an allele is of a certain type.  See above for specific
 * types.
 *@memo Test an Alleletype
*/
#define  TESTALLELETYPE( allele_type_variable, allele_type_test ) \
 ( ( ~allele_type_variable & allele_type_test ) == 0 )

#endif // GENE_H

