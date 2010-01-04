#include <olson-tools/nsort/map/_1D.h>
#include <olson-tools/nsort/map/remap-_1D.h>
#include <olson-tools/nsort/map/w_species.h>
#include <olson-tools/Vector.h>

#define BOOST_TEST_MODULE  map_species

#include <boost/test/unit_test.hpp>

namespace {
  using olson_tools::Vector;
  using olson_tools::V3;
  struct Particle {
    typedef std::vector<Particle> list;
  
    Vector<double, 3> x;
    Vector<double, 3> v;
    unsigned int species;
  
    Particle(const Vector<double,3> & x = 0.0,
             const Vector<double,3> & v = 0.0,
             const unsigned int & species = 0 ) : x(x), v(v), species(species) {}
  };

  const unsigned int & species( const Particle & p ) {
    return p.species;
  }
  
  std::ostream & operator<< (std::ostream & out, const Particle & p) {
    return out << "{x: (" << p.x[0] << ", " << p.x[1] << ", " << p.x[2] << "), "
                   "v: (" << p.v[0] << ", " << p.v[1] << ", " << p.v[2] << "), "
                   "t: " << species(p) << "} ";
  }
}

BOOST_AUTO_TEST_CASE( map_1D_w_species ) {
  using olson_tools::nsort::map::_1D;
  using olson_tools::nsort::map::pivot_ctor;
  using olson_tools::nsort::map::w_species;
  typedef pivot_ctor< w_species< _1D<0> > > map_t;
  map_t map(1u,V3(0,0,0));


  /* make sure the dimensionality is correct. */
  int j = map.spatial_dimensions;
  int i = map_t::spatial_dimensions;
  BOOST_CHECK_EQUAL( i, 1 );
  BOOST_CHECK_EQUAL( j, 1 );


  Particle p;
  p.x[0] = -1;
  BOOST_CHECK_EQUAL( map(p), 0 );

  p.x[0] =  1;
  BOOST_CHECK_EQUAL( map(p), 1 );
}

BOOST_AUTO_TEST_CASE( remap_1D_w_species ) {
  using olson_tools::nsort::map::remap;
  using olson_tools::nsort::map::_1D;
  using olson_tools::nsort::map::pivot_ctor;
  using olson_tools::nsort::map::w_species;
  typedef pivot_ctor< w_species< remap< _1D<0> > > > map_t;
  map_t rmap(1u,V3(0,0,0));

  Particle p;
  p.x[0] = -1;
  BOOST_CHECK_EQUAL( rmap(p), 0 );

  p.x[0] =  1;
  BOOST_CHECK_EQUAL( rmap(p), 1 );
}

