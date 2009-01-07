#ifndef OLSON_TOOLS_DATA_SET_H
#define OLSON_TOOLS_DATA_SET_H

#include "xml/XMLDoc.h"

#include <map>

namespace olson_tools {
    template <class A, class B>
    struct data_point : std::pair<A,B> {
        typedef std::pair<A,B> super;
        data_point() : super() {}
        data_point(const A& a, const B& b) : super(a,b) {}
    };

    template <class A, class B>
    struct data_set : std::map<A,B> {};

    template <class A, class B>
    std::ostream & operator<<(std::ostream & out, const data_point<A,B> & p) {
        return out << "<data x=\'" << p.first << "\' y=\'" << p.second << "\'/>";
    }

    template<class A, class B>
    std::ostream & operator<<(std::ostream & out, const data_set<A,B> & data) {
        out << "<dataset>\n";
        typename data_set<A,B>::const_iterator i;
        for (i = data.begin(); i != data.end(); i++)
            out << ((data_point<A,B> &)(*i)) << '\n';
        return out << "</dataset>";
    }

    template <class C, class D, class InIter, class UnitsPair>
    data_set<C,D> convert_data_set( InIter i, const InIter & f,
                                    const UnitsPair & units) {
        data_set<C,D> retval;
        for (; i != f; i++)
            retval.insert(
                std::make_pair<C,D>(
                    i->first.assertMatch(units.first).coeff,
                    i->second.assertMatch(units.second).coeff
                )
            );
        return retval;
    }

    template <class A, class B>
    static void parse_item(data_point<A,B> & out, const xml::XMLContext & x) {
        A a = x.query<A>("@x");
        B b = x.query<B>("@y");
        out = data_point<A,B>(a,b);
    }


    template <class A, class B>
    static void parse_item(data_set<A,B> & out, const xml::XMLContext & x) {
        A xscale = x.query<A>("@xscale");
        B yscale = x.query<B>("@yscale");

        xml::XMLContext::set x_set = x.eval("val");
        xml::XMLContext::set::iterator i = x_set.begin();

        for(; i != x_set.end(); i++) {
            const xml::XMLContext & x1 = (*i);
            data_point<A,B> dp = x1.parse< data_point<A,B> >();
            dp.first  *= xscale;
            dp.second *= yscale;
            out.insert( dp );
        }
    }

}/* namespace olson_tools */

#endif // OLSON_TOOLS_DATA_SET_H
