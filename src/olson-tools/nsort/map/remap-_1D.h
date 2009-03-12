
#ifndef olson_tools_nsort_map_remap__1D_h
#define olson_tools_nsort_map_remap__1D_h

#include <olson-tools/nsort/map/_1D.h>
#include <olson-tools/nsort/map/remap_base.h>

namespace olson_tools {
  namespace nsort {
    namespace map {
      /** Specialization of remap for _1D. */
      template < unsigned int _dir0 >
      struct remap< _1D<_dir0> > : remap_base< _1D<_dir0>, 2u > {
        typedef remap_base< _1D<_dir0>, 2u > super;
        /** Default constructor required by wrapping components. */
        remap() : super() {}
        /** Single arg constructor required by wrapping components.*/
        template <class TT> remap(const TT & tt) : super(tt) {}
      };
    }
  }
}

#endif // olson_tools_nsort_map_remap__1D_h

