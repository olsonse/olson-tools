// $Id$ 
/*! 
  \file GeneMinFunc.hpp

  \brief Thread cache APPSPACK executor.

  \sa \ref pageCustomize_executor 
*/

#ifndef olson_tools_fit_GeneMinFunc_hpp
#define olson_tools_fit_GeneMinFunc_hpp

#include <olson-tools/fit/Individual.h>

#include <fstream>
#include <iterator>
#include <limits>
#include <cstdlib>

namespace olson_tools {
  namespace fit {
    namespace detail {

      template < typename MF >
      struct GeneMinFunc {
        /* MEMBER STORAGE */
        Individual<MF> * const member;

        /* MEMBER FUNCTIONS */
        GeneMinFunc( Individual<MF> * const member )
          : member( member ) { }

        template < typename V0, typename V1 >
        std::string operator() ( const V0 & x,
                                       V1 & f,
                                 const int & tag ) const {

          f.resize(1);
          f[0] = -member->test_Merit(x.begin(), ALLELE_DYNAMIC_CONT);

          return "success";

          // FIXME:  There needs to be a mechanism for returning status through
          // Merit() to the genetic algorithm.
        }/* operator() */
      };

    }/* namespace olson_tools::fit::detail */
  }/* namespace olson_tools::fit */
}/* namespace olson_tools */

#endif // olson_tools_fit_GeneMinFunc_hpp
