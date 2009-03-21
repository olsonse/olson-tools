#ifndef olson_tools_nsort_map_get_dims_h
#define olson_tools_nsort_map_get_dims_h

#include <boost/static_assert.hpp>

namespace olson_tools {
  namespace nsort {
    namespace map {

      template < typename _Dimensions, unsigned int ndims = _Dimensions::ndims >
      struct get_dims {
        BOOST_STATIC_ASSERT(sizeof(_Dimensions) == 0);
      };

      template < typename _Dimensions >
      struct get_dims<_Dimensions,1u> {
        typedef _1D< _Dimensions::dir0 > type;
      };

      template < typename _Dimensions >
      struct get_dims<_Dimensions,2u> {
        typedef _2D< _Dimensions::dir0, _Dimensions::dir1 > type;
      };

      template < typename _Dimensions >
      struct get_dims<_Dimensions,3u> {
        typedef _3D< _Dimensions::dir0, _Dimensions::dir1, _Dimensions::dir2 > type;
      };
    }
  }
}

#endif // olson_tools_nsort_map_get_dims_h
