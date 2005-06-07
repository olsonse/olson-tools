// -*- c++ -*-
// $Id: Gene.C,v 1.1 2005/06/07 20:38:10 olsonse Exp $
/*
 * $Log: Gene.C,v $
 * Revision 1.1  2005/06/07 20:38:10  olsonse
 * Fixed the old genetic algorithm files.  They compile.  Hopefully they work.
 *
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
 *
 * Revision 1.7  2000/06/15 19:31:00  olsons
 * Made debug_level act more like levels rather than codes.
 *
 * Revision 1.6  2000/05/06 20:37:18  olsons
 * Reorganized libfit: made it self-containing and use new definition of alleles.
 *
 * Revision 1.5  1999/10/18 18:44:19  olsons
 * Changed variable declarations and initializations for several functions.
 *
 * Revision 1.4  1999/09/23 21:15:56  olsons
 * Just redoing constructors to use default values.  (not so much overloading)
 *
 * Revision 1.3  1999/07/28 00:32:08  olsons
 * fixed and re-enabled class::freetoheap so that we can free the 
 * memory to the heap again.
 * found a bug and squashed it in simfit (it was in checking 
 * for minvals[] and maxvals[] -- my addition to simfit)
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
 * Revision 1.8  1998/01/14  15:37:06  turley
 * added top oxide layers
 *
 * Revision 1.7  1997/12/29  16:50:34  turley
 * using angular averages and file io
 *
 * Revision 1.5  1997/12/17  18:51:26  turley
 * Fixed bug, added histogram capability and resource competition
 *
 * Revision 1.4  1997/12/10  22:15:41  turley
 * bug fixes
 *
 * Revision 1.1  1997/12/02  19:56:36  turley
 * Initial revision
 *
 */

#include "Gene.h"
#if defined (BUILD_TOOLS)
#  include "../random/random.h"
#else
#  include "random.h"
#endif
#include "io.h" // in and output
#include "error.h"
#include <stdlib.h>
#include <assert.h>

/* All of the following needs to be changed where we
   use Allele definitions, so that, if ALLELE_STATIC
   is used, then all of the static alleles are referenced
   and the same with ALLELE_DYNAMIC and with ALLELE_ALL
*/

inline Allele_t min(Allele_t i,Allele_t j) {
  if(i<j) return i;
  return j;
}//min

inline Allele_t max(Allele_t i,Allele_t j) {
  if(i>j) return i;
  return j;
}//min

Chromosome::Chromosome(const Gene& cgene): Gene(cgene) {
} // create chromosome with this gene make-up

Chromosome::~Chromosome(){
}// Chromosome destructor

void crossover(Chromosome& c1, Chromosome& c2){
  int nalleles=c1.Numalleles();
  if (nalleles!=c2.Numalleles()) {
     throw libfitError( CROSSOVER_MISMATCH );
  }
  // make t1 and t2 a copy of c1 and c2
  Chromosome t1(c1);
  Chromosome t2(c2);
  int iterations = 0;
  while(iterations<10) {
    // switching from one to another at a random point
    // First find a random gene
    // Use the first Chromosome to find random accessible gene
    int gloc = c1.randgene();//gene random gene index
    // Now find a random bit in the gene for the actual crossover point
    unsigned short int mask=0;
    // This may need to be changed in the future because we might
    // use doubles in gene
    int bit = int(15.99*MTRNGrand());  // there are only eight possible bits
    for(int j=bit; j<16; j++)
      mask = mask | (1<<j);
    if (CROSSOVER==CREAL){
    // make the children a hybrid of the parents 
    // with complementary random weights.  Another 
    // choice here for the hybrids would be to have different
    // random weights for each child
      // in the following, we should have w compatiblly typed with Alleles
      Allele_t w= MTRNGrand();//get random # between 0 and 1
      t1[gloc].val = w * c1[gloc].val + (1-w) * c2[gloc].val;
      t2[gloc].val = (1-w) * c1[gloc].val + w * c2[gloc].val;
    }
    else { //if (CROSSOVER==CBIT)
      //t1[gloc].val |= c1[gloc].val & mask;
      //t2[gloc].val |= c2[gloc].val & mask;
      //mask = ~mask;
      //t1[gloc].val |= c2[gloc].val & mask;
      //t2[gloc].val |= c1[gloc].val & mask;
    }
    // copy the switched genes (may be a zero pass loop)
    // make the second parts of t1 and t2 copies of c2 and c1 (switch)
    for(int i = ( gloc + 1 ); i<nalleles; i++){
      t1[i].val = c2[i].val;
      t2[i].val = c1[i].val;
    }
    // replace original chromosomes with crossover ones
    if(t1.valid() && t2.valid()){
      c1=t1;
      c2=t2;
      return;// it's valid, so let's finish this
    } else {
      // This used to be recursive
      //crossover(c1, c2);
      t1=c1;//copy to start loop over
      t2=c2;
      iterations++;
    } //if -- else
  }//while
} // crossover

void Chromosome::mutate(){
  // turn a random gene from a 1 to a 0 or vice-a-versa
  // with CROSSOVER==CREAL, this works a little different.  The real genes if 
  // mutated, take a random value in their range
  // First find a random gene
  //
  // This used to be recursive, but I got a stack overflow sometimes.
  bool validv=false;
  int loops=0;
  while(!validv){
    int gloc = randgene();//gene random gene index
#if CROSSOVER==CBIT
    // Now find a random bit in the gene
    int bit = int(15.99*MTRNGrand());  // there are only eight possible bits
    (*this)[gloc].val ^= 1<<bit;
#elif CROSSOVER==CREAL
    (*this)[gloc].val = __my_rand.rand((*this)[gloc].max - (*this)[gloc].min) + (*this)[gloc].min;
#else
#error "CROSSOVER has unrecognized value"
#endif
    validv=valid();
    if(!validv){
      if(++loops > 100){
	throw libfitError( CHROMOSOME_MUTATE_TOO_MANY_INVALID_LOOPS );
      } // if too many loops
// For this method, we need to be able to switch back to the original
#if CROSSOVER==CBIT
	(*this)[gloc].val ^= 1<<bit;   // switch back
#endif
    } // if not valid
  } // while not valid
} // Chromosome::mutate

std::ostream& operator<<(std::ostream &output, const Gene & gene) {
    output<<'\t';
    int nalleles=gene.Numalleles();
    for(int i=0;i<nalleles;i++)
      output << gene[i].val <<"  ";
    output<<std::endl;
    return output;
} // operator<<  Gene


Gene::Gene( int nalleles /* = 0 */, Allele_struct alls[] /* = 0 */ ):
  numalleles(0), alleles(NULL) {

  /* Allocate and assign alleles */
  if( nalleles && alls ) {
    //Allocation
    alleles = new Allele_struct[numalleles = nalleles];
    //Assignment
    for(int i=0;i<nalleles;i++) {
      alleles[i] = alls[i];
    }//for

  }//if
}// Gene specific constructor

Gene::Gene(const Gene& gn):
  numalleles(gn.numalleles), alleles(NULL) {

  if( numalleles ) {
    //Allocation
    alleles = new Allele_struct[ numalleles ];
    //Assignment
    for(int i=0;i<numalleles;i++) { // copy all of the parts
      alleles[i] = gn.alleles[i];
    }//for
  }//if

}//Gene copy constructor

Gene::~Gene(){
  delete[] alleles;
}//Gene destructor

void Gene::randinit() {
  // initialize gene to random, but valid value
  // init gene randomly
  for(int i=0; i<numalleles; i++){
    if( TESTALLELETYPE( alleles[i].allele_type, ALLELE_DYNAMIC ) )
      alleles[i].val = __my_rand.rand(alleles[i].max - alleles[i].min) + alleles[i].min;
  } // for i
} // Gene::randinit

bool Gene::valid() const {
  // Check to see if specific alleles are within the allowed range
  // otherwise return false.
  for (int i=0;i<numalleles;i++){
    if( TESTALLELETYPE( alleles[i].allele_type, ALLELE_DYNAMIC )
       && (  alleles[i].val < alleles[i].min
          || alleles[i].max < alleles[i].val ) ){
      return false;
    }
  }
  return true;  // okay otherwise
} // Gene::valid

int Gene::randgene() const {
  // Get a random gene index where Max(i)-Min(i)>0.
  // We will just get a random number between 0 and numalleles, 
  // and just circulate
  // through the alleles until we find an allele that can be modified.
  int rgint = int((numalleles-0.001)*MTRNGrand());
  int i=0;
  while( i < numalleles ){ // no need to circulate more than once
    // if beyond range, start at the first allele
    if( rgint >= numalleles) rgint=0;
    if( TESTALLELETYPE( alleles[rgint].allele_type, ALLELE_DYNAMIC ) )
        return rgint;
    i++;// increment iteration number
    rgint++;
  }
  throw
    libfitError( GENE_RANDGENE_NO_DYNAMIC_ALLELES );
}//Gene::randgene

void Gene::regene(const Allele_t newalleles[], const unsigned char alleletype){
  //This thing only updates the Allele_struct.val's
  assert(newalleles);
  // Assumes that sizeof(newalleles) is appropriate for alleles.
  // Also, we need to know what kind of allele it is that we're updating.
  // if( ( ((alleles[i].max-alleles[i].min)>0)
  for ( int i=0, ith=0; i<numalleles; i++ ) {
    if( TESTALLELETYPE( alleles[i].allele_type, alleletype ) ) {
      // make sure that we keep the alleles in proper boundaries
      alleles[i].val = min( alleles[i].max, 
                            max(alleles[i].min, newalleles[ith++]));
    }
  }
}//Gene::regene

Allele_struct & Gene::operator[](int i) {
  // read and write function
  // This function will return by reference, the ith allele.
  // If the index i was out of range, it will
  // throw an error
  //first check to see if index is valid; if not, throw error
  if( i >= 0 && i < numalleles ) return alleles[i];
  throw libfitError(( i<0 ? GENE_ALLELE_UNDERRUN:GENE_ALLELE_OVERRUN ));
}//Gene::operator[]

const Allele_struct & Gene::operator[](int i) const{
  // read function
  // This function will return by reference, the ith allele.
  // If the index i was out of range, it will
  // throw an error
  //first check to see if index is valid; if not, throw error
  if( i >= 0 && i < numalleles ) return alleles[i];
  throw libfitError(( i<0 ? GENE_ALLELE_UNDERRUN:GENE_ALLELE_OVERRUN ));
}//Gene::operator[]

int Gene::Numalleles() const {
  return numalleles;
}// Gene::Numalleles

int Gene::Numalleles(unsigned char alleletype) const {
  // refer to gene.h for the definitions of the alleletypes
  int nalleles=0;
  for ( int i=0; i < numalleles; i++ ) 
    if( TESTALLELETYPE( alleles[i].allele_type, alleletype ) ) nalleles++;
  return nalleles;
}// Gene::Numalleles(unsigned char alleletype) const

Allele_struct & Gene::Allele(int i, unsigned char alleletype) {
  // returns the ith allele of type alleletype
  // read and write function
  // This function will return by reference
  // If the index i was out of range, it will
  // throw an error
  int j(0), ith(0);
  if( i >= 0 && i < numalleles ) {//first check to see if index is valid
    // The ith allele of alleletype
    for ( j=0,ith=0; j<numalleles; j++ ) {
      if( TESTALLELETYPE( alleles[j].allele_type, alleletype ) ) {
        if (i==ith) {
          return alleles[j];
        }
        else ith++;
      }
    }
  } // if index valid
  throw libfitError( GENE_ALLELE_OUT_OF_INDEX );
} // Gene::Allele

const Allele_struct & Gene::Allele(int i, unsigned char alleletype) const {
  // returns the ith allele of type alleletype
  // read and write function
  // This function will return by reference
  // If the index i was out of range, it will
  // throw an error
  int j(0), ith(0);
  if( i >= 0 && i < numalleles ) {//first check to see if index is valid
    // The ith allele of alleletype
    for ( j=0,ith=0; j<numalleles; j++ ) {
      if( TESTALLELETYPE( alleles[j].allele_type, alleletype ) ) {
        if (i==ith) {
          return alleles[j];
        }
        else ith++;
      }
    }
  } // if index valid
  throw libfitError(GENE_ALLELE_OUT_OF_INDEX);
} // Gene::Allele

const Gene & Gene::operator=(const Gene & g2) {
  // gene copy operator
  // the following only reallocates space if the amount
  // of space needed is differnent than the already
  // allocated space

  if(numalleles != g2.numalleles) {
    delete[] alleles;alleles = NULL;
    if( (numalleles=g2.numalleles) ) alleles  = new Allele_struct[numalleles];
  } // reallocate the space for normal alleles

  /* now copy over some values */
  for( int i = 0; i< numalleles; i++ ) alleles[i] = g2.alleles[i];
  return *this;
} // operator= Gene

void Gene::copy_alleles_from_Gene( const Gene & g2,
                                   int a /* = 0 */, int b /* = 65355 */ ) {
  if( b >= g2.numalleles ) b = g2.numalleles -1;
  if( a > b || a < 0 ) return;
  // gene partial copy function
  // the following only reallocates space if the amount
  // of space needed is differnent than the already
  // allocated space

  if( numalleles != ( b - a + 1 ) ) {
    delete alleles; alleles = NULL;
    if((numalleles = ( b - a + 1 ) )) alleles  = new Allele_struct[numalleles];
  } // reallocate the space for normal alleles

  /* now copy over some values */
  for( int i = 0; i< numalleles; i++ ) alleles[i] = g2.alleles[a++];
}//Gene::copy_alleles_from_Gene

/// Gene add-to operator
const Gene & Gene::operator+=(const Gene & g2) {
  if( !g2.numalleles ) return *this;
  Allele_struct * tmpalleles = alleles;
  int i(0), iter;
  alleles = new Allele_struct[ iter = (numalleles + g2.numalleles) ];
  for( ; i < numalleles; i++) alleles[i] = tmpalleles[i];
  for( ; i < iter; i++) alleles[i] = g2.alleles[ i - numalleles ];
  numalleles = iter;
  delete[] tmpalleles;
  return *this;
}//Gene::operator+=

/// Gene add-to operator (with Allele_struct).
const Gene & Gene::operator+=( const Allele_struct & a ) {
  Allele_struct * tmpalleles = alleles;
  alleles = new Allele_struct[ numalleles + 1 ];
  for(int i = 0; i < numalleles; i++) alleles[i] = tmpalleles[i];
  alleles[ numalleles++ ] = a;
  delete[] tmpalleles;
  return *this;
}//Gene::operator+=

