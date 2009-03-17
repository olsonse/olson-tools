
#ifndef olson_tools_nsort_map_remap_base_h
#define olson_tools_nsort_map_remap_base_h

#include <olson-tools/nsort/map/_3D.h>

#include <set>

namespace olson_tools {
  namespace nsort {
    namespace map {
      namespace tag {
        struct remap_base {};
      }

      /** Re-maps an underlying map function onto a different set of mapped
       * values.
       * An example use of this class is to remap a portion of the domain of the
       * underlying map function onto another portion of the map function's
       * domain.
       * */
      template < typename T , unsigned int _nval = 0u >
      struct remap_base : T {
        typedef tag::remap_base tag;
        typedef T super;

        /* STORAGE MEMBER(S) */
        /** Outside access to the maximum number of possible values. */
        static const unsigned int nval = _nval;

        /** The remap map. */
        int m_remap[nval];
      

        /* FUNCTION MEMBERS */
        /** Default constructor calls 'reset'.
         * @see reset. */
        remap_base() {
          reset();
        }

        /** Single arg constructor required by wrapping components.*/
        template <class TT>
        remap_base(const TT & tt) : T(tt) {
          reset();
        }

        /** Resets the m_remap[] array to 1..nval. */
        void reset() {
          for (unsigned int i = 0; i < nval; ++i)
            m_remap[i] = i;
        }

        /** Returns the number of unique values.  This function cannot be
         * optimized away during compilation because it relies on runtime
         * calculations.
         * */
        inline int getNumberValues() const {
          /** \todo Is there a way to make this not rely on heap allocation? */
          std::set<int> s(m_remap, m_remap+nval);
          return s.size();
        }

        /** Actual remap operation used when performing sorting. */
        template<class _Particle>
        inline int operator()(const _Particle & p) const {
          return m_remap[T::operator()(p)];
        }
      };


      template < typename T >
      struct remap : remap_base< T > {
        static const int USING_NON_SPECIALIZATION = T::IS_NOT_ALLOWED;

        /** Default constructor required by wrapping components.
         * Specializations must implement this. */
        remap() {}
        /** Single arg constructor required by wrapping components.
         * Specializations must implement this. */
        template <class TT> remap(const TT & tt) : T(tt) {}
      };
    }
  }
}

#endif // olson_tools_nsort_map_remap_base_h

