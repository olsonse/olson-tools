# $Id: Makefile,v 1.1 2005/01/08 04:27:26 olsonse Exp $
#
# Copyright 1997-2004 Spencer Olson
#
# $Log: Makefile,v $
# Revision 1.1  2005/01/08 04:27:26  olsonse
# Initial revision
#
#

include ../../../Makeconf

OBJ=testfast_pow.o stdpow.o

PROGNAME=testfast_pow$(EXT)

LIBS:=-lolson-tools$(EXT) $(LIBS)
LDFLAGS=-L$(MAINDIR)/lib
INCLUDES=-I$(MAINDIR)/include

${PROGNAME}: ${OBJ}
	$(F77) ${CFLAGS} ${LDFLAGS} $(INCLUDES) -o $(PROGNAME) ${OBJ} ${LIBS} -lc

clean:
	rm -f ${OBJ} ${PROGNAME}
