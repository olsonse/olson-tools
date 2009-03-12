#ifndef olson_tools_nsort_direct_h
#define olson_tools_nsort_direct_h

namespace olson_tools {
  namespace nsort {
    namespace map {

      /** maps x<pivot to 0 and x>= pivot to 1. */
      struct direct {
        template <class T>
        inline const int & operator()(const T & t) const {
          return t;
        }
      };
    }/* namespace map */
  }/* namespace nsort */
}/* namespace olson_tools */

#endif // olson_tools_nsort_direct_h
