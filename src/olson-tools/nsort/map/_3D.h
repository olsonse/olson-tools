
#ifndef olson_tools_nsort_map__3D_h
#define olson_tools_nsort_map__3D_h

#include <olson-tools/nsort/map/_2D.h>
#include <olson-tools/nsort/map/depth_to_for3d.h>

namespace olson_tools {
  namespace nsort {
    namespace map {
      namespace tag {
        /** Simple tag class that can be used to detect this map. */
        template <unsigned int dir0, unsigned int dir1, unsigned int dir2>
        struct _3D {};
      }

      /** maps x[_dir0],x[_dir1],x[_dir2] to 0,1,2,3,4,5,6,7 */
      template <unsigned int _dir0, unsigned int _dir1, unsigned int _dir>
      struct _3D : _2D<_dir0,_dir1> {
        /* TYPEDEFS */
      private:
        typedef _2D<_dir0,_dir1> twoD;
      public:
        typedef tag::_3D<_dir0,_dir1,_dir> tag;
        typedef void super;

        template <unsigned int _depth> 
        struct depth {
          typedef depth_to_for3d<_dir0,_dir1,_dir,_depth> to;
        };
      
        /** The number of spatial dimensions. */
        static const unsigned int spatial_dimensions = 3u;
        /** The 3rd dimension direction */
        static const unsigned int dir2 = _dir;

      
        _3D() {}
        template <class T> _3D(const T & t) {}
      
        inline int getNumberValues() const { return 8; }
        template<class _Particle>
        inline int operator()(const _Particle & p) const {
          return twoD::operator()(p) + 4*(p.x[_dir] >= twoD::pivot[_dir]);
        }
      };
    }
  }
}

#endif // olson_tools_nsort_map__3D_h

