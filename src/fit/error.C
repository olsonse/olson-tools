// -*- c++ -*-
// $Id: error.C,v 1.1 2005/01/08 04:27:25 olsonse Exp $
/*
 * $Log: error.C,v $
 * Revision 1.1  2005/01/08 04:27:25  olsonse
 * Initial revision
 *
 * Revision 1.5  2000/06/15 19:31:00  olsons
 * Made debug_level act more like levels rather than codes.
 *
 * Revision 1.4  2000/05/06 20:37:18  olsons
 * Reorganized libfit: made it self-containing and use new definition of alleles.
 *
 * Revision 1.3  1999/10/18 18:56:34  olsons
 * Added documentation in comments to be parsed by doc++.
 *
 * Revision 1.2  1999/09/09 15:53:21  olsons
 * added runtime debug level capability
 *
 * Revision 1.1.1.1  1999/06/04 13:47:54  olsons
 * Imported as XUVMulti into cvs
 *
 *
 */

#include "error.h"
#include "io.h"
#include "debug.h"

///
libfitError::libfitError( const LIBFIT_ERRCODE err) {
  /* I did not want to include string.h for selfish reasons. */
  for( int i = 0; i < LIBFIT_ERRCODE_LEN; i++ )
	  errcode[i] = err[i];
}

