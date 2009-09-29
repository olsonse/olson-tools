// $Id$ 
/** \file ExecMerit.hpp
 *
 * Execute a merit function file and obtain the result via FILE communication. 
 *
 */

#ifndef olson_tools_fit_ExecMerit_hpp
#define olson_tools_fit_ExecMerit_hpp

#include <olson-tools/fit/merit_def.h>
#include <olson-tools/fit/Gene.h>
#include <olson-tools/fit/ExecFunc.h>
#include <olson-tools/strutil.h>

#include <string>
#include <fstream>
#include <iterator>
#include <limits>
#include <sstream>
#include <vector>

#include <cstdlib>
#include <cassert>

namespace olson_tools {
  namespace fit {

    struct ExecMerit : ExecFunc {
      /* MEMBER STORAGE */
      /** The result that will be given if the external program does not return
       * a value [Default -inf].  
       * NOTE:  don't forget that the genetic algorithm seeks to maximize merit
       * and not minimize it.   Therefore, make sure that ERROR_RESULT is set to
       * a reasonably low number.  */
      const merit_t ERROR_RESULT;

      /* MEMBER FUNCTIONS */
      ExecMerit( const std::string & program_name,
                 const merit_t & ERROR_RESULT =
                   -std::numeric_limits<merit_t>::infinity() )
        : ExecFunc( program_name ), ERROR_RESULT(ERROR_RESULT) { }

      /** Evaluate the merit function. */
      merit_t operator() ( const Gene & gene ) const {
        std::vector<Allele_t> g;
        const int sz = gene.size();
        for ( int i = 0; i < sz; ++i ) {
          g.push_back( gene[i].val );
        }

        std::vector<merit_t> f;

        /* FIXME:  There needs to be a way to pass back messages and error
         * conditions to the genetic algorithm. */
        std::string msg = ExecFunc::operator() ( g, f );

        if ( f.size() > 0 )
          return f[0];
        else
          return ERROR_RESULT;
      }/* operator() */

      /** Evaluate the merit function, using ex_instance pointer to the
       * ExecMerit instance.
       */
      static merit_t merit( const Gene & gene, void * ex_instance ) /* const */ {
        assert( ex_instance != NULL );
        return static_cast<const ExecMerit*>( ex_instance )->operator()( gene );
      }/* operator() */

    };

  }/* namespace olson_tools::fit */
}/* namespace olson_tools */

#endif // olson_tools_fit_ExecMerit_hpp
