#ifndef olson_tools_nsort_NSort_h
#define olson_tools_nsort_NSort_h

#include <olson-tools/nsort/map/direct.h>
#include <olson-tools/nsort/tweak/Null.h>
#include <olson-tools/ref_of.h>

#include <string.h>
#include <ostream>

namespace olson_tools {
  namespace nsort {

    /** 
     *  This is not necessarily a stable sort.
     * @param n_values
     *     The number of integer values expected in the list to sort.
     *
     * Note that this function does NOT check for overruns in the expected values.
     */
    template < typename val_map = map::direct,
               typename NSortTweaker = tweak::Null >
    class NSort {
    private:
      int n_values;
      int * bin;

    public:
      NSort(const int & n_values) : n_values (n_values) {
        bin = new int[n_values];
      }

      ~NSort() {
        delete[] bin;
      }

      /** Get the number of bins/values used in this sort. */
      inline const int & size() const { return n_values; }

      /** Obtain the index of the end() element of the ith value.
       * Note that i should conform to 0 <= i < n_values; there is no bound
       * checking on the input i.
       *
       * Only valid after sort(...) has been called. */
      inline const int & end(const int & i) const { return bin[i]; }

      /** Obtain the index of the begin() element of the ith value.
       * Note that i should conform to 0 <= i < n_values; there is no bound
       * checking on the input i.
       *
       * Only valid after sort(...) has been called. */
      inline int begin(const int & i) const {
        if (i == 0) return 0;
        else return bin[i-1];
      }

      /** Get the number of items for a particular value index.
       * This function just computes <code>(end(i) - begin(i))</code>.
       *
       * Only valid after sort(...) has been called. */
      inline int size(const int & i) const { return end(i) - begin(i); }

      template <class Iter>
      void sort(const Iter & Ai, const Iter & Af,
                const val_map & map = val_map(),
                const NSortTweaker & nsortTweaker = NSortTweaker()) {
        val_map mapcopy = map;
        NSortTweaker tweakcopy = nsortTweaker;
        sort(Ai,Af,mapcopy,tweakcopy);
      }

      template <class Iter>
      void sort(const Iter & Ai, const Iter & Af,
                val_map & map, NSortTweaker & nsortTweaker ) {
        int * ptr = new int[n_values];

        memset(bin, 0, sizeof(int)*n_values);
        memset(ptr, 0, sizeof(int)*n_values);

        /* first count the number of occurrences for each value. */
        for (Iter i = Ai; i < Af; ++i)
          ++bin[map(ref_of(*i))];

        /* Allow user code to tweak the map according to the preliminary
         * counting statistics. */
        nsortTweaker.tweakNSort(map, bin, static_cast<const int&>(n_values));

        /* now change this array of occurrences to an array of start
         * positions. */
        for (int i = 0, cur_ptr = 0; i < n_values; ++i) {
          ptr[i]   = cur_ptr;
          cur_ptr += bin[i];
          bin[i]   = cur_ptr;
        }

        for (int i = 0; i < n_values; ++i) {
          const int & end_pos = end(i);
          int & pos = ptr[i];

          while (pos < end_pos) {
            int & pos2 = ptr[map(ref_of(*(Ai + pos)))];

            if (pos == pos2) {
              /* don't need to swap current position, move to next... */
              pos++;
              continue;
            }
            std::iter_swap(Ai+pos, Ai + pos2++);
          }/*while*/
        }/*for*/

        delete[] ptr;
      }/*sort()*/
    };/* NSort class */

  }/* namespace nsort */
}/* namespace olson_tools */

#endif // olson_tools_nsort_NSort_h
