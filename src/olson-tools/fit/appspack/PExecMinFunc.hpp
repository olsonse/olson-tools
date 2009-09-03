// $Id$ 
/*! 
  \file PExecMinFunc.hpp

  \brief Thread cache APPSPACK executor.

  \sa \ref pageCustomize_executor 
*/

#ifndef olson_tools_fit_appspack_PExecMinFunc_hpp
#define olson_tools_fit_appspack_PExecMinFunc_hpp

#include <olson-tools/fit/appspack/ThreadedExecutor.hpp>
#include <olson-tools/strutil.h>

#include <redi/pstream.h>

#include <iterator>
#include <limits>
#include <sstream>

namespace olson_tools {
  namespace fit {
    namespace appspack {

      struct PExecMinFunc {
        /* MEMBER STORAGE */
        const std::string program_name;

        /* MEMBER FUNCTIONS */
        PExecMinFunc( const std::string & program_name )
          : program_name( program_name ) { }

        template < typename V0, typename V1 >
        std::string operator() ( const V0 & x,
                                       V1 & f,
                                 const int & tag ) const {
          redi::pstream prog( program_name + ' ' + to_string(tag),
                              redi::pstreams::pstdin |
                              redi::pstreams::pstdout );

          /* send program the inputs values. */
          prog << x.size() << std::endl;
          copy( x.begin(), x.end(), std::ostream_iterator<double>(prog, "\n") );
          prog << std::flush;

          std::string msg;
          { /* read output */
            std::string line;
            std::getline( prog, line );
            std::istringstream istr( line );
            /* reap the child program and check its status. */
            prog.close();
            int status = prog.rdbuf()->status() >> 8;

            double val = std::numeric_limits<double>::infinity();
            istr >> val;

            if ( istr.fail() ) {
              f.resize(0);
              msg = line;
            } else if (status != 0) {
              msg = "unknown evaluation failure";
            } else {
              f.resize(1);
              f[0] = val;
              std::getline( istr, msg );
            }
          }

          return msg;
        }/* operator() */
      };

    }/* namespace olson_tools::fit::appspack */
  }/* namespace olson_tools::fit */
}/* namespace olson_tools */

#endif // olson_tools_fit_appspack_PExecMinFunc_hpp
