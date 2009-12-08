// -*- c++ -*-
// $Id$
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2008 Spencer E. Olson
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

/** \file
 * Classes and routines to help with integrating arbitrary data types with MPI.
 *
 * A few quick hacks.  
 *
 * Copyright 2008 Spencer Olson.
 *
 */

/** \example mpi/types/testtypes.cpp
 * Demonstrates using arbitrary data types in MPI.
 *
 */

#ifndef MPI_INIT_H
#define MPI_INIT_H

#include <mpi.h>
#include <vector>

namespace olson_tools {

/** Helper class to help define runtime MPI struct types.
 *
 * The vector of Blocks that have been added represent the struct that you are
 * trying to define in the MPI runtime.  If defining a non-contiguous struct
 * for MPI, you must add a spacer Block for each break in contiguous memory.
 * A spacer Block is defined by using the MPI::DATATYPE_NULL data type with
 * the space being equal to length*size.
 */
class MPIStructBuilder {
  public:

    /** The fundamental block of a structure representing a contiguous section
     * of memory.  */
    class Block {
      public:
        Block(const int & len, const size_t & sz,
              const MPI::Datatype & tp = MPI::DATATYPE_NULL) :
            length(len), size(sz), typ(tp) {}

        int total_size() const { length * size; }

        int length; /* array size.  >1 if more than one element */
        size_t size;
        MPI::Datatype typ;
    };


    /** Constructor. */
    MPIStructBuilder() : blocks(), type(MPI::DATATYPE_NULL) {}

    void addBlock(const int & len, const size_t & sz, const MPI::Datatype & tp) { 
        blocks.push_back(Block(len,sz,tp));
    }

    void addBlock(const Block & b) { 
        blocks.push_back(b);
    }

    template <class T>
    void addBlocks( const T & t, const int & len = 1) {
        addBlocks<T>(len);
    }

    template <class T>
    void addBlocks(const int & len = 1) {
        for ( int i = 0; i < len; i++) {
            blockAdder( blocks, (T*)NULL );
        }
    }

    /** Measures the total length of the collection of blocks (in bytes).
     * */
    static int total_size(std::vector<Block> & b) {
        int sz = 0;
        for (unsigned int i = 0, ii = 0; i < b.size(); i++) {
            sz += b[i].total_size();
        }

        return sz;
    }

    /** Returns the derived MPI::Datatype. */
    const MPI::Datatype & getType() const { return type; }

    /** Creates the MPI::Datatype according to the representation in blocks.
     * */
    const MPI::Datatype & Create_struct() {
        int nblocks = blocks.size();
        int *blocklengths = new int[nblocks];
        MPI::Aint *displacements = new MPI::Aint[nblocks];
        MPI::Datatype *typs = new MPI::Datatype[nblocks];

        int disp = 0;
        for (unsigned int i = 0, ii = 0; i < blocks.size(); i++) {
            Block & b = blocks[i];

            if (b.typ == MPI::DATATYPE_NULL) {
                nblocks--;
            } else {
                blocklengths[ii] = b.length;
                typs[ii] = b.typ;
                displacements[ii] = disp;
                ii++;
            }

            disp += b.length*b.size;
        }

        type = MPI::Datatype::Create_struct(nblocks, blocklengths, displacements, typs);

        delete[] blocklengths;
        delete[] displacements;
        delete[] typs;

        return type;
    }

  private:
    std::vector<Block> blocks;
    MPI::Datatype type;

};


/** Simple class to help initialize things for MPI.  
 *
 * The intent of this class is to provide a cache of functions that must be
 * called after MPI::Init and before MPI::Finalize.  To make things easy for
 * users, functions which define library data types and MPI::Op functions can
 * be cached during initialization of the static storage class.  
 *
 * @see GenericBin for an example.
 */
class MPIInit {
  public:
    typedef void (* MPI_INIT_FUNC)();
  private:
    typedef std::vector<MPI_INIT_FUNC> fvector;
    static fvector init_functions;
    static fvector finish_functions;
    static bool initialized;

    static void add(fvector &v, MPI_INIT_FUNC f) {
        for (unsigned int i = 0; i < v.size(); i++) {
            if (v[i] == f) {
                /* found so don't continue. */
                return;
            }
        }/* for */

        v.push_back(f);
    }

  public:

    /** Add initialization function to be run after MPI::Init.  It is the job
     * of the user to call MPIInit::Init after MPI::Init.
     */
    static void add_init(MPI_INIT_FUNC f) { add(init_functions,f); }

    /** Add finalization function to be run before MPI::Finalize.  It is the job
     * of the user to call MPIInit::Finish before MPI::Finalize.
     */
    static void add_finish(MPI_INIT_FUNC f) { add(finish_functions,f); }

    /** Calls all cached init functions. */
    static void Init() {
        if (initialized) {
            std::cerr << "MPIInit::Init() already called." << std::endl;
            return;
        }

        for (unsigned int i = 0; i < init_functions.size(); i++) {
            MPI_INIT_FUNC f = init_functions[i];
            f();
        }

        initialized = true;
    }

    /** Calls all cached finish functions. */
    static void Finish() {
        if (!initialized) {
            std::cerr << "MPIInit::Init() not previously called." << std::endl;
            return;
        }

        for (unsigned int i = 0; i < init_functions.size(); i++) {
            MPI_INIT_FUNC f = finish_functions[i];
            f();
        }

        initialized = false;
    }
};

bool MPIInit::initialized = false;
MPIInit::fvector MPIInit::init_functions = MPIInit::fvector();
MPIInit::fvector MPIInit::finish_functions = MPIInit::fvector();

}; /* namespace olson_tools */

#endif // MPI_INIT_H

