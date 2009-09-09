// $Id$ 
/** \file ExecMerit.hpp
 *
 * Execute a merit function file and obtain the result via FILE communication. 
 *
 */

#ifndef olson_tools_fit_ExecMerit_hpp
#define olson_tools_fit_ExecMerit_hpp

#include <olson-tools/fit/Gene.h>
#include <olson-tools/fit/ExecFunc.h>
#include <olson-tools/strutil.h>

#include <string>
#include <fstream>
#include <iterator>
#include <limits>
#include <cstdlib>
#include <sstream>

namespace olson_tools {
  namespace fit {

    struct ExecMerit : ExecFunc {
      /* MEMBER FUNCTIONS */
      ExecMerit( const std::string & program_name )
        : ExecFunc( program_name ) { }

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

        return f[0];
      }/* operator() */

      /** Evaluate the merit function, using ex_instance pointer to the
       * ExecMerit instance.
       */
      static merit_t merit( const Gene & gene, void * ex_instance ) /* const */ {
        assert( ex_intance != NULL );
        return static_cast<const ExecMerit*>( ex_instance )->merit( gene );
      }/* operator() */

    };

  }/* namespace olson_tools::fit */
}/* namespace olson_tools */

#endif // olson_tools_fit_ExecMerit_hpp
