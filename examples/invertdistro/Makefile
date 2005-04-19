# $Id: Makefile,v 1.1 2005/04/19 17:20:42 olsonse Exp $
#
# Copyright 1997-2004 Spencer Olson
#
# $Log: Makefile,v $
# Revision 1.1  2005/04/19 17:20:42  olsonse
# Added example for using distribution inverter.
#
# Revision 1.1.1.1  2005/01/08 04:27:26  olsonse
# Initial import
#
#

include ../../Makeconf

OBJ=testdist.o

PROGNAME=testdist$(EXT)

LIBS:=-lolson-tools$(EXT) $(LIBS)
LDFLAGS=-L$(MAINDIR)/lib
INCLUDES=-I$(MAINDIR)/include

${PROGNAME}: ${OBJ}
	$(CXX) ${CFLAGS} ${LDFLAGS} $(INCLUDES) -o $(PROGNAME) ${OBJ} ${LIBS}

clean:
	rm -f ${OBJ} ${PROGNAME}
