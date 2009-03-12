

#ifndef olson_tools_nsort_map_w_type_h
#define olson_tools_nsort_map_w_type_h


#include <olson-tools/nsort/map/type.h>

#include <olson-tools/IteratorRange.h>

namespace olson_tools {
  namespace nsort {
    namespace map {
      template <class T>
      struct w_type : T {
        /** Fields that must be inherited by a node type class that wants to use
         * map::type sorting and subsequently use the getIterators function to
         * store the results. */
        template <class Iter>
        struct node_fields : T::node_fields, map::type::node_fields<Iter> {};

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

        template <class sorter, class Info>
        static void getIterators(const sorter & s, Info & info) {
          typedef typename Info::particle_iter_type Iter;

          info.particles = IteratorRange<Iter>(
            info.particles.begin() + s.begin(0),
            info.particles.begin() + s.end(1)
          );

          map::type::getIterators(0, s.size(), s, info);
        }
      };/*struct w_type */
    }/*namespace map */
  }/*namespace nsort */
}/*namespace olson_tools */


#endif // olson_tools_nsort_map_w_type_h

