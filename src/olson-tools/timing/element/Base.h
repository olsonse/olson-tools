
#ifndef olson_tools_timing_element_Base_h
#define olson_tools_timing_element_Base_h


namespace olson_tools {
  namespace timing {
    namespace element {

      /** Abstract timing element class. 
       * In general, each timing element only operates in relative time.  An array
       * of these elements will then describe how a particular value (of something)
       * changes over the length of absolute time.  
       *
       * @see Timing
       */
      class Base {
      public:
        /** Length of this time interval. */
        double dt;

        Base( const double & _dt   = 0.0 ) : dt(_dt) { }
        virtual ~Base() {}

        /** return the value of this interval at t_rel relative time.
         * @param t_rel relative time.
         */
        virtual double getValue( const double & t_rel ) = 0;
      };

    }/* namespace olson_tools::timing::element */
  }/* namespace olson_tools::timing */
}/* namespace olson_tools */

#endif // olson_tools_timing_element_Base_h
