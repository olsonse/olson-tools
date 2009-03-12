

#ifndef olson_tools_nsort_map_ptr_h
#define olson_tools_nsort_map_ptr_h

namespace olson_tools {
  namespace nsort {
    namespace map {
      /** Map to allow the mapping of a set of pointers to objects.  This class
       * is intended to wrap around the real map class and provides the
       * dereference of pointers to objects so that the real map classes can
       * operate without modification.
       * */
      template <class T>
      struct ptr : T {
        struct node_fields : T::node_fields {};

        typedef T super;

        ptr() {}
        template <class TT> ptr(const TT & tt) : T(tt) {}

        template<class _Particle>
        inline int operator()(const _Particle * p) const {
          return T::operator()(*p);
        }

        template <class sorter, class Info>
        void getIterators(const sorter & s, Info & info) {
          T::getIterators(s,info);
        }
      };
    }/*namespace map */
  }/*namespace nsort */
}


#endif // olson_tools_nsort_map_ptr_h

