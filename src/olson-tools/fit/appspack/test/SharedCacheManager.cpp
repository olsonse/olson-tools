#define BOOST_TEST_MODULE  SharedCacheManager

#include <olson-tools/fit/appspack/SharedCacheManager.hpp>

#include <appspack/APPSPACK_Parameter_List.hpp>

#include <boost/test/unit_test.hpp>

namespace {
  using APPSPACK::Vector;

  static APPSPACK::Parameter::List params;
  APPSPACK::Vector scaling(1,0.0);

  static int prep() {
    params.sublist("Solver").setParameter("Step Tolerance", static_cast<double>(0.02));
    return 0;
  }
  static int prepped = prep();

  typedef olson_tools::fit::appspack::SharedCacheManager<> shared_manager;
}


BOOST_AUTO_TEST_SUITE( SharedCacheManager_test );

  BOOST_AUTO_TEST_CASE( instantiation ) {
    shared_manager sm(params.sublist("Solver"), scaling);

    Vector x(1,1.5), f(1, 2.5);
    sm.insert( x, f );

    BOOST_CHECK_EQUAL( sm.isCached( x, f ), true );
  }

  BOOST_AUTO_TEST_CASE( shared_data ) {
    shared_manager sm2(params.sublist("Solver"), scaling);

    Vector x(1,1.5), f(1, 2.5);
    BOOST_CHECK_EQUAL( sm2.isCached( x, f ), true );
  }

BOOST_AUTO_TEST_SUITE_END();

