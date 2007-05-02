// -*- c++ -*-
// $Id: readData.h,v 1.2 2005/04/19 17:23:21 olsonse Exp $
/*
 * Copyright 2004 Spencer Olson
 *
 * $Log: readData.h,v $
 * Revision 1.2  2005/04/19 17:23:21  olsonse
 * Added new RKIntegrator wrapper class to allow for generic integration
 * templates.
 *
 * Also added trapfe library to help with trapping floating point exceptions.
 *
 * Fixed Distribution inverter finally (hopefull).  It no longer truncates the
 * distribution or reads from bogus memory.
 *
 * Added rk2 integrator (modified Euler) to rk.F.
 *
 * Various other fixes.
 *
 * Revision 1.1  2005/01/14 14:09:31  olsonse
 * Fixed documentation on memory.h, msh.h, options.h.
 * Moved new files Distribution.[hC] listutil.h readData.h from dsmc code.
 *
 * Revision 1.1.1.1  2005/01/08 04:57:25  olsonse
 * Initial CVS import (finally).
 *
 *
 */

/** \file
 * Some utility functions to help read various types of data from a file.
 *
 * Copyright 2004 Spencer Olson
 */

#ifndef READDATA_H
#define READDATA_H

#include <stdexcept>
#include <vector>
#include <iterator>
#include <iostream>
#include <string>
#include <sstream>
#include "listutil.h"

/** Read a block of data from input stream into a linked list.
 * A block of data is defined by the overloaded operator>>(...) that is used
 * to read in an individual data point.
 * As an example, you may define, for your data type _Data:
 *
 \verbatim
    std::istream & operator>>(std::istream & input, _Data * & d) {
        while (read_past_comment(input));

        if (!input.good()) return input;

        d = new _Data;
        return input >> d->col[0]
                     >> d->col[1]
                     >> d->col[2]
                     // and so on
                     ;
    }
 \endverbatim
 *
 * If you use the read_past_comment function, as in this example, your blocks of
 * data will automatically be delimited by two consecutive blank lines.
 *
 * Your _Data class must have a (_Data * next) field to create the linked
 * list.
 *
 * @param input
 *     Input stream.
 *
 * @param Nout
 *     On exit, the memory pointed to by Nout (if not NULL) will store the
 *     number of data elements read in.
 *
 * @returns Linked list of data on (_Data * next) field of _Data class.
 * @see readDataBlocks.
 *
 * @see Data<T,L> below for a generic class to read in data.
 */
template <class _Data>
_Data * readData(std::istream & input, int * Nout = NULL) {
    _Data * list = NULL;
    _Data * next = NULL;
    int N = 0;

    while ( (input >> next) && input.good() ) {
        next->next = list;
        list = next;
        N++;
    }

    if (Nout != NULL) *Nout = N;

    return list;
}

static bool interrupt_read = false;

/** Read in blocks of data.
 * A data block is defined by a set of data columns on each line.
 * Two blank lines separate blocks of data.
 *
 * @param input
 *     Input stream from which to read blocks of data.
 * @param retval
 *     Standard c++ vector in which to return the read-in data blocks.
 * @param start
 *     Index of the first data block to return [Defaults to 0, or the first].
 * @param interval
 *     Number of real data blocks to read in per data blocks returned
 *     [Default 1, or ``don't skip any''].
 * @param end
 *     Index of block to end on (first block is index 0).
 */
template <class _Data>
std::vector<_Data*> & readDataBlocks(std::istream & input,
                                     std::vector<_Data*> & retval,
                                     const unsigned int & start = 0,
                                     const unsigned int & interval = 1,
                                     const int & end = -1) {
    _Data * dataP = NULL;
    unsigned int index = 0;

    /* reset the read interrupt signal. */
    interrupt_read = false;

    while(!interrupt_read && (end < 0 || index <= ((unsigned long)end))) {
        /* this should read the first data block */
        dataP = readData<_Data>(input);

        if (dataP) {
            if (index >= start && (index % interval) == 0) {
                retval.push_back(dataP);
            } else {
                deleteList(dataP);
                dataP = NULL;
            }
            /* clear the input in case it was just a block that ended. */
            input.clear();
            index++;
        } else {
            /* time to quit */
            break;
        }

    }/* while */

    return retval;
}

/** Read past all consecutive comments (and white space).
 * A comment begins a line with <code>comment_char</code> and includes the
 * entire line.
 *
 * @param input
 *     Input stream.
 *
 * @param comment_char
 *     The character to use for comments [Default '#'].
 *
 * @param end_block_lines
 *     The number of blank lines between blocks [Default 2].
 */
inline bool read_past_comment(std::istream & input,
                              const char & comment_char = '#',
                              const unsigned int end_block_lines = 2) {
    char testchar = input.peek();
    unsigned int endblock = 0;
    /* read past spaces and endlines. */
    while (testchar == ' ' || (testchar == '\n' && ++endblock)) {
        (void)input.get();
        testchar = input.peek();
    }

    if (endblock>=(1+end_block_lines)) {
        input.setstate(std::ios_base::failbit);
        return false;
    }

    if (testchar == comment_char) {
        /* read past comments. */
	std::stringbuf testbuf;
	input.get(testbuf, '\n');
        return true;
    }

    return false;
}

#include "Vector.h"

template<class T, unsigned int L>
class Data : public Vector<T,L> {
  public:
    typedef Vector<T,L> super;
    inline Data() : super(), next(NULL) {}
    class Data * next;
};

template<class T, unsigned int L>
std::istream & operator>>(std::istream & input, Data<T,L> * & d) {
    while (read_past_comment(input));

    if (!input.good()) return input;

    d = new Data<T,L>;
    for (unsigned int i = 0; i < L ; i++) {
        input >> d->val[i];
    }

    return input;
}

#endif // READDATA_H
