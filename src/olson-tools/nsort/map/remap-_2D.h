
#ifndef olson_tools_nsort_map_remap__2D_h
#define olson_tools_nsort_map_remap__2D_h

#include <olson-tools/nsort/map/_2D.h>
#include <olson-tools/nsort/map/remap_base.h>

namespace olson_tools {
  namespace nsort {
    namespace map {
      /** Specialization of remap for _2D. */
      template < unsigned int _dir0, unsigned int _dir1 >
      struct remap< _2D<_dir0,_dir1> > : remap_base< _2D<_dir0,_dir1>, 4u > {
        typedef remap_base< _2D<_dir0,_dir1>, 4u > super;
        /** Default constructor required by wrapping components. */
        remap() {}
        /** Single arg constructor required by wrapping components.*/
        template <class TT> remap(const TT & tt) : super(tt) {}
      };
    }
  }
}

#endif // olson_tools_nsort_map_remap__2D_h

