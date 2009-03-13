
#ifndef olson_tools_nsort_map__1D_h
#define olson_tools_nsort_map__1D_h

#include <olson-tools/nsort/map/position_base.h>
#include <olson-tools/nsort/map/depth_to_for1d.h>

#include <olson-tools/IteratorRange.h>

namespace olson_tools {
  namespace nsort {
    namespace map {
      /** maps x<pivot to 0 and x>= pivot to 1.
       * This sort-map <i>does</i> dictate that new children are to be created,
       * provided the given set of rules (supplied by info) are followed.  
       * */
      template <unsigned int _dir>
      struct _1D : pivot {
        /* TYPEDEFS */
        struct node_fields {};

        template <unsigned int _depth> 
        struct depth {
          typedef depth_to_for1d<_dir,_depth> to;
        };
      
        /** The number of spatial dimensions. */
        static const unsigned int spatial_dimensions = 1u;
        /** The 1st dimension direction */
        static const unsigned int dir0 = _dir;


        _1D() {}
        template <class T> _1D(const T & t) {}
      
        inline int getNumberValues() const { return 2; }
        template<class _Particle>
        inline int operator()(const _Particle & p) const {
          return p.x[_dir] >= pivot[_dir];
        }
      };
    }
  }/*namespace nsort */
}/*namespace olson_tools */

#endif // olson_tools_nsort_map__1D_h
