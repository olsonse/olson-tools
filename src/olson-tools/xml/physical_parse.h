#ifndef olson_tools_xml_physical_parse_h
#define olson_tools_xml_physical_parse_h

#include <olson-tools/xml/XMLDoc.h>
#include <physical/calc/infix.h>

namespace olson_tools { namespace xml {
    static void parse_item(runtime::physical::Quantity & out, const XMLContext & x) {
        using runtime::physical::calc::InfixCalc;
    
        if( x.node->type != XML_ELEMENT_NODE &&
            x.node->type != XML_ATTRIBUTE_NODE &&
            x.node->type != XML_TEXT_NODE)
            throw xml_error("unknown xml node type");
    
        char * bob = (char*)xmlNodeGetContent(x.node);
        out = InfixCalc::instance().parse(bob);
        free(bob);
    }
}} /* namespace olson_tools::xml */
#endif // olson_tools_xml_physical_parse_h
