// $Id$ 
/*! 
  \file GeneMinFunc.hpp

  \brief Thread cache APPSPACK executor.

  \sa \ref pageCustomize_executor 
*/

#ifndef olson_tools_fit_GeneMinFunc_hpp
#define olson_tools_fit_GeneMinFunc_hpp

#include <olson-tools/fit/Individual.h>
#include <olson-tools/fit/appspack/ThreadedExecutor.hpp>

#include <fstream>
#include <iterator>
#include <limits>
#include <cstdlib>

namespace olson_tools {
  namespace fit {
    namespace detail {

      struct GeneMinFunc {
        /* MEMBER STORAGE */
        Individual * const member;

        /* MEMBER FUNCTIONS */
        GeneMinFunc( Individual * const member )
          : member( member ) { }

        template < typename V0, typename V1 >
        std::string operator() ( const V0 & x,
                                       V1 & f,
                                 const int & tag ) const {

          Allele_t * p = new Allele_t[x.size()];
          for ( unsigned int i = 0; i < x.size(); ++i )
            p[i] = x[i];

          f.resize(1);
          f[0] = -member->test_Merit(p,ALLELE_DYNAMIC_CONT);

          delete[] p;

          return "success";

          // FIXME:  There needs to be a mechanism for returning status through
          // Merit() to the genetic algorithm.
        }/* operator() */
      };

    }/* namespace olson_tools::fit::detail */
  }/* namespace olson_tools::fit */
}/* namespace olson_tools */

#endif // olson_tools_fit_GeneMinFunc_hpp
