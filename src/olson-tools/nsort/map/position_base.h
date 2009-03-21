#ifndef olson_tools_nsort_map_position_base_h
#define olson_tools_nsort_map_position_base_h

#include <olson-tools/Vector.h>
#include <olson-tools/IteratorRange.h>

namespace olson_tools {
  namespace nsort {
    namespace map {
      struct pivot {
        Vector<double,3> pivot;
      };

      namespace tag {
        /** Simple tag class that can be used to detect this map. */
        struct pivot_ctor {};
      }

      /** A simple wrapper for position maps (types that have a pivot fields)
       * that provides a constructor to assign the pivot fields regardless of
       * how many layers of wrappers there are.  Because this wrapper is for
       * providing constructors, it should be used as the outer wrapper class
       * for positional maps.
       *
       * some various example maps that can be used:<br>
       *  - Providing constructors for _1D map of value types:<br>
       *    \verbatim
              pivot_ctor<              _1D<0    > >     map1D;
            \endverbatim
       *  - Providing constructors for _2D map of value types:<br>
       *    \verbatim
              pivot_ctor<              _2D<0,1  > >     map2D;
            \endverbatim
       *  - Providing constructors for _3D map of value types:<br>
       *    \verbatim
              pivot_ctor<              _3D<0,1,2> >     map3D;
            \endverbatim
       *  - Providing constructors for _1D map of pointer types:<br>
       *    \verbatim
              pivot_ctor< ptr<         _1D<0    > > >   ptrmap1D;
            \endverbatim
       *  - Providing constructors for _2D map of pointer types:<br>
       *    \verbatim
              pivot_ctor< ptr<         _2D<0,1  > > >   ptrmap2D;
            \endverbatim
       *  - Providing constructors for _3D map of pointer types:<br>
       *    \verbatim
              pivot_ctor< ptr<         _3D<0,1,2> > >   ptrmap3D;
            \endverbatim
       *  - Providing constructors for map::type and map::_1D map of value types:<br>
       *    \verbatim
              pivot_ctor<      w_type< _1D<0    > > >   map1Dtype(10);
            \endverbatim
       *  - Providing constructors for map::type and map::_2D map of value types:<br>
       *    \verbatim
              pivot_ctor<      w_type< _2D<0,1  > > >   map2Dtype(10);
            \endverbatim
       *  - Providing constructors for map::type and map::_3D map of value types:<br>
       *    \verbatim
              pivot_ctor<      w_type< _3D<0,1,2> > >   map3Dtype(10);
            \endverbatim
       *  - Providing constructors for map::type and map::_2D map of pointer types:<br>
       *    \verbatim
              pivot_ctor< ptr< w_type< _1D<0    > > > > ptrmap1Dtype(10);
            \endverbatim
       *  - Providing constructors for map::type and map::_2D map of pointer types:<br>
       *    \verbatim
              pivot_ctor< ptr< w_type< _2D<0,1  > > > > ptrmap2Dtype(10);
            \endverbatim
       *  - Providing constructors for map::type and map::_2D map of pointer types:<br>
       *    \verbatim
              pivot_ctor< ptr< w_type< _3D<0,1,2> > > > ptrmap3Dtype(10);
            \endverbatim
      */
      template <class DIMS>
      struct pivot_ctor : DIMS {
        typedef tag::pivot_ctor tag;
        typedef DIMS super;

        /* ** BEGIN CONSTRUCTORS ** */
        pivot_ctor( const double & px = 0.0,
                    const double & py = 0.0,
                    const double & pz = 0.0) {
          DIMS::pivot = V3(px,py,pz);
        }
      
        pivot_ctor(const Vector<double,3> & pivot) {
          DIMS::pivot = pivot;
        }
      
        template <class Info>
        pivot_ctor(const Info & info, const Vector<double,3> & pivot = 0.0) : DIMS(info) {
          DIMS::pivot = pivot;
        }
        /* ** END CONSTRUCTORS ** */
      };
      
    }/* namespace map*/
  }/* namespace nsort */
}/* namespace olson_tools */

#endif // olson_tools_nsort_map_position_base_h
