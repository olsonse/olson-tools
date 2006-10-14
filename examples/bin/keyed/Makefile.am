# $Id: Makefile,v 1.1 2005/06/22 09:23:05 olsonse Exp $
#
# Copyright 1997-2004 Spencer Olson
#
# $Log: Makefile,v $
# Revision 1.1  2005/06/22 09:23:05  olsonse
# moved to subdirectory for 1D example.
#
# Revision 1.1  2005/04/19 17:20:42  olsonse
# Added example for using distribution inverter.
#
# Revision 1.1.1.1  2005/01/08 04:27:26  olsonse
# Initial import
#
#

include $(MAKECONF)
MAKECONF=$(top_builddir)/Makeconf

CLEANFILES      = bin.dat
bin_PROGRAMS    = testbin
testbin_SOURCES = testbin.C
testbin_LDADD   = -L$(top_builddir)/lib -lolson-tools
testbin_CPPFLAGS = -I$(top_builddir)/include
