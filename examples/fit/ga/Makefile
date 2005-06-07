# $Id: Makefile,v 1.1 2005/06/07 20:38:37 olsonse Exp $
#
# Copyright 1997-2004 Spencer Olson
#
# $Log: Makefile,v $
# Revision 1.1  2005/06/07 20:38:37  olsonse
# Added genetic algorithm test/example code.
#
# Revision 1.1.1.1  2005/01/08 04:27:26  olsonse
# Initial import
#
#

include ../../../Makeconf

OBJ=testGA.o

PROGNAME=testGA$(EXT)

LIBS:=-lolson-tools$(EXT) $(LIBS)
LDFLAGS=-L$(MAINDIR)/lib
INCLUDES=-I$(MAINDIR)/include

${PROGNAME}: ${OBJ}
	$(CXX) ${CFLAGS} ${LDFLAGS} $(INCLUDES) -o $(PROGNAME) ${OBJ} ${LIBS}

clean:
	rm -f ${OBJ} ${PROGNAME}
