/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2004-2008 Spencer Olson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *  
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *                                                                                 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.                                                                           .
 * 
 * Questions? Contact Spencer Olson (olsonse@umich.edu) 
 */

#ifndef OLSON_TOOLS_DATA_SET_H
#define OLSON_TOOLS_DATA_SET_H

#include <olson-tools/xml/Doc.h>

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
        for (i = data.begin(); i != data.end(); ++i)
            out << ((data_point<A,B> &)(*i)) << '\n';
        return out << "</dataset>";
    }

    template <class C, class D, class InIter, class UnitsPair>
    data_set<C,D> convert_data_set( InIter i, const InIter & f,
                                    const UnitsPair & units) {
        data_set<C,D> retval;
        for (; i != f; ++i)
            retval.insert(
                std::make_pair<C,D>(
                    i->first.assertMatch(units.first).coeff,
                    i->second.assertMatch(units.second).coeff
                )
            );
        return retval;
    }

    template <class A, class B>
    static void parse_item(data_point<A,B> & out, const xml::Context & x) {
        A a = x.query<A>("@x");
        B b = x.query<B>("@y");
        out = data_point<A,B>(a,b);
    }


    template <class A, class B>
    static void parse_item(data_set<A,B> & out, const xml::Context & x) {
        A xscale = x.query<A>("@xscale");
        B yscale = x.query<B>("@yscale");

        xml::Context::list x_list = x.eval("val");
        xml::Context::list::iterator i = x_list.begin();

        for(; i != x_list.end(); ++i) {
            const xml::Context & x1 = (*i);
            data_point<A,B> dp = x1.parse< data_point<A,B> >();
            dp.first  *= xscale;
            dp.second *= yscale;
            out.insert( dp );
        }
    }

}/* namespace olson_tools */

#endif // OLSON_TOOLS_DATA_SET_H
