

#ifndef olson_tools_nsort_map_type_h
#define olson_tools_nsort_map_type_h

#include <olson-tools/IteratorRange.h>

#include <vector>

namespace olson_tools {
  namespace nsort {
    namespace map {
      namespace tag {
        struct type {};
      }

      /** maps particle to particle.type.
       * This sort-map does <b>not</b> dictate that new children are to be created.
       * */
      struct type {
        typedef tag::type tag;
        typedef void super;

        template<class _Particle>
        inline int operator()(const _Particle & p) const {
          return p.type;
        }
   
      };/*struct type */
    }/*namespace map */
  }/*namespace nsort */
}/*namespace olson_tools */


#endif // olson_tools_nsort_map_type_h

