

#ifndef olson_tools_nsort_map_species_h
#define olson_tools_nsort_map_species_h

#include <olson-tools/IteratorRange.h>

#include <vector>

namespace olson_tools {
  namespace nsort {
    namespace map {
      namespace tag {
        struct species_only {};
      }

      /** maps particle to particle.species.
       * This sort-map does <b>not</b> dictate that new children are to be created.
       *
       * The species information will be attempted to be retrieved via a
       * required Argument-Dependent Lookup ADL function called species that
       * takes a single argument of the const Particle &. 
       * */
      struct species_only {
        typedef tag::species_only tag;
        typedef void super;

        template<class Particle>
        inline int operator()(const Particle & p) const {
          return species(p);
        }
   
      };/*struct species_only */
    }/*namespace map */
  }/*namespace nsort */
}/*namespace olson_tools */


#endif // olson_tools_nsort_map_species_h

