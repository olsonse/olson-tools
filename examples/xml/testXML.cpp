/** \file
 * Simple test for XMLDoc class + physical::Quantity parser.
 * */


#ifndef   XML_FILENAME
#  define XML_FILENAME  "file.xml"
#endif

#include <olson-tools/data_set.h>
#include <olson-tools/xml/physical_parse.h>

#include <iostream>
#include <cstdlib>

using namespace runtime; /* strip runtime:: from physical::* */
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

  XMLContext::list xl = doc.eval("//calc-commands/command");
  XMLContext::list::iterator i = xl.begin();
  for (; i != xl.end(); i++) {
    const XMLContext & x = (*i);
    calc.exec(x.parse<std::string>());
  }
}


void usage( const char * arg0 ) {
  std::cout
    << "testXML program of olson-tools package.\n"
       "$Id$\n\n"
       "usage:  "
    << arg0 << " [options] [modifier]QUERY1 [[modifier]QUERY2] ...\n"
       "where the valid modifiers are q,d,r [default : q].\n"
       "The meaning of the modifiers is given by:\n\n"
       "\tq\tParse the query as a runtime::physical::Quanity item.\n"
        "\t\tIf there are multiple items returned by the query\n"
        "\t\teach will be parsed as a runtime::physical::Quantity.\n"
        "\t\tThis is the default when no option is given.\n"
       "\td\tParse the query as a olson_tools::data_set<A,B>\n"
        "\t\twhere A := B := runtime::physical::Quantity.\n"
        "\t\tIf there are multiple items returned by the query\n"
        "\t\teach will be parsed as a olson_tools::data_set<A,B>.\n"
       "\tr\tObtain the raw text of the query result.\n"
        "\t\tIf there are multiple items returned by the query\n"
        "\t\tthe text of each be shown.\n\n"
       "options:\n"
       "\t--help\tShow this help.\n\n"
       "Influential environment variables:\n"
       "\tXML_FILENAME\tSpecify an alternate xml file to query from\n"
    << std::endl;
}/* usage */

static const std::string help_str = "--help";

enum query_type {
  QUANTITY,
  DATASET,
  RAW
};

void showResults( std::ostream & out, XMLDoc & db,
                  const enum query_type qt, const char * query ) {
  using namespace olson_tools::xml;
  using physical::Quantity;
  using olson_tools::data_set;
  //using olson_tools::convert_data_set;

  XMLContext::list xl = db.eval(query);
  XMLContext::list::iterator i = xl.begin();
  for (; i != xl.end(); ++i) {
    const XMLContext & x = (*i);
    switch (qt) {
      case RAW:
        out << x.text() << std::endl;
        break;

      case DATASET:
        try {
          enum Quantity::PRINT_TYPE old = Quantity::getPrintMode();
          Quantity::setPrintMode( Quantity::MATH_PRINT );
          std::cout << db.query<pqdata_set>(query) << std::endl;
          Quantity::setPrintMode( old );
        } catch (physical::exception & e) {
          std::cout << e.what() << std::endl;
        }
        break;

      default:
      case QUANTITY:
        try {
          std::cout << x.parse<Quantity>() << std::endl;
        } catch (physical::exception & e) {
          std::cout << x.text() << std::endl;
        }
        break;
    }/* switch */
  }/* for */
}/* showResults() */

int main(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if ( help_str == argv[i] ) {
      usage(argv[0]);
      return EXIT_SUCCESS;
    }
  }

  const char * xml_filename = getenv("XML_FILENAME");
  if ( !xml_filename )
    xml_filename = XML_FILENAME;

  using olson_tools::xml::XMLDoc;
  XMLDoc db(xml_filename);
  prepareCalculator(db);


  /* Evaluate xpath expression */
  for (int i = 1; i < argc; ++i) {
    switch(argv[i][0]) {
      case 'd':
        showResults(std::cout, db, DATASET, argv[i]+1);
        break;
      case 'r':
        showResults(std::cout, db, RAW, argv[i]+1);
        break;
      case 'q':
        showResults(std::cout, db, QUANTITY, argv[i]+1);
        break;
      default:
        showResults(std::cout, db, QUANTITY, argv[i]);
        break;
    }
  }
  return 0;
}


