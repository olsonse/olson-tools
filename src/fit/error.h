// -*- c++ -*-
// $Id: error.h,v 1.1 2005/01/08 04:27:25 olsonse Exp $
/*
 * $Log: error.h,v $
 * Revision 1.1  2005/01/08 04:27:25  olsonse
 * Initial revision
 *
 * Revision 1.4  2000/06/15 19:31:00  olsons
 * Made debug_level act more like levels rather than codes.
 *
 * Revision 1.3  2000/05/06 20:37:18  olsons
 * Reorganized libfit: made it self-containing and use new definition of alleles.
 *
 * Revision 1.2  1999/10/18 18:56:34  olsons
 * Added documentation in comments to be parsed by doc++.
 *
 * Revision 1.1.1.1  1999/06/04 13:47:54  olsons
 * Imported as XUVMulti into cvs
 *
 *
 */

#ifndef ERROR_H
#define ERROR_H

#include "errcodes.h"

/// The Exception Class
class libfitError {
public:
  /// The constructor will print an 'err' as an error message.
  libfitError( const LIBFIT_ERRCODE err );
  /// Destructor does nothing.
  ~libfitError() {}
  LIBFIT_ERRCODE errcode;
};

#endif //ERROR_H

