
#ifndef olson_tools_nsort_map__3D_h
#define olson_tools_nsort_map__3D_h

#include <olson-tools/nsort/map/_2D.h>
#include <olson-tools/nsort/map/depth_to_for3d.h>

namespace olson_tools {
  namespace nsort {
    namespace map {
      /** maps x[_dir0],x[_dir1],x[_dir2] to 0,1,2,3,4,5,6,7 */
      template <unsigned int _dir0, unsigned int _dir1, unsigned int _dir>
      struct _3D : _2D<_dir0,_dir1> {
        struct node_fields : _2D<_dir0,_dir1>::node_fields {};

        template <unsigned int _depth> 
        struct depth {
          typedef depth_to_for3d<_dir0,_dir1,_dir,_depth> to;
        };
      
        /** The number of spatial dimensions. */
        static const unsigned int spatial_dimensions = 3u;
      
        typedef _2D<_dir0,_dir1> super;
      
        _3D() {}
        template <class T> _3D(const T & t) {}
      
        inline int getNumberValues() const { return 8; }
        inline int dir2() const { return _dir; }
        template<class _Particle>
        inline int operator()(const _Particle & p) const {
          return super::operator()(p) + 4*(p.x[_dir] >= super::pivot[_dir]);
        }
      };
    }
  }
}

#endif // olson_tools_nsort_map__3D_h

