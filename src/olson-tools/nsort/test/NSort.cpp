#define BOOST_TEST_MODULE  NSort

#include <olson-tools/nsort/NSort.h>

#include <boost/test/unit_test.hpp>
#include <iostream>


BOOST_AUTO_TEST_SUITE( NSort );

BOOST_AUTO_TEST_CASE( c_array ) {
  const int len = 10;
  int v[len] = {1, 2, 0, 1, 2, 3, 0, 1, 2, 4};
  int ans[len] = {0, 0, 1, 1, 1, 2, 2, 2, 3, 4};
  olson_tools::nsort::NSort<> s(len);
  s.sort(static_cast<int*>(v), v+len);

  for (int i = 0; i < len; ++i)
    BOOST_CHECK_EQUAL( v[i], ans[i] );
}

BOOST_AUTO_TEST_CASE( std_vector ) {
  const int len = 10;
  int v[len] = {1, 2, 0, 1, 2, 3, 0, 1, 2, 4};
  int ans[len] = {0, 0, 1, 1, 1, 2, 2, 2, 3, 4};
  std::vector<int> sv(v, v+len);


  olson_tools::nsort::NSort<> s(len);
  s.sort(sv.begin(), sv.end());

  for (int i = 0; i < len; ++i)
    BOOST_CHECK_EQUAL( sv[i], ans[i] );
}

BOOST_AUTO_TEST_SUITE_END();

