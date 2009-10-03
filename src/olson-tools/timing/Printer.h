
#ifndef olson_tools_timing_Printer_h
#define olson_tools_timing_Printer_h

#include <olson-tools/timing/Timing.h>

#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include <limits>


namespace olson_tools {
  namespace timing {

    /** Generic timing print class.  
     * This class prints a set of timings out to file.
     *
     * @see Timing
     * @see TimingElement
     */
    struct Printer {
      /* MEMBER STORAGE */
      std::vector<Timing *> timers;


      /* MEMBER FUNCTIONS */

      /** Open a new file and print to it. */
      void print( const std::string & filename,
                  const double & ti,
                  const double & dt,
                  const double & tf ) {
        std::ofstream fout( filename.c_str() );
        print( fout, ti, dt, tf );
        fout.close();
      }

      /** Print to an output stream. */
      std::ostream & print( std::ostream & out,
                            const double & ti,
                            const double & dt,
                            const double & tf ) {

        /* save old timing values. */
        for (unsigned int i = 0; i < timers.size(); i++) {
          timers[i]->push_time();
        }

        double t_max = tf * (1. + 10. * std::numeric_limits<double>::epsilon());
        for (double t = ti; t <= t_max ; t+=dt) {
          out << t << '\t';

          for (unsigned int i = 0; i < timers.size(); i++) {
            timers[i]->set_time(t);
            out << timers[i]->getVal() << '\t';
          }

          out << '\n';
        }

        /* restore old timing values. */
        for (unsigned int i = 0; i < timers.size(); i++) {
          timers[i]->pop_time();
        }

        return out;
      }
    };

  }/* namespace olson_tools::timing */
}/* namespace olson_tools */

#endif // olson_tools_timing_Printer_h
