

#ifndef olson_tools_nsort_map_w_type_h
#define olson_tools_nsort_map_w_type_h


#include <olson-tools/nsort/map/type.h>

#include <olson-tools/IteratorRange.h>

namespace olson_tools {
  namespace nsort {
    namespace map {
      namespace tag {
        struct w_type {};
      }

      template <class T>
      struct w_type : T {
        typedef tag::w_type tag;
        typedef T super;
        const int n_types;

        w_type(const int & n_types) : n_types(n_types) {}
        template <class TT> w_type(const TT & tt) : T(tt) {}

        inline int getNumberValues() const {
          return T::getNumberValues() * n_types;
        }

        /** Map operation used when performing sorting. */
        template<class _Particle>
        inline int operator()(const _Particle & p) const {
          return n_types * T::operator()(p) + p.type;
        }

      };/*struct w_type */
    }/*namespace map */
  }/*namespace nsort */
}/*namespace olson_tools */


#endif // olson_tools_nsort_map_w_type_h

