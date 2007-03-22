# $Id: Makefile,v 1.1 2005/04/19 17:21:02 olsonse Exp $
#
# Copyright 1997-2004 Spencer Olson
#
# $Log: Makefile,v $
# Revision 1.1  2005/04/19 17:21:02  olsonse
# Added example for trapfe library to trap floating point exceptions.
#
# Revision 1.1.1.1  2005/01/08 04:27:26  olsonse
# Initial import
#
#

MAKECONF=$(top_builddir)/Makeconf
include $(MAKECONF)

DUMMY:=$(shell printf "OPTIM_CXX_COPY:=\$$(OPTIM_CXX)\nOPTIM_CXX:= \n" '$$' > Makeconf.local; echo Makeconf.local)
include $(DUMMY)

OPTIM_CXX_COPY:=$(OPTIM_CXX)
OPTIM_CXX:=

bin_PROGRAMS    = testtrapfe_opt testtrapfe_debug
LDADD           = -L$(top_builddir)/lib -ltrapfe
testtrapfe_opt_SOURCES = testtrapfe.C
testtrapfe_opt_CXXFLAGS = $(OPTIM_CXX_COPY)
testtrapfe_debug_SOURCES = testtrapfe.C
testtrapfe_debug_CXXFLAGS = -g
