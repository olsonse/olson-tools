
#ifndef olson_tools_xml_Doc_h
#define olson_tools_xml_Doc_h

#include <olson-tools/xml/error.h>
#include <olson-tools/xml/Context.h>

#include <olson-tools/strutil.h>

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xinclude.h>

namespace olson_tools {
  namespace xml {

    /** A simple class to load a XML document into memory (DOM model) and
     * provide simple access to the contents via xpath queries.  This and the
     * other associated classes of this package resemble a type of interface
     * that can be had using the python interface to libxml2. */
    struct Doc {
      /* MEMBER STORAGE */
      /** A pointer to the actual libxml2 XML document in memory. */
      xmlDocPtr doc;

      /** A context referencing the root of the tree.  */
      Context root_context;



      /* MEMBER FUNCTIONS. */

      /** Constructor also opens a file if the file name is given as a paramter.
       * @param filename
       *   Name of the XML file on disk to open.
       * */
      Doc(const std::string & filename = "") : doc(NULL), root_context() {
        if (filename.length() > 0)
          open(filename);
      }

      /** Destructor closes the XML file. */
      ~Doc() {
        close();
      }

      /** Open the specified XML file on disk.
       * @param filename
       *   Name of the XML file on disk to open.
       */
      void open(const std::string & filename) throw (error) {
        close();
        doc = xmlParseFile(filename.c_str());
        if (doc == NULL)
          throw error( "Error: unable to parse file \"" + filename + '\"');

        /* specify that we want XInclude processing to work. */
        xmlXIncludeProcess(doc);

        /* Create xpath evaluation context.  It isn't my fault that the libxml
         * guys force me to do a reinterpret cast. */
        root_context.assign( xmlXPathNewContext(doc),
                             reinterpret_cast<xmlNodePtr>(doc) );
        try {
          root_context.assertContext("unable to create root XPath context");
        } catch (error & e) {
          close();
          throw e;
        }
      }

      /** Close and cleanup, including freeing the XML trees and related XPath
       * context. */
      void close() {
        if (doc)
          xmlFreeDoc(doc); 
        doc = NULL;
        if (root_context.ctx)
          xmlXPathFreeContext(root_context.ctx); 
        root_context.ctx = NULL;
      }

      /** Assertion which throws an error if either the XML document pointer or
       * the context object pointer are NULL. */
      void assertOpen() const throw (error) {
        if (!doc || !root_context.ctx)
          throw error("xml file not opened");
      }

      /** Perform a query for a single XML node starting from the root context
       * of the XML tree. */
      template <class T>
      T query(const std::string & q) const
      throw (nonsingle_result_error) {
        assertOpen();
        return root_context.query<T>(q);
      }

      /** Perform a query for a single XML node starting from the root context
       * of the XML tree.  This version of query allows for a default parameter
       * to be used in case the query fails.  This version does NOT throw any
       * exceptions. */
      template <class T>
      T query(const std::string & q, const T & _default) const {
        assertOpen();
        return root_context.query(q, _default);
      }

      /** Evaluate an XPath expression which can possibly return multiple
       * results starting at the context of the root node. */
      Context::list eval(const std::string & q) const {
        assertOpen();
        return root_context.eval(q);
      }

      /** Find a single XML node based on a XPath query starting at the root
       * context of the XML tree. */
      Context find(const std::string & q) const throw(nonsingle_result_error) {
        assertOpen();
        return root_context.find(q);
      }
    };

  }/* namespace olson_tools::xml */
}/* namespace olson_tools */

#endif // olson_tools_xml_Doc_h
