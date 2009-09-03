
#include <olson-tools/fit/GeneSimplex.h>
#include <olson-tools/fit/simfit.h>

#include <olson-tools/ompexcept.h>

#include <stdexcept>

namespace olson_tools {
  namespace fit {

    class GeneSimplex::Impl: public Fit<merit_t,merit_t> {
      /* MEMBER STORAGE */
    private:
      ///
      Individual * member;

      /* MEMBER FUNCTIONS */
    public:
      /**@param ppar[] should be an array of type Allele\_t of all of
       * the alleles which are of type: ALLELE\_DYNAMIC\_COORD.
       *@param nparams should be the size of ppar[].
       *@param ml should then be a pointer to the appropriate Individual object.
       *@memo Gene\_Simplex constructor.
      */
      Impl( Allele_t ppar[], int nparams, Individual * ml )
        : Fit<>(ppar,nparams),member(ml) { }
      ///
      virtual ~Impl() { }

      ///
      virtual merit_t minfunc(Allele_t p[]) {
        /* Update the layer thicknesses using the parameters and return the
         * new merit function.  Actually, I need to return minus the merit
         * function since simplex is set up to minimize, rather than
         * maximize a function.
        */
        return -member->test_Merit(p,ALLELE_DYNAMIC_CONT);
      } // GeneSimplex::minfunc
    }; // Impl class

    merit_t GeneSimplex::operator() ( Individual *member,
                                      const Parameters & param ) const {
      // find a minimum of this population member
      // by doing param.max_iteration steps using the simplex
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
      for(int i=0; i<nalleles; ++i) {
        ppar[i] = member->DNA.Allele( i, ALLELE_DYNAMIC_CONT ).val;
        minvals[i] = member->DNA.Allele( i, ALLELE_DYNAMIC_CONT ).min;
        maxvals[i] = member->DNA.Allele( i, ALLELE_DYNAMIC_CONT ).max;
      }//for
      Impl fit(ppar, nalleles, member);

      /* get member's merit before simplex.
       * NOTE:  I am obtaining bmerit and amerit
       * before and after applying the simplex,
       * respectively.  The reason for this is just
       * to keep the logic clear.  One could get these
       * values in any order (I think). */
      merit_t bmerit = member->Merit();

      fit.simplex(minvals,maxvals,param.tolerance, param.max_iteration);

      /* force a range onto the fit. */
      bool had_to_force_genes = false;
      for (int i = 0; i < nalleles; ++i) {
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
      } else { //only regene if the simplex helped the design
        member->regene(ppar, ALLELE_DYNAMIC_CONT, amerit);
      }
      delete[] ppar; // only need this thing for this method
      delete[] minvals; // only need this thing for this method
      delete[] maxvals; // only need this thing for this method
      return amerit;
    } // GeneSimplex::fit

  }/* namespace olson_tools::fit */
}/* namespace olson_tools */

