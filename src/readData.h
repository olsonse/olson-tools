// -*- c++ -*-
// $Id: readData.h,v 1.1 2005/01/14 14:09:31 olsonse Exp $
/*
 * Copyright 2004 Spencer Olson
 *
 * $Log: readData.h,v $
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
 * @param ViP
 *     If you want to have a std::vector<_Data*> returned also, (a vector of
 *     pointers to all your data points, then you must pass in a pointer to 
 *     an empty, but initialized std::vector<_Data*>.
 *
 * @returns Linked list of data on (_Data * next) field of _Data class.
 * @see readDataBlocks.
 */
template <class _Data>
_Data * readData(std::istream & input, std::vector<_Data*> * ViP = NULL) {
    std::vector<_Data*> * VP = NULL;
    if (ViP) {VP = ViP;}
    else { VP = new std::vector<_Data*>; }

    std::vector<_Data*> & V = *VP;

    /* read in all of the data. */
    copy(std::istream_iterator<_Data*>(input),
         std::istream_iterator<_Data*>(),
         std::back_inserter(V));   

    _Data * last = NULL;
    for (typename std::vector<_Data*>::iterator i = V.begin(); i < V.end(); i++) {
        _Data & f = *(*i);
        f.next = last;
        last = &f;
    }

#if 0
    if (V.size() < 101) {
        cout << "And the input was:  " << endl;
        copy(V.begin(), V.end(), ostream_iterator<_Data*>(std::cout, "\n"));
    }

    if (!input.eof()) {
        std::cerr << "Error reading data point number "
        << V.size()
        << std::endl;
    }

#endif
    std::cout << "Read in " << V.size() << " data points" << std::endl;

    if (!ViP) delete VP;
    return last;
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
 */
template <class _Data>
std::vector<_Data*> & readDataBlocks(std::istream & input,
                                     std::vector<_Data*> & retval,
                                     const unsigned int & start = 0,
                                     const unsigned int & interval = 1) {
    _Data * dataP = NULL;
    unsigned int index = 0;

    /* reset the read interrupt signal. */
    interrupt_read = false;

    while(!interrupt_read) {
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
 */
inline bool read_past_comment(std::istream & input, const char & comment_char = '#') {
    char testchar = input.peek();
    unsigned int endblock = 0;
    /* read past spaces and endlines. */
    while (testchar == ' ' || (testchar == '\n' && ++endblock)) {
        (void)input.get();
        testchar = input.peek();
    }

    if (endblock>=3) {
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

#endif // READDATA_H
