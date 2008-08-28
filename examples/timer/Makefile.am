# $Id: Makefile,v 1.1.1.1 2005/01/08 04:27:26 olsonse Exp $
#
# Copyright 1997-2004 Spencer Olson
#
# $Log: Makefile,v $
# Revision 1.1.1.1  2005/01/08 04:27:26  olsonse
# Initial import
#
#

MAKECONF=$(top_builddir)/Makeconf
include $(MAKECONF)

CLEANFILES      = 
bin_PROGRAMS    = testTimer
LDADD           = -L$(top_builddir)/lib -lolson-tools
AM_CPPFLAGS     = -I$(top_builddir)/include
testTimer_SOURCES = testTimer.C
