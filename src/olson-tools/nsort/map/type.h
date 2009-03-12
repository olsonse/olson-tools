

#ifndef olson_tools_nsort_map_type_h
#define olson_tools_nsort_map_type_h


#include <olson-tools/IteratorRange.h>

#include <vector>

namespace olson_tools {
  namespace nsort {
    namespace map {
      /** maps particle to particle.type.
       * This sort-map does <b>not</b> dictate that new children are to be created.
       * */
      struct type {
        /** Fields that must be inherited by a node type class that wants to use
         * map::type sorting and subsequently use the getIterators function to
         * store the results. */
        template <class Iter>
        struct node_fields {
          node_fields (const int & n =  0) {
            types.resize(n);
          }
        
          /* BEGIN STORAGE MEMBERS */
          std::vector< IteratorRange<Iter> > types; /* size : n */
        };
   
        template<class _Particle>
        inline int operator()(const _Particle & p) const {
          return p.type;
        }
   
        /** Obtain iterators for map::type values.
         * This version allows this function to be called from another map that
         * sorts on type for it fastest index. */
        template <class sorter, class Node>
        static void getIterators( const int & b,
                                  const int & e,
                                  const sorter & s, Node & node) {
          typedef typename Node::particle_iter_type Iter;
          node.types.resize(e - b);
          for (int i = b; i < e; i++) {
            node.types[i] = IteratorRange<Iter>(
              node.particles.begin() + s.begin(i),
              node.particles.begin() + s.end(i)
            );
          }
        }
   
        /** Obtain iterators for map::type values. */
        template <class sorter, class Node>
        static void getIterators(const sorter & s, Node & node) {
          getIterators(0, s.size(), s, node);
        }
      };/*struct type */
    }/*namespace map */
  }/*namespace nsort */
}/*namespace olson_tools */


#endif // olson_tools_nsort_map_type_h

