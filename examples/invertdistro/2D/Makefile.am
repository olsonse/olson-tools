# $Id: Makefile,v 1.1 2005/06/22 09:23:27 olsonse Exp $
#
# Copyright 1997-2004 Spencer Olson
#
# $Log: Makefile,v $
# Revision 1.1  2005/06/22 09:23:27  olsonse
# Added 2D distribution inversion example.
#
# Revision 1.1  2005/04/19 17:20:42  olsonse
# Added example for using distribution inverter.
#
# Revision 1.1.1.1  2005/01/08 04:27:26  olsonse
# Initial import
#
#

MAKECONF=$(top_builddir)/Makeconf
include $(MAKECONF)

CLEANFILES      = disttest.dat disttest150uK.dat
bin_PROGRAMS    = testdist
LDADD           = -L$(top_builddir)/lib -lolson-tools
AM_CPPFLAGS     = -I$(top_builddir)/include
testdist_SOURCES = testdist.C
