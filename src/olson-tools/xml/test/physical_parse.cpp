#define BOOST_TEST_MODULE  XML_physical_parse

#include <olson-tools/xml/Doc.h>
#include <olson-tools/xml/physical_parse.h>
#include <olson-tools/strutil.h>

#include <physical/calc/Driver.h>

#include <boost/test/unit_test.hpp>

#include <ostream>
#include <sstream>

#ifndef XML_FILENAME
#  error The filename was supposed to already be defined on the command line
#endif

namespace {
  namespace xml = olson_tools::xml;
  using runtime::physical::Quantity;
  using olson_tools::to_string;
}

BOOST_AUTO_TEST_SUITE( XML_physical_parse_tests );//{

  BOOST_AUTO_TEST_CASE( parse ) {
    using runtime::physical::calc::Driver;
    Driver & calc = Driver::instance();
    /* clear the old symbols out */
    calc.symbols.clear();
    calc.addMathLib();
    calc.addPhysicalUnits();

    /* do imports to make things easier. */
    calc.exec( "from physical::constant import *;" );
    calc.exec( "from physical::unit import *;" );
    calc.exec( "from physical import 'unit::pi';" );

    xml::Doc doc(XML_FILENAME);

    xml::Context x = doc.find("//some-value");

    BOOST_CHECK_EQUAL( to_string( x.parse<Quantity>() ), "<10 m>" );
  }

BOOST_AUTO_TEST_SUITE_END();//}

