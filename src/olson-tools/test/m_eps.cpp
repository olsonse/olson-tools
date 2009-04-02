#define BOOST_TEST_MODULE  m_eps

#include <olson-tools/m_eps.h>

#include <boost/test/unit_test.hpp>
#include <iostream>


BOOST_AUTO_TEST_SUITE( m_eps );

BOOST_AUTO_TEST_CASE( m_eps_calc ) {
  using olson_tools::M_EPSl;
  using olson_tools::M_EPS;
  using olson_tools::M_EPSf;
  using olson_tools::getMachineEPS_quad;
  using olson_tools::getMachineEPS_double;
  using olson_tools::getMachineEPS_single;
  std::cout.precision(250);

  //std::cout << "        M_EPSf      :  " << M_EPSf << std::endl;
  //std::cout << "getMachineEPS_quad  :  " << getMachineEPS_quad() << std::endl;
  //std::cout << "        M_EPS       :  " << M_EPS << std::endl;
  //std::cout << "getMachineEPS_double:  " << getMachineEPS_double() << std::endl;
  //std::cout << "        M_EPSl      :  " << M_EPSl << std::endl;
  //std::cout << "getMachineEPS_single:  " << getMachineEPS_single() << std::endl;

  /* FIXME:  probably will need a more robust comparison, like say allow for
   * 0.001 percent error */
  BOOST_CHECK_EQUAL( M_EPSl, getMachineEPS_quad() );
  BOOST_CHECK_EQUAL( M_EPS,  getMachineEPS_double() );
  BOOST_CHECK_EQUAL( M_EPSf, getMachineEPS_single() );
}

BOOST_AUTO_TEST_SUITE_END();

