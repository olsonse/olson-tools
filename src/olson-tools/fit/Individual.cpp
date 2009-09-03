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

#include "Individual.h"
#include "Gene.h"
#include "debug.h"

namespace olson_tools{ namespace fit {


///
#ifdef __SC__
int __cdecl mcomp(const void* l1, const void* l2){
#else
int mcomp(const void* l1, const void* l2){
#endif
    // need to cast then dereference
    Individual *v1=*(Individual**)l1;
    Individual *v2=*(Individual**)l2;
    if(v1->Merit()<v2->Merit()) return 1;
    if(v1->Merit()>v2->Merit()) return -1;
    return 0;
} // mcomp

void crossover(Individual& i1, Individual& i2){
    // Do a crossover on the genes from i1 and i2 (other parts of multilayer are
    // not updated).  (Huh?? --spencer may12,1999)
    crossover(i1.DNA, i2.DNA);
    i1.updatemerit = true;
    i2.updatemerit = true;
} // crossover

void crossover(Individual* i1, Individual* i2){
    // Do a crossover on the genes from i1 and i2 (other parts of multilayer are
    // not updated).  (Huh?? --spencer may12,1999)
    crossover(i1->DNA, i2->DNA);
    i1->updatemerit = true;
    i2->updatemerit = true;
} // crossover

REDEF_NEW_DEL_C(Individual,0);

void Individual::regene( const Gene &g ) {
  updatemerit = true;// we have updated the gene
  DNA = Chromosome(g);
} // Individual::regene

void Individual::regene( const Gene &g, const merit_t & m ) {
  regene(g);
  merit = m;
  updatemerit = false;// we have updated the merit
} // Individual::regene

void Individual::regene( Allele_t alleles[], int alleletype ) {
  updatemerit = true;// we have updated the gene
  DNA.regene(alleles,alleletype);//alleles better be the correct size, or we'll segfault
} // Individual::regene

void Individual::regene( Allele_t alleles[],int alleletype, const merit_t & m) {
  regene(alleles, alleletype);
  merit = m;
  updatemerit = false;// we have updated the gene
} // Individual::regene

Individual::Individual(const Individual & ind):
  meritfnc( ind.meritfnc ), DNA( ind.DNA ),
  updatemerit( ind.updatemerit ),
  merit( ind.merit ), exterior_pointer( ind.exterior_pointer ) {
} // Individual copy constructor

Individual::Individual( const Gene & gene ,
                        void * meritfptr /* = NULL */,
                        void * obj_ptr /* = NULL */ ):
  meritfnc( (MERIT_FUNCTION) meritfptr ), DNA( gene ),
  updatemerit( true ), exterior_pointer( obj_ptr ) {
} // Individual constructor

Individual::~Individual(){
};// clean things up

/* The following function tests to see what the merit of this individual would
 * be if the DNA were defined by the values in the test_alleles[] array.  This
 * means that test_alleles[] must be the correct size, so that a segmentation
 * violation does not occur.
*/
merit_t Individual::test_Merit( Allele_t test_alleles[],
                                unsigned char alleletype /* = 0 */ ){
  int i,j, N;
  Allele_struct * a;
  Allele_t * tmp_alleles = new Allele_t[ N = DNA.numAlleles() ];

  /* First make a temporary stor of the original alleles and copy the new
   * test_alleles over into the genes.
  */
  for( i = j = 0; i < N; i++ ) {
    tmp_alleles[i] = ( a = &DNA[i] )->val;
    a->val = TESTALLELETYPE( a->allele_type, alleletype)
                 ? test_alleles[j++] : a->val;
  }//for
  merit_t tmp1_merit = merit; // Save the old merit value.

  /* Evaluate the merit function. */
  updatemerit = true;
  merit_t tmp2_merit = Merit();

  /* Now copy the original values back into place. */
  updatemerit = false;
  merit = tmp1_merit; // Reset the merit value.
  for( i = j = 0; i < N; i++ ) {
    DNA[i].val = tmp_alleles[i];
  }//for

  /* Now clean up and return a value. */
  delete[] tmp_alleles;
  return tmp2_merit;
} // test_Merit

merit_t Individual::Merit(){
// this function can be overridden by the inheriting programmer
  if(updatemerit) {
    updatemerit = false;//We only calculate meritfnc when needed
    if( meritfnc )
      return ( merit = meritfnc( DNA, exterior_pointer ) );
    else {
#ifdef DEBUG
      if ( debug_level == GA ) {
        gaerr << "Individual::Merit(): NULL merit function.\n";
      } //if
#endif
      return merit = 0;
    }//else
  }
  return merit;
}// Individual::Merit()

const Individual & Individual::operator=(const Individual & ind){
  updatemerit = ind.updatemerit;
  DNA = ind.DNA;
  meritfnc = ind.meritfnc;
  merit = ind.merit;
  exterior_pointer = ind.exterior_pointer;
  return *this;
}//Individual::operator=

void Individual::randinit(){
  DNA.randinit();
  updatemerit = true;
}// randinit -- inititialize things randomly

void Individual::multMerit(merit_t mf){
  merit = Merit() * mf;
}// Individual::multMerit

void Individual::setMerit( const merit_t & m ) {
  merit = m;
  updatemerit = false;
}// Individual::setMerit

void Individual::mutate(){
  DNA.mutate();
  updatemerit = true;
}//Individual::mutate

std::ostream & operator<<(std::ostream &output, const Individual & ind) {
  output<<"DNA:"
        << ind.DNA
        <<"\nIndividual Merit: "<<ind.merit;
  return output;
} // operator << Individual

Individual * create_Individual( const Gene & gn,
                                void * meritfnc /* = NULL */,
                                void * obj_ptr /* = NULL */ ) {
  return new Individual( gn, meritfnc, obj_ptr );
}

}}/*namespace olson_tools::fit */
