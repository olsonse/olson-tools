// -*- c++ -*-
// $Id: io.C,v 1.1 2005/01/08 04:27:25 olsonse Exp $
/*
 * $Log: io.C,v $
 * Revision 1.1  2005/01/08 04:27:25  olsonse
 * Initial revision
 *
 * Revision 1.2  2000/06/15 19:31:00  olsons
 * Made debug_level act more like levels rather than codes.
 *
 * Revision 1.1  2000/05/06 20:37:18  olsons
 * Reorganized libfit: made it self-containing and use new definition of alleles.
 *
 * Revision 1.5  1999/09/23 21:16:55  olsons
 * Added the rc file ability to the program.
 * xuvrcfile.y is a bison (yacc) grammar.
 *
 * Revision 1.4  1999/09/09 15:53:42  olsons
 * *** empty log message ***
 *
 * Revision 1.3  1999/08/18 19:34:40  olsons
 * *** empty log message ***
 *
 * Revision 1.2  1999/07/03 05:28:01  olsons
 * Added the character-based user interface to XUVMulti.
 *
 * Revision 1.1.1.1  1999/06/04 13:47:55  olsons
 * Imported as XUVMulti into cvs
 *
 *
 */

#include "io.h"

// The following will function for now as our virtual io

_IO_istream_withassign gain;
_IO_ostream_withassign gaout;
_IO_ostream_withassign gaerr;
_IO_ostream_withassign galog;

static int init_io() {
  gain = cin;
  gaout = cout;
  gaerr = cerr;
  galog = clog;
  return 0;
}

static int init_io_var = init_io();

