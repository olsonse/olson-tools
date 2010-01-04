#include <olson-tools/Vector.h>
#include <algorithm>
/* This is the partition function (partial quicksort) used currently in the
 * octree sort implementation. */

template <unsigned int dir, class Iter>
inline Iter partition(const Iter & Ai, const Iter & Af, const double & pivot) {
    int swaps = 0;
    Iter left = Ai, right = Af-1;
    while ( left < right ) {
	/* Move left while item < pivot */
	while( left < right && left->x[dir] <= pivot ) left++;

	/* Move right while item > pivot */
	while( left < right && right->x[dir] > pivot ) right--;

	if ( left < right ) {
            std::iter_swap(left,right);
            swaps++;
        }
    }

    /* if there have been no swaps, then we need to determine whether it was
     * already sorted. */
    if (swaps || (Ai->x[dir] <= pivot && (Af-1)->x[dir] > pivot))
        return right;
    else {
        /* didn't have to sort because everything was on one side of the
         * partition. */
        return Ai;
    }
}

const Particle & dref(const Particle & p) { return p; }
const Particle & dref(const Particle * p) { return *p; }

template<class T, unsigned int dir>
struct partition_position_pred {
    const olson_tools::Vector<double,3> & pivot;
    partition_position_pred(const olson_tools::Vector<double,3> & pivot = 0.0)
        : pivot(pivot) {}
    inline bool operator() (const T & p) {
        return dref(p).x[dir] < pivot[dir];
    }
};

/* Only useful for exactly two particle types. */
struct partition_species_pred {
    inline bool operator() (const Particle & p) {
        return p.species == 0;
    }
};

template <unsigned int dirx, unsigned int diry, class Iter>
inline void quadpartition(const Iter & Ai, const Iter & Af,
                          const olson_tools::Vector<double,3> & p = 0.0) {
    typedef typename Iter::value_type T;
    /* first do dirx direction. */
    Iter m = std::partition(Ai, Af, partition_position_pred<T,diry>(p));
    /* now do the diry direction. */
    std::partition(Ai, m, partition_position_pred<T,dirx>(p));
    std::partition(m, Af, partition_position_pred<T,dirx>(p));
}

template <
    unsigned int dirx,
    unsigned int diry,
    unsigned int dirz,
    class Iter
>
inline void octpartition(const Iter & Ai, const Iter & Af,
                         const olson_tools::Vector<double,3> & p = 0.0) {
    typedef typename Iter::value_type T;
    /* first do dirz direction. */
    Iter m = std::partition(Ai, Af, partition_position_pred<T,dirz>(p));
    /* now do the dirx,diry directions. */
    quadpartition<dirx,diry>(Ai, m, p);
    quadpartition<dirx,diry>(m, Af, p);
}

