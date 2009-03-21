#define BOOST_TEST_MODULE  ref_of

#include <olson-tools/ref_of.h>

#include <boost/test/unit_test.hpp>
#include <iostream>


BOOST_AUTO_TEST_SUITE( ref_of );

BOOST_AUTO_TEST_CASE( ref_deref ) {
  int a = 10;
  int & b = a;
  int & c = olson_tools::ref_of(a);
  int & d = olson_tools::ref_of(b);

  BOOST_CHECK_EQUAL( c, d );
}

BOOST_AUTO_TEST_CASE( pointer_deref ) {
  int a = 10;
  int * a_ptr = &a;
  int & b = olson_tools::ref_of(a_ptr);
  int & c = olson_tools::ref_of(&a);

  BOOST_CHECK_EQUAL( b, c );
}

BOOST_AUTO_TEST_SUITE_END();

