#ifndef OLSON_TOOLS_NSORT_H
#define OLSON_TOOLS_NSORT_H


#include <string.h>
#include <ostream>


namespace olson_tools {

/** maps x<pivot to 0 and x>= pivot to 1. */
struct direct_map {
    template <class T>
    inline const int & operator()(const T & t) const {
        return t;
    }
};

/** 
 *  This is not necessarily a stable sort.
 * @param n_values
 *     The number of integer values expected in the list to sort.
 *
 * Note that this function does NOT check for overruns in the expected values.
 */
template <class val_map = direct_map>
class nsort {
  private:
    struct binval {
        int end;
        int ptr;
    };
    int n_values;
    binval * bin;

  public:
    nsort(const int & n_values) : n_values (n_values) {
        bin = new binval[n_values];
    }

    ~nsort() {
        delete[] bin;
    }

    /** Get the number of bins/values used in this sort. */
    inline const int & size() const { return n_values; }

    /** Obtain the index of the end() element of the ith value.
     * Note that i should conform to 0 <= i < n_values; there is no bound
     * checking on the input i.
     *
     * Only valid after sort(...) has been called. */
    inline const int & end(const int & i) const { return bin[i].end; }

    /** Obtain the index of the begin() element of the ith value.
     * Note that i should conform to 0 <= i < n_values; there is no bound
     * checking on the input i.
     *
     * Only valid after sort(...) has been called. */
    inline int begin(const int & i) const {
        if (i == 0) return 0;
        else return bin[i-1].end;
    }

    /** Get the number of items for a particular value index.
     * This function just computes <code>(end(i) - begin(i))</code>.
     *
     * Only valid after sort(...) has been called. */
    inline int size(const int & i) const { return end(i) - begin(i); }

    template <class Iter>
    void sort(const Iter & Ai, const Iter & Af, const val_map & map = val_map()) {
        memset(bin, 0, sizeof(binval)*n_values);

        /* first count the number of occurrences for each value. */
        for (Iter i = Ai; i < Af; ++i)
            bin[map(*i)].end++;

        /* now change this array of occurrences to an array of start
         * positions. */
        for (int i = 0, cur_ptr = 0; i < n_values; ++i) {
            bin[i].ptr   = cur_ptr;
            cur_ptr += bin[i].end;
            bin[i].end   = cur_ptr;
        }

        for (int i = 0; i < n_values; ++i) {
            const int & end_pos = end(i);
            int & pos = bin[i].ptr;

            while (pos < end_pos) {
                int & pos2 = bin[map(*(Ai + pos))].ptr;

                if (pos == pos2) {
                    /* don't need to swap current position, move to next... */
                    pos++;
                    continue;
                }
                std::iter_swap(Ai+pos, Ai + pos2++);
            }
        }
    }
};

}

#endif // OLSON_TOOLS_NSORT_H
