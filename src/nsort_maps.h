#ifndef OLSON_TOOLS_NSORT_MAPS_H
#define OLSON_TOOLS_NSORT_MAPS_H

#include "Vector.h"
#include "IteratorRange.h"

namespace olson_tools {

/* *** BEGIN PARTICLE MAPS FOR NSORT ALGORITHM */
/** maps particle to particle.type.
 * This sort-map does <b>not</b> dictate that new children are to be created.
 * */
struct type_map {
    template<class _Particle>
    inline int operator()(const _Particle & p) const {
        return p.type;
    }

    /** Obtain iterators for type_map values.
     * This version allows this function to be called from another map that
     * sorts on type for it fastest index. */
    template <class sorter, class Node>
    static void getIterators(const int & b, const int & e, const sorter & s, Node & node) {
        typedef typename Node::particle_iter_type Iter;
        node.types.resize(e - b);
        for (int i = b; i < e; i++) {
            node.types[i] = IteratorRange<Iter>(
                node.particles.begin() + s.begin(i),
                node.particles.begin() + s.end(i)
            );
        }
    }

    /** Obtain iterators for type_map values. */
    template <class sorter, class Node>
    static void getIterators(const sorter & s, Node & node) {
        getIterators(0, s.size(), s, node);
    }
};



struct map_pivot {
    olson_tools::Vector<double,3> pivot;
};

template <class DIMS>
struct position_map : DIMS {
    typedef olson_tools::Vector<double,3> Vector;

    /* ** BEGIN CONSTRUCTORS ** */
    position_map(const double & px = 0.0,
                 const double & py = 0.0,
                 const double & pz = 0.0) {
        DIMS::pivot = V3(px,py,pz);
    }

    position_map(const Vector & pivot) {
        DIMS::pivot = pivot;
    }

    template <class Info>
    position_map(const Info & info, const Vector & pivot = 0.0) : DIMS(info) {
        DIMS::pivot = pivot;
    }
    /* ** END CONSTRUCTORS ** */
};

/** maps x<pivot to 0 and x>= pivot to 1.
 * This sort-map <i>does</i> dictate that new children are to be created,
 * provided the given set of rules (supplied by info) are followed.  
 * */
template <unsigned int dir>
struct _1D : map_pivot {
    _1D() {}
    template <class T> _1D(const T & t) {}

    inline int getNumberValues() const {
        return 2;
    }

    inline int dir0() const { return dir; }

    template<class _Particle>
    inline int operator()(const _Particle & p) const {
        return p.x[dir] >= pivot[dir];
    }

    template <class sorter, class Info>
    void getIterators(const sorter & s, Info & info) {
        typedef typename Info::particle_iter_type Iter;
        if (!info.rules.pass(s)) {
            /* do not create any children; retain all particles within this
             * node. */
            info.children = 0;
            info.leaves = 1;
            return;
        }

        info.particles = IteratorRange<Iter>(
            info.particles.begin() + s.begin(0),
            info.particles.begin() + s.end(1)
        );
    }
};

template <unsigned int dir0, unsigned int dir>
struct _2D : _1D<dir0> {
    typedef _1D<dir0> super;

    _2D() {}
    template <class T> _2D(const T & t) {}

    inline int getNumberValues() const { return 4; }
    inline int dir1() const { return dir; }
    template<class _Particle>
    inline int operator()(const _Particle & p) const {
        return super::operator()(p) + 2*(p.x[dir] >= super::pivot[dir]);
    }
};

/** maps x[dir0],x[dir1],x[dir2] to 0,1,2,3,4,5,6,7 */
template <unsigned int dir0, unsigned int dir1, unsigned int dir>
struct _3D : _2D<dir0,dir1> {
    typedef _2D<dir0,dir1> super;

    _3D() {}
    template <class T> _3D(const T & t) {}

    inline int getNumberValues() const { return 8; }
    inline int dir2() const { return dir; }
    template<class _Particle>
    inline int operator()(const _Particle & p) const {
        return super::operator()(p) + 4*(p.x[dir] >= super::pivot[dir]);
    }
};

template <class T>
struct _ptr : T {
    typedef T super;

    _ptr() {}
    template <class TT> _ptr(const TT & tt) : T(tt) {}

    template<class _Particle>
    inline int operator()(const _Particle * p) const {
        return T::operator()(*p);
    }

    template <class sorter, class Info>
    void getIterators(const sorter & s, Info & info) {
        T::getIterators(s,info);
    }
};

template <class T>
struct _w_type : T {
    typedef T super;
    const int n_types;

    _w_type(const int & n_types) : n_types(n_types) {}
    template <class TT> _w_type(const TT & tt) : T(tt) {}

    inline int getNumberValues() const {
        return T::getNumberValues() * n_types;
    }

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

        type_map::getIterators(0, s.size(), s, info);
    }
};

template <class T>
struct _identity : T {
    _identity() {}
    template <class TT> _identity(const TT & tt) : T(tt) {}
};

/* some various example maps that can be used
   position_map<               _1D<0    > >     map1D;
   position_map<               _2D<0,1  > >     map2D;
   position_map<               _3D<0,1,2> >     map3D;
   position_map< _ptr<         _1D<0    > > >   ptrmap1D;
   position_map< _ptr<         _2D<0,1  > > >   ptrmap2D;
   position_map< _ptr<         _3D<0,1,2> > >   ptrmap3D;
   position_map<       _w_type<_1D<0    > > >   map1Dtype(10);
   position_map<       _w_type<_2D<0,1  > > >   map2Dtype(10);
   position_map<       _w_type<_3D<0,1,2> > >   map3Dtype(10);
   position_map< _ptr< _w_type<_1D<0    > > > > ptrmap1Dtype(10);
   position_map< _ptr< _w_type<_2D<0,1  > > > > ptrmap2Dtype(10);
   position_map< _ptr< _w_type<_3D<0,1,2> > > > ptrmap3Dtype(10);
*/

} /* namespace olson_tools */

#endif // OLSON_TOOLS_NSORT_MAPS_H
