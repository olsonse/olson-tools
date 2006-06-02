# $Id: Makefile,v 1.1.1.1 2005/01/08 04:27:26 olsonse Exp $
#
# Copyright 1997-2004 Spencer Olson
#
# $Log: Makefile,v $
# Revision 1.1.1.1  2005/01/08 04:27:26  olsonse
# Initial import
#
#

include ../../../Makeconf

#OBJ=createfieldfile.o  testfield.o

all: createfieldfile  testfield

PROGNAME=createfieldfile  testfield

LIBS:=-lolson-tools$(EXT) $(LIBS)
LDFLAGS=-L$(MAINDIR)/lib
INCLUDES=-I$(MAINDIR)/include

$(PROGNAME): $(OBJ)
	make $@.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $@.o -o $@$(EXT) $(OBJ) $(LIBS)

clean:
	rm -f *.o *$(EXT)
	rm -f error.dat field.dat
