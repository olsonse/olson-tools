#define BOOST_TEST_MODULE  XML_vector_parse

#include <olson-tools/xml/Doc.h>
#include <olson-tools/xml/vector_parse.h>

#include <boost/test/unit_test.hpp>

#include <vector>

#ifndef XML_FILENAME
#  error The filename was supposed to already be defined on the command line
#endif

namespace {
  namespace xml = olson_tools::xml;

  template < typename T >
  struct Value {
    T value;

    Value() : value(0) {}
  };

  template < typename T >
  static void parse_item( Value<T> & out, const xml::Context & x ) {
    out.value = x.parse< T >();
  }



  struct ABCValue {
    double a, b, c;

    ABCValue() : a(0), b(0), c(0) {}
  };

  static void parse_item( ABCValue & out, const xml::Context & x ) {
    out.a = x.query< double >("a");
    out.b = x.query< double >("b");
    out.c = x.query< double >("c");
  }

}

BOOST_AUTO_TEST_SUITE( XML_vector_parse_tests );//{

  BOOST_AUTO_TEST_SUITE( Basic_types );//{
  
    BOOST_AUTO_TEST_CASE( Double ) {
      xml::Doc doc(XML_FILENAME);
  
      xml::Context x = doc.find("//basic-vectors/double-vector");
  
      std::vector<double> v = x.parse< std::vector<double> >();
  
      BOOST_CHECK_EQUAL( v.size(), 3u );
      BOOST_CHECK_EQUAL( v[0], 1.1 );
      BOOST_CHECK_EQUAL( v[1], 2.2 );
      BOOST_CHECK_EQUAL( v[2], 3.3 );
    }
  
    BOOST_AUTO_TEST_CASE( Int ) {
      xml::Doc doc(XML_FILENAME);
  
      xml::Context x = doc.find("//basic-vectors/int-vector");
  
      std::vector<int> v = x.parse< std::vector<int> >();
  
      BOOST_CHECK_EQUAL( v.size(), 3u );
      BOOST_CHECK_EQUAL( v[0], 1 );
      BOOST_CHECK_EQUAL( v[1], 2 );
      BOOST_CHECK_EQUAL( v[2], 3 );
    }
  
    BOOST_AUTO_TEST_CASE( UInt ) {
      xml::Doc doc(XML_FILENAME);
  
      xml::Context x = doc.find("//basic-vectors/uint-vector");
  
      std::vector<unsigned int> v = x.parse< std::vector<unsigned int> >();
  
      BOOST_CHECK_EQUAL( v.size(), 3u );
      BOOST_CHECK_EQUAL( v[0], 1u );
      BOOST_CHECK_EQUAL( v[1], 2u );
      BOOST_CHECK_EQUAL( v[2], 3u );
    }
  
    BOOST_AUTO_TEST_CASE( Char ) {
      xml::Doc doc(XML_FILENAME);
  
      xml::Context x = doc.find("//basic-vectors/char-vector");
  
      std::vector<char> v = x.parse< std::vector<char> >();
  
      BOOST_CHECK_EQUAL( v.size(), 4u );
      BOOST_CHECK_EQUAL( v[0], 'I' );// comes from first char of comment
      BOOST_CHECK_EQUAL( v[1], '1' );
      BOOST_CHECK_EQUAL( v[2], '2' );
      BOOST_CHECK_EQUAL( v[3], '3' );
    }
  
  BOOST_AUTO_TEST_SUITE_END();//} Basic vector types



  BOOST_AUTO_TEST_SUITE( Proxy_types );//{

    BOOST_AUTO_TEST_CASE( Double ) {
      xml::Doc doc(XML_FILENAME);
  
      xml::Context x = doc.find("//proxy-vectors/double-vector");
  
      std::vector< Value<double> > v
        = x.parse< std::vector< Value<double> > >();
  
      BOOST_CHECK_EQUAL( v.size(), 3u );
      BOOST_CHECK_EQUAL( v[0].value, 1.1 );
      BOOST_CHECK_EQUAL( v[1].value, 2.2 );
      BOOST_CHECK_EQUAL( v[2].value, 3.3 );
    }

    BOOST_AUTO_TEST_CASE( Int ) {
      xml::Doc doc(XML_FILENAME);
  
      xml::Context x = doc.find("//proxy-vectors/int-vector");
  
      std::vector<int> v = x.parse< std::vector<int> >();
  
      BOOST_CHECK_EQUAL( v.size(), 3u );
      BOOST_CHECK_EQUAL( v[0], 1 );
      BOOST_CHECK_EQUAL( v[1], 2 );
      BOOST_CHECK_EQUAL( v[2], 3 );
    }
  
    BOOST_AUTO_TEST_CASE( UInt ) {
      xml::Doc doc(XML_FILENAME);
  
      xml::Context x = doc.find("//proxy-vectors/uint-vector");
  
      std::vector<unsigned int> v = x.parse< std::vector<unsigned int> >();
  
      BOOST_CHECK_EQUAL( v.size(), 3u );
      BOOST_CHECK_EQUAL( v[0], 1u );
      BOOST_CHECK_EQUAL( v[1], 2u );
      BOOST_CHECK_EQUAL( v[2], 3u );
    }
  
    BOOST_AUTO_TEST_CASE( Char ) {
      xml::Doc doc(XML_FILENAME);
  
      xml::Context x = doc.find("//proxy-vectors/char-vector");
  
      std::vector<char> v = x.parse< std::vector<char> >();
  
      BOOST_CHECK_EQUAL( v.size(), 4u );
      BOOST_CHECK_EQUAL( v[0], 'I' );// comes from first char of comment
      BOOST_CHECK_EQUAL( v[1], '1' );
      BOOST_CHECK_EQUAL( v[2], '2' );
      BOOST_CHECK_EQUAL( v[3], '3' );
    }
  
  BOOST_AUTO_TEST_SUITE_END();//} Proxy vector types




  BOOST_AUTO_TEST_SUITE( Composite_types );//{

    BOOST_AUTO_TEST_CASE( ABC ) {
      xml::Doc doc(XML_FILENAME);
  
      xml::Context x = doc.find("//composite-vectors/abc-vector");
  
      std::vector< ABCValue > v = x.parse< std::vector<ABCValue> >();
  
      BOOST_CHECK_EQUAL( v.size(), 3u );
      BOOST_CHECK_EQUAL( v[0].a, 1.1 );
      BOOST_CHECK_EQUAL( v[0].b, 2.1 );
      BOOST_CHECK_EQUAL( v[0].c, 3.1 );

      BOOST_CHECK_EQUAL( v[1].a, 1.2 );
      BOOST_CHECK_EQUAL( v[1].b, 2.2 );
      BOOST_CHECK_EQUAL( v[1].c, 3.2 );

      BOOST_CHECK_EQUAL( v[2].a, 1.3 );
      BOOST_CHECK_EQUAL( v[2].b, 2.3 );
      BOOST_CHECK_EQUAL( v[2].c, 3.3 );

    }

  BOOST_AUTO_TEST_SUITE_END();//} Composite vector types


BOOST_AUTO_TEST_SUITE_END();//}

