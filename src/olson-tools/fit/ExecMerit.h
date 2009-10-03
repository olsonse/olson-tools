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

    /** Execute an external program/script as a merit function for the GA.
     * @param id
     *    dummy argument to allow multiple ExecMerit instances use different
     *    static data (e.g. use different external scripts/programs).
     */
    template < unsigned int id = 0u >
    struct ExecMerit {
      /* NON-LOCAL STORAGE */
      /** The result that will be given if the external program does not return
       * a value [Default -inf].  
       * NOTE:  don't forget that the genetic algorithm seeks to maximize merit
       * and not minimize it.   Therefore, make sure that ERROR_RESULT is set to
       * a reasonably low number.  */
      static merit_t ERROR_RESULT;

      /** Static instance of ExecFunc class to execute the external program.
       * This defaults to attempting to open '/dev/null', so you will NEED to
       * set this appropriately befor ExecMerit::operator() will properly
       * succeed.
       *
       * @see setProgramName
       */
      static ExecFunc execFunc;

      /* STATIC FUNCTIONS */
      /** A simple function to help changing the external program to execute. */
      static void setProgramName( const std::string & program_name ) {
        ExecMerit::execFunc = ExecFunc( program_name );
      }

      /* MEMBER FUNCTIONS */
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
        std::string msg = execFunc( g, f );

        if ( f.size() > 0 )
          return f[0];
        else
          return ERROR_RESULT;
      }/* operator() */

    };

    template < unsigned int id >
    merit_t ExecMerit<id>::ERROR_RESULT =
      -std::numeric_limits<merit_t>::infinity();

    template < unsigned int id >
    ExecFunc ExecMerit<id>::execFunc("/dev/null");

  }/* namespace olson_tools::fit */
}/* namespace olson_tools */

#endif // olson_tools_fit_ExecMerit_hpp
