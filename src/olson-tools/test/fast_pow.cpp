#define BOOST_TEST_MODULE  fast_pow



#include <olson-tools/Timer.h>
#include <olson-tools/power.h>


#include <boost/test/unit_test.hpp>

#include <cmath>
//#include <iostream>

namespace {
  const double pos_min = 1e-1, pos_max = 1e8, pos_incr = 1.0;
  const double neg_min = -1e-1, neg_max = -1e8, neg_incr = -1.0;
}

BOOST_AUTO_TEST_SUITE( fast_pow );

BOOST_AUTO_TEST_CASE( timing ) {
  using olson_tools::fast_pow;

  olson_tools::Timer timer_fast_pow, timer_std_pow;
  timer_fast_pow.wall_time_label = timer_std_pow.wall_time_label = "s";
  timer_fast_pow.cpu_time_label = timer_std_pow.cpu_time_label = "s (cpu)";

  double sum = 0;
  timer_fast_pow.start();
  for (double i = pos_min; i < pos_max; i += pos_incr) {
    sum += fast_pow(i);
  }
  timer_fast_pow.stop();

  sum = 0;
  timer_std_pow.start();
  for (double i = pos_min; i < pos_max; i += pos_incr) {
    sum += std::pow(i);
  }
  timer_std_pow.stop();

  //std::cout << "timer(fast_pow)[" << pos_min << ":" << pos_incr << ":" << pos_max << "]:  " << timer_fast_pow << std::endl;
  //std::cout << "timer(std::pow)[" << pos_min << ":" << pos_incr << ":" << pos_max << "]:  " << timer_std_pow << std::endl;

  BOOST_CHECK_GT( timer_std_pow.dt, 30.0*timer_fast_pow.dt );

  sum = 0;
  timer_fast_pow.start();
  for (double i = neg_min; i > neg_max; i += neg_incr) {
    sum += fast_pow(i);
  }
  timer_fast_pow.stop();

  sum = 0;
  timer_std_pow.start();
  for (double i = neg_min; i > neg_max; i += neg_incr) {
    sum += std::pow(i);
  }
  timer_std_pow.stop();

  //std::cout << "timer(fast_pow)[" << neg_min << ":" << neg_incr << ":" << neg_max << "]:  " << timer_fast_pow << std::endl;
  //std::cout << "timer(std::pow)[" << neg_min << ":" << neg_incr << ":" << neg_max << "]:  " << timer_std_pow << std::endl;

  BOOST_CHECK_GT( timer_std_pow.dt, 30.0*timer_fast_pow.dt );
}

BOOST_AUTO_TEST_CASE( values ) {
  using olson_tools::fast_pow;

  double total_err = 0;
  for (double i = neg_max; i < pos_max; i += pos_incr) {
    double fl2 = fast_pow(i);
    double  l2 = std::pow(i);
    total_err += (1.0 - (fl2/l2));
  }

  //std::cout << "total error[" << pos_min << ":" << pos_incr << ":" << pos_max << "]:  " << total_err << std::endl;
  //#ifdef __CYGWIN__
  ///* Cygwin has a lame macro definition of log2. */
  //BOOST_CHECK_LT( std::abs(total_err), 1e-8 );
  //#else
  BOOST_CHECK_LT( std::abs(total_err), 3e-11 );
  //#endif

}

BOOST_AUTO_TEST_SUITE_END();
