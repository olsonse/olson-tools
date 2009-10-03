#ifndef olson_tools_nsort_tweak_Null_h
#define olson_tools_nsort_tweak_Null_h

namespace olson_tools {
  namespace nsort {
    namespace tweak {
      /** Default NSort tweaker does nothing. */
      struct Null {
        template < typename Map >
        inline void tweakNSort( Map & map,
                                const int * const bin,
                                const int & n_values ) const {}
      };
    }/* namespace tweak */
  }/* namespace nsort */
}/* namespace olson_tools */

#endif // olson_tools_nsort_tweak_Null_h
