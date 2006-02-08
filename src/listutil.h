// -*- c++ -*-
// $Id: listutil.h,v 1.1 2005/01/14 14:09:31 olsonse Exp $
/*
 * Copyright 2004 Spencer Olson
 *
 * $Log: listutil.h,v $
 * Revision 1.1  2005/01/14 14:09:31  olsonse
 * Fixed documentation on memory.h, msh.h, options.h.
 * Moved new files Distribution.[hC] listutil.h readData.h from dsmc code.
 *
 * Revision 1.1.1.1  2005/01/08 04:57:25  olsonse
 * Initial CVS import (finally).
 *
 */


/* \file
 * Simple utilitiy functions for use with linked lists.
 *
 * Copyright 2004 Spencer Olson
 */

#ifndef LISTUTIL_H
#define LISTUTIL_H

/** A c++ routine for deleting a linked list of particles.
 * @returns
 *     The number of items deleted.
 */
template <class T>
inline int deleteList(T * list) {
    T * next = NULL;
    int n = 0;

    while (list) {
        n++;
        next = list->next;
        delete list;
        list = next;
    }

    return n;
}

#endif // LISTUTIL_H
