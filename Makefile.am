ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = olson-tools.pc

makeconfdir = @MAKECONF_PATH@
makeconf_DATA = Makeconf

SUBDIRS = src

tests :
	cd examples; make

doc :
	cd src; doxygen tools.doxy
