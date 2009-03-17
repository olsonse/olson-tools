#ifndef olson_tools_nsort_direct_h
#define olson_tools_nsort_direct_h

namespace olson_tools {
  namespace nsort {
    namespace map {
      namespace tag {
        /** Simple tag class that can be used to detect this map. */
        struct direct {};
      }

      /** maps x<pivot to 0 and x>= pivot to 1. */
      struct direct {
        typedef tag::direct tag;
        typedef void super;

        template <class T>
        inline const int & operator()(const T & t) const {
          return t;
        }
      };
    }/* namespace map */
  }/* namespace nsort */
}/* namespace olson_tools */

#endif // olson_tools_nsort_direct_h
