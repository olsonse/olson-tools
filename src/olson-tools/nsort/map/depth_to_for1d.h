#ifndef olson_tools_nsort_map_depth_to_for1d_h
#define olson_tools_nsort_map_depth_to_for1d_h

namespace olson_tools {
  namespace nsort {
    namespace map {

      template < unsigned int _dir0,
                 unsigned int _depth >
      struct depth_to_for1d {
        static const unsigned int dir;
        static const unsigned int rdir;
      };

      template < unsigned int dir0 >
      struct depth_to_for1d<dir0,0u> {
        static const unsigned int dir = dir0;
        static const unsigned int rdir = dir0;
      };

    }
  }
}

#endif // olson_tools_nsort_map_depth_to_for1d_h
