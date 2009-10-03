
#ifndef olson_tools_xml_error_h
#define olson_tools_xml_error_h

#include <string>
#include <stdexcept>

namespace olson_tools {
  namespace xml {

    /** The error exception class for this little XML library wrapper. */
    struct error : std::runtime_error {
      typedef std::runtime_error super;
      error(const std::string & s) : super(s) {}
    };

    struct nonsingle_result_error : error {
      nonsingle_result_error(const std::string & s) : error(s) {}
    };

    struct no_results : nonsingle_result_error {
      no_results(const std::string & s) : nonsingle_result_error(s) {}
    };

    struct too_many_results : nonsingle_result_error {
      too_many_results(const std::string & s) : nonsingle_result_error(s) {}
    };

  }/* namespace olson_tools::xml */
}/* namespace olson_tools */

#endif // olson_tools_xml_error_h
