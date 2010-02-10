
#ifndef olson_tools_xml_Context_h
#define olson_tools_xml_Context_h

#include <olson-tools/xml/error.h>

#include <olson-tools/strutil.h>

#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <list>
#include <set>
#include <stdexcept>
#include <sstream>

namespace olson_tools {
  namespace xml {

    /** A simple class to represent a single (node-specific) XML context.  */
    struct Context {
      /* TYPEDEFS */
      /** The comparator for the Context::set type (for maintaining order). */
      struct Comparator {
        bool operator()(const Context & lhs, const Context & rhs) {
          return lhs.node < rhs.node;
        }
      };

      /** A (unique) set of Context instances.
       * @see eval(...)
       * */
      typedef std::set<Context, Comparator> set;

      /** A list of Context instances.
       * @see eval(...)
       * */
      typedef std::list<Context> list;



      /* MEMBER STORAGE */
      xmlXPathContextPtr ctx;
      xmlNodePtr node;



      /* MEMBER FUNCTIONS */

      /** The default constructor creates a contextless instance. */
      Context() : ctx(NULL), node(NULL) {}

      /** Constructor which specifies the context within the inherited XML
       * DOM tree. */
      Context(const Context & x, xmlNodePtr node) :
        ctx(x.ctx), node(node) {}

      /** Constructor which simply wraps around the current context in the
       * DOM tree context object. */
      Context(xmlXPathContextPtr ctx) :
        ctx(ctx), node(ctx->node) {}

      /** Constructor which specifies the context within the XML DOM tree.*/
      Context(xmlXPathContextPtr ctx, xmlNodePtr node) :
          ctx(ctx), node(node) {}

      /** Method to reset/set the context of this instance. */
      void assign(xmlXPathContextPtr _ctx, xmlNodePtr _node) {
        ctx = _ctx;
        node = _node;
      }

      /** Method to reset/set the context of this instance. */
      void assign(xmlXPathContextPtr _ctx) {
        assign(_ctx, _ctx->node);
      }

      /** Assertion which throws an error if the context object pointer
       * is NULL. */
      void assertContext(const std::string & err = "invalid XML context") const
      throw (error) {
        if (!ctx || !node)
          throw error(err);
      }

      /** XPath evaluation routine which properly pushes this context to the
       * context object and promptly pops this context back off after the
       * XPath evaluation is done. 
       *
       * @return A libxml2 structure which can hold several results.
       * */
      xmlXPathObjectPtr raw_eval(const std::string & q) const {
        assertContext();
        /*push*/
        xmlNodePtr old = ctx->node; ctx->node = node;
        /* eval */
        xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST q.c_str(), ctx);
        /*pop*/
        ctx->node = old;
        /* finished */
        return xpathObj;
      }

      /** A simpler method to obtain the unformatted results of a particular
       * XPath expression.
       *
       * @return A std::list of Context instances that (uniquely) represent each of
       * the different results of the XPath expression <code>q</code>.
       * */
      Context::list eval(const std::string & q) const {
        Context::list result;
        xmlXPathObjectPtr xpathObj = raw_eval(q);

        if (!xpathObj) {
          xmlXPathFreeObject(xpathObj);
          throw error("xpath expression failed:  '" + q + '\'');
        }

        xmlNodeSetPtr nodes = xpathObj->nodesetval;

        if (!nodes)
          return result;

        for(int i = nodes->nodeNr - 1; i >= 0; i--) {
          Context x(*this, nodes->nodeTab[i]);
          result.insert( result.begin(), x );
        }
        xmlXPathFreeObject(xpathObj);

        return result;
      }

      /** Search for a specific (single-valued) result of the XPath
       * expression in <code>q</code>.
       * */
      Context find(const std::string & q) const throw(nonsingle_result_error) {
        Context::list x_set = eval(q);
        if      (x_set.size() == 0)
          throw no_results("query '" + q + "' produced no results. cannot create context");
        else if (x_set.size() >1)
          throw too_many_results("query '" + q + "' produced too many results. cannot create context");
        else
          return *x_set.begin();
      }

      /** Search for and parse a specific (single-valued) result of the
       * XPath expression in <code>q</code>.
       *
       * @return The parsed value of the resulting XML node where the type
       * of the parsing function depends on the template parameter T.
       * */
      template <class T>
      T query(const std::string & q) const throw (nonsingle_result_error) {
        Context x = find(q);
        T retval;
        parse_item(retval, x);
        return retval;
      }

      /** Search for and parse a specific (single-valued) result of the
       * XPath expression in <code>q</code>.
       *
       * @return The parsed value of the resulting XML node where the type
       * of the parsing function depends on the template parameter T.
       * */
      template <class T>
      T query(const std::string & q, const T & _default) const {
        try {
          Context x = find(q);
          T retval;
          parse_item(retval, x);
          return retval;
        } catch (nonsingle_result_error) {
          return _default;
        }
      }

      /** Attempt to parse the text of this current node. */
      template <class T>
      T parse() const {
        T retval;
        parse_item(retval, *this);
        return retval;
      }

      /** Return the unparsed/unformatted text of this current node. */
      std::string text() const {
        std::string retval;
        if( node->type == XML_ELEMENT_NODE ||
          node->type == XML_ATTRIBUTE_NODE ||
          node->type == XML_TEXT_NODE ||
          node->type == XML_COMMENT_NODE) {

          char * txt = (char*)xmlNodeGetContent(node);
          retval = txt;
          free(txt);
        } else {
          std::ostringstream ostr;
          ostr <<  "node \"" << reinterpret_cast<const char*>(node->name)
               << "\": type " << node->type;
          retval = ostr.str();
        }
      
        return retval;
      }
    };


    /** Context less than operator (uses the Context::Comparator object).
     * @see Context::Comparator.
     */
    static inline bool operator< ( const Context & lhs, const Context & rhs ) {
      return Context::Comparator()(lhs,rhs);
    }

    /** Context equals operator (uses the Context::Comparator object).
     * @see Context::Comparator.
     */
    static inline bool operator== ( const Context & lhs, const Context & rhs ) {
      return !Context::Comparator()(lhs,rhs) and
             !Context::Comparator()(rhs,lhs);
    }

    /** Generic parse_item implementation that will match for any type that has
     * an istream extraction operation defined. */
    template <class A>
    static inline void parse_item(A & out, const Context & x) {
      try {
        out = olson_tools::from_string<A>(x.text());
      } catch (olson_tools::string_error& e) {
        throw error(e.what());
      }
    }

    /** Special implementation of parse_item for string return values.  This one
     * is specially implemented because of its glaring simplicity--no need to
     * use an extraction operation when we already have a string. */ 
    static inline void parse_item(std::string & out, const Context & x) {
      out = x.text();
    }

  }/* namespace olson_tools::xml */
}/* namespace olson_tools */

#endif // olson_tools_xml_Context_h
