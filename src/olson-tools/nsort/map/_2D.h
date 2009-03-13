
#ifndef olson_tools_nsort_map__2D_h
#define olson_tools_nsort_map__2D_h

#include <olson-tools/nsort/map/_1D.h>
#include <olson-tools/nsort/map/depth_to_for2d.h>

namespace olson_tools {
  namespace nsort {
    namespace map {
      /** maps x[_dir0],x[_dir1] to 0,1,2,3 */
      template <unsigned int _dir0, unsigned int _dir>
      struct _2D : _1D<_dir0> {
        /* TYPEDEFS */
        typedef _1D<_dir0> super;
        struct node_fields : _1D<_dir0>::node_fields {};

        template <unsigned int _depth> 
        struct depth {
          typedef depth_to_for2d<_dir0,_dir,_depth> to;
        };
      
        /** The number of spatial dimensions. */
        static const unsigned int spatial_dimensions = 2u;
        /** The 2nd dimension direction */
        static const unsigned int dir1 = _dir;
      
      
        _2D() {}
        template <class T> _2D(const T & t) {}
      
        inline int getNumberValues() const { return 4; }
        template<class _Particle>
        inline int operator()(const _Particle & p) const {
          return super::operator()(p) + 2*(p.x[_dir] >= super::pivot[_dir]);
        }
      };
    }
  }
}

#endif // olson_tools_nsort_map__2D_h

