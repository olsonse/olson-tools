
#ifndef olson_tools_nsort_map__2D_h
#define olson_tools_nsort_map__2D_h

#include <olson-tools/nsort/map/_1D.h>
#include <olson-tools/nsort/map/depth_to_for2d.h>

namespace olson_tools {
  namespace nsort {
    namespace map {
      template <unsigned int _dir0, unsigned int _dir>
      struct _2D : _1D<_dir0> {
        struct node_fields : _1D<_dir0>::node_fields {};

        template <unsigned int _depth> 
        struct depth {
          typedef depth_to_for2d<_dir0,_dir,_depth> to;
        };
      
        /** The number of spatial dimensions. */
        static const unsigned int spatial_dimensions = 2u;
      
        typedef _1D<_dir0> super;
      
        _2D() {}
        template <class T> _2D(const T & t) {}
      
        inline int getNumberValues() const { return 4; }
        inline int dir1() const { return _dir; }
        template<class _Particle>
        inline int operator()(const _Particle & p) const {
          return super::operator()(p) + 2*(p.x[_dir] >= super::pivot[_dir]);
        }
      };
    }
  }
}

#endif // olson_tools_nsort_map__2D_h

