

#ifndef olson_tools_nsort_map_w_species_h
#define olson_tools_nsort_map_w_species_h


#include <olson-tools/IteratorRange.h>

namespace olson_tools {
  namespace nsort {
    namespace map {
      namespace tag {
        struct w_species {};
      }

      /** Wraps a map (may be an aggregate) inside a species map such that the
       * species map creates the slowest moving index.  
       *
       * The species information will be attempted to be retrieved via a
       * required Argument-Dependent Lookup ADL function called species that
       * takes a single argument of the const Particle &. 
       * */
      template <class T>
      struct w_species : T {
        typedef tag::w_species tag;
        typedef T super;
        const unsigned int n_species;

        w_species(const unsigned int & n_species) : n_species(n_species) {}
        template <class TT> w_species(const TT & tt) : T(tt) {}

        inline int getNumberValues() const {
          return T::getNumberValues() * n_species;
        }

        /** Map operation used when performing sorting. */
        template<class Particle>
        inline int operator()(const Particle & p) const {
          return n_species * T::operator()(p) + species(p);
        }

      };/*struct w_species */
    }/*namespace map */
  }/*namespace nsort */
}/*namespace olson_tools */


#endif // olson_tools_nsort_map_w_species_h

