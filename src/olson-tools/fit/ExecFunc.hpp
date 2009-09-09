// $Id$ 
/*! 
  \file ExecFunc.hpp

  \brief Execute a function file and obtain the result via FILE communication.  

  \sa \ref pageCustomize_executor 
*/

#ifndef olson_tools_fit_ExecFunc_hpp
#define olson_tools_fit_ExecFunc_hpp

#include <olson-tools/strutil.h>

#include <fstream>
#include <iterator>
#include <limits>
#include <cstdlib>
#include <sstream>

namespace olson_tools {
  namespace fit {

    struct ExecFunc {
      /* MEMBER STORAGE */
      const std::string program_name;

      /* MEMBER FUNCTIONS */
      ExecFunc( const std::string & program_name )
        : program_name( program_name ) { }

      template < typename V0, typename V1 >
      std::string operator() ( const V0 & x,
                                     V1 & f,
                               const int & tag = 0 ) const {
        /* create temporary input and output filenames. */
        char infile[100]  = "infile.XXXXXX";
        char outfile[100] = "outfile.XXXXXX";
        close(::mkstemp(infile));
        close(::mkstemp(outfile));

        std::string cmd = program_name
                        + ' '
                        + std::string(infile)
                        + ' '
                        + std::string(outfile)
                        + ' '
                        + to_string(tag);

        { /* write program the inputs values. */
          std::ofstream ifile(infile);
          ifile << x.size() << '\n';
          copy( x.begin(), x.end(), std::ostream_iterator<double>(ifile, "\n") );
          ifile.close();
        }
        
        /* run program */
        int status = std::system(cmd.c_str()) >> 8;

        std::string msg;
        { /* read output. */
          std::ifstream ofile(outfile);
          std::string line;
          std::getline( ofile, line );
          std::istringstream istr( line );
          ofile.close();

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

        /* remove temporary files. */
        unlink(infile);
        unlink(outfile);

        return msg;
      }/* operator() */
    };

  }/* namespace olson_tools::fit */
}/* namespace olson_tools */

#endif // olson_tools_fit_ExecFunc_hpp
