ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = olson-tools.pc

makeconfdir = @MAKECONF_PATH@
makeconf_DATA = Makeconf

SUBDIRS = src

DIST_SUBDIRS = src examples

tests :
	cd examples; make




include doxygen.am

MOSTLYCLEANFILES = $(DX_CLEANFILES)

if DX_COND_doc
all-local: doc doxygen-doc
doc :
	mkdir -p doc
APIDOCS = doc/api
endif

EXTRA_DIST = $(DX_CONFIG) $(APIDOCS) licenses

