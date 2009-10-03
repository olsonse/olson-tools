
#ifndef olson_tools_nsort_map__2D_h
#define olson_tools_nsort_map__2D_h

#include <olson-tools/nsort/map/_1D.h>
#include <olson-tools/nsort/map/depth_to_for2d.h>

namespace olson_tools {
  namespace nsort {
    namespace map {
      namespace tag {
        /** Simple tag class that can be used to detect this map. */
        template <unsigned int dir0, unsigned int dir1>
        struct _2D {};
      }

      /** maps x[_dir0],x[_dir1] to 0,1,2,3 */
      template <unsigned int _dir0, unsigned int _dir>
      struct _2D : _1D<_dir0> {
        /* TYPEDEFS */
      private:
        typedef _1D<_dir0> oneD;
      public:
        typedef tag::_2D<_dir0,_dir> tag;

        typedef void super;

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
          return oneD::operator()(p) + 2*(p.x[_dir] >= oneD::pivot[_dir]);
        }
      };
    }
  }
}

#endif // olson_tools_nsort_map__2D_h

