#ifndef OLSON_TOOLS_XML_PHYSICAL_PARSE_H
#define OLSON_TOOLS_XML_PHYSICAL_PARSE_H

#include "XMLDoc.h"
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
#endif // OLSON_TOOLS_XML_PHYSICAL_PARSE_H
