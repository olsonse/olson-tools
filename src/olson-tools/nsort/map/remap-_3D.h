
#ifndef olson_tools_nsort_map_remap__3D_h
#define olson_tools_nsort_map_remap__3D_h

#include <olson-tools/nsort/map/_3D.h>
#include <olson-tools/nsort/map/remap_base.h>

namespace olson_tools {
  namespace nsort {
    namespace map {
      namespace tag {
        struct remap_3D {};
      }

      /** Specialization of remap for _3D. */
      template < unsigned int _dir0, unsigned int _dir1, unsigned int _dir2 >
      struct remap< _3D<_dir0,_dir1,_dir2> >
        : remap_base< _3D<_dir0,_dir1,_dir2>, 8u > {
        typedef tag::remap_3D tag;
        typedef remap_base< _3D<_dir0,_dir1,_dir2>, 8u > super;
        /** Default constructor required by wrapping components. */
        remap() {}
        /** Single arg constructor required by wrapping components.*/
        template <class TT> remap(const TT & tt) : super(tt) {}
      };
    }
  }
}

#endif // olson_tools_nsort_map_remap__3D_h

