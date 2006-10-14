# Copyright 2004 Spencer Olson

include Makeconf

all:
	# Type 'make tools' or 'make tests' or 'make doc'

tools:
	cd src; make;
	rm -rf include;
	make libs;
	make include;

libs:
	mkdir -p lib;
	rm -f lib/libolson-tools$(EXT).a;
	install -m "ugo=r" src/libolson-tools$(EXT).a lib/;
	install -m "ugo=r" src/libtrapfe$(EXT).a lib/;
	chmod -R "ugo=r" lib/libolson-tools$(EXT).a lib/libtrapfe$(EXT).a;
	#install -m "u=rx,go=rx" src/libolson-tools*.so lib/;

doc:
	cd src; doxygen tools.doxy

tests:
	cd examples; make;

include:
	mkdir -p include/olson-tools;
	install -m "ugo=r" `find src/ -name "*.h"` include/olson-tools/;
	chmod -R "ugo=r" `find include/olson-tools -name "*.h"`;

clean:
	cd src; make clean;
	cd examples; make clean;
	rm -rf lib;
	rm -rf include;
