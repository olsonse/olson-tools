
BOOST_AUTO_TEST_CASE( map_1D_w_type ) {
  using olson_tools::nsort::map::_1D;
  using olson_tools::nsort::map::pivot_ctrs;
  using olson_tools::nsort::map::w_type;
  typedef pivot_ctrs< w_type< _1D<0> > > map_t;
  map_t map(1,V3(0,0,0));


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

BOOST_AUTO_TEST_CASE( remap_1D_w_type ) {
  using olson_tools::nsort::map::remap;
  using olson_tools::nsort::map::_1D;
  using olson_tools::nsort::map::pivot_ctrs;
  using olson_tools::nsort::map::w_type;
  typedef pivot_ctrs< w_type< remap< _1D<0> > > > map_t;
  map_t rmap(1,V3(0,0,0));

  Particle p;
  p.x[0] = -1;
  BOOST_CHECK_EQUAL( rmap(p), 0 );

  p.x[0] =  1;
  BOOST_CHECK_EQUAL( rmap(p), 1 );
}

