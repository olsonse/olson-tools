

#ifndef olson_tools_nsort_map_identity_h
#define olson_tools_nsort_map_identity_h

namespace olson_tools {
  namespace nsort {
    namespace map {
      namespace tag {
        /** Simple tag class that can be used to detect this map. */
        struct identity {};
      }

     /** Identity nsort_map wrapper.  Useful as a default parameter in a templated
      * nsort::map wrap.
      * */
      template <class T>
      struct identity : T {
        typedef tag::identity tag;
        typedef T super;

        identity() {}
        template <class TT> identity(const TT & tt) : T(tt) {}
      };
    }/*namespace map */
  }/*namespace nsort */
}/* namespace olson_tools */


#endif // olson_tools_nsort_map_identity_h

