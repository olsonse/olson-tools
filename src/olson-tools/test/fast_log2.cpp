#ifndef USE_SPENCERS_FAST_POW
int main() { return 0; }
#else

#define BOOST_TEST_MODULE  fast_log2_timing



#include <olson-tools/Timer.h>
#include <olson-tools/power.h>


#include <boost/test/unit_test.hpp>

#include <vector>
#include <limits>
#include <cmath>

#if defined(__INTEL_COMIPILER) && (__INTEL_COMPILER <= 1100)
/* Intel doesn't yet work with gcc 4.4 where these are in std */
namespace std {
  using ::isinf;
  using ::isnan;
  using ::isfinite;
}
#endif

namespace {
  const double pos_min = 1e-1, pos_max = 1e8, pos_incr = 1.0;
  const double neg_min = -1e-1, neg_max = -1e8, neg_incr = -1.0;
}

BOOST_AUTO_TEST_CASE( timing ) {
  using olson_tools::fast_log2;

  olson_tools::Timer timer_fast_log2, timer_std_log2;
  timer_fast_log2.wall_time_label = timer_std_log2.wall_time_label = "s";
  timer_fast_log2.cpu_time_label = timer_std_log2.cpu_time_label = "s (cpu)";

  std::vector<double> results;
  double sum = 0;
  timer_fast_log2.start();
  for (double i = pos_min; i < pos_max; i += pos_incr) {
    sum += fast_log2(i);
  }
  timer_fast_log2.stop();

  // save the results and tell the compiler to quit optmizing things away.
  results.push_back( sum );

  sum = 0;
  timer_std_log2.start();
  for (double i = pos_min; i < pos_max; i += pos_incr) {
    sum +=      log2(i);
  }
  timer_std_log2.stop();

  // save the results and tell the compiler to quit optmizing things away.
  results.push_back( sum );

  //std::cout << "timer(fast_log2)[" << pos_min << ":" << pos_incr << ":" << pos_max << "]:  " << timer_fast_log2 << std::endl;
  //std::cout << "timer(     log2)[" << pos_min << ":" << pos_incr << ":" << pos_max << "]:  " << timer_std_log2 << std::endl;

  BOOST_CHECK_GT( timer_std_log2.dt, 30.0*timer_fast_log2.dt );

  sum = 0;
  timer_fast_log2.start();
  for (double i = neg_min; i > neg_max; i += neg_incr) {
    sum += std::isnan(fast_log2(i));
  }
  timer_fast_log2.stop();

  // save the results and tell the compiler to quit optmizing things away.
  results.push_back( sum );

  sum = 0;
  timer_std_log2.start();
  for (double i = neg_min; i > neg_max; i += neg_incr) {
    sum +=      std::isnan(log2(i));
  }
  timer_std_log2.stop();

  // save the results and tell the compiler to quit optmizing things away.
  results.push_back( sum );

  //std::cout << "timer(fast_log2)[" << neg_min << ":" << neg_incr << ":" << neg_max << "]:  " << timer_fast_log2 << std::endl;
  //std::cout << "timer(     log2)[" << neg_min << ":" << neg_incr << ":" << neg_max << "]:  " << timer_std_log2 << std::endl;

  // STOP OPTIMIZING THINGS AWAY!!!
  BOOST_CHECK_EQUAL( results.size(), 4u );
  BOOST_CHECK_LT( results[0], std::numeric_limits<double>::max() );
  BOOST_CHECK_LT( results[1], std::numeric_limits<double>::max() );
  BOOST_CHECK_LT( results[2], std::numeric_limits<double>::max() );
  BOOST_CHECK_LT( results[3], std::numeric_limits<double>::max() );

  BOOST_CHECK_GT( timer_std_log2.dt, 10.0*timer_fast_log2.dt );
}

BOOST_AUTO_TEST_CASE( values ) {
  using olson_tools::fast_log2;

  double total_err = 0;
  for (double i = pos_min; i < pos_max; i += pos_incr) {
    double fl2 = fast_log2(i);
    double  l2 =      log2(i);
    total_err += (1.0 - (fl2/l2));
  }

  //std::cout << "total error[" << pos_min << ":" << pos_incr << ":" << pos_max << "]:  " << total_err << std::endl;
  #ifdef __CYGWIN__
  /* Cygwin has a lame macro definition of log2. */
  BOOST_CHECK_LT( std::abs(total_err), 1e-8 );
  #else
  BOOST_CHECK_LT( std::abs(total_err), 3e-11 );
  #endif

  int total_invalid_differences = 0;
  for (double i = neg_min; i > neg_max; i += neg_incr) {
    double fl2 = fast_log2(i);
    double  l2 =      log2(i);

    if ( (std::isnan(fl2) ^ std::isnan(l2)) ||
         (std::isinf(fl2) ^ std::isinf(l2)) ||
         ( (std::isfinite(fl2) && std::isfinite(l2)) && 
           (fl2 != l2) ) ) {
      ++total_invalid_differences;
    }
  }

  //std::cout << "total differences["
  //          << neg_min << ":" << neg_incr << ":" << neg_max << "]:  "
  //          << total_invalid_differences << std::endl;
  BOOST_CHECK_EQUAL( total_invalid_differences, 0);

}

#endif // USE_SPENCERS_FAST_POW
