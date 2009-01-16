/** \file
 * Simple test for XMLDoc class + physical::Quantity parser.
 * */


#ifndef   PARTICLEDB_XML
#  define PARTICLEDB_XML  "file.xml"
#endif

#include <iostream>
#include <olson-tools/data_set.h>
#include <olson-tools/xml/physical_parse.h>

typedef olson_tools::data_set<physical::Quantity,physical::Quantity> pqdata_set;

using olson_tools::xml::XMLDoc;
using olson_tools::xml::XMLContext;

void prepareCalculator(const XMLDoc & doc) {
    /* prepare infix units calculator. */
    using physical::calc::InfixCalc;
    using physical::calc::symbol;
    InfixCalc::base_calc & calc = InfixCalc::instance();

    /* clear the old symbols out */
    calc.symbols.clear();
    calc.addMathLib();
    calc.addPhysicalUnits();

    XMLContext::set xl = doc.eval("//calc-commands/command");
    XMLContext::set::iterator i = xl.begin();
    for (; i != xl.end(); i++) {
        const XMLContext & x = (*i);
        calc.exec(x.parse<std::string>());
    }
}


int main(int argc, char **argv) {
    using physical::Quantity;
    using namespace olson_tools::xml;
    using olson_tools::data_set;
    using olson_tools::convert_data_set;
    using physical::constant::eV;
    using physical::unit::m;

    XMLDoc db(PARTICLEDB_XML);
    prepareCalculator(db);


    /* Evaluate xpath expression */
    for (int i = 1; i < argc; i++) {
        bool raw = false;
        switch(argv[i][0]) {
            case 'q':
                try {
                    std::cout << db.query<Quantity>(argv[i]+1) << std::endl;
                } catch (physical::exception & e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            case 'd':
                try {
                    enum Quantity::PRINT_TYPE old = Quantity::print_type;
                    Quantity::print_type = Quantity::MATH_PRINT;
                    std::cout << db.query<pqdata_set>(argv[i]+1) << std::endl;
                    Quantity::print_type = old;
                } catch (physical::exception & e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            default:
                try {
                    std::cout << db.query<physical::Quantity>(argv[i]) << std::endl;
                } catch (physical::exception & e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            case 'r':
                raw = true;
            case 'm':
                XMLContext::set xl = db.eval(argv[i]+1);
                XMLContext::set::iterator i = xl.begin();
                for (; i != xl.end(); i++) {
                    const XMLContext & x = (*i);
                    try {
                        if (!raw)
                            std::cout << x.parse<Quantity>() << std::endl;
                        else
                            std::cout << x.text() << std::endl;
                    } catch (physical::exception & e) {
                        std::cout << x.text() << std::endl;
                    }
                }
                break;
        }
    }
    return 0;
}

