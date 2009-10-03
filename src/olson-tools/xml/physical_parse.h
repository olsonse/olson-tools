#ifndef olson_tools_xml_physical_parse_h
#define olson_tools_xml_physical_parse_h

#include <olson-tools/xml/Doc.h>
#include <physical/calc/infix.h>

namespace olson_tools {
  namespace xml {
    static inline void parse_item( runtime::physical::Quantity & out,
                                   const Context & x) {
      using runtime::physical::calc::InfixCalc;
      out = InfixCalc::instance().parse( x.text() );
    }
  } /* namespace olson_tools::xml */
} /* namespace olson_tools */
#endif // olson_tools_xml_physical_parse_h
