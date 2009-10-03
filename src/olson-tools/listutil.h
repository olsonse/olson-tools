// -*- c++ -*-
// $Id: listutil.h,v 1.1 2005/01/14 14:09:31 olsonse Exp $
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

/*
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
 * Copyright 2004-2008 Spencer Olson
 */

#ifndef LISTUTIL_H
#define LISTUTIL_H


namespace olson_tools {

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

} /*namespace olson_tools*/

#endif // LISTUTIL_H
