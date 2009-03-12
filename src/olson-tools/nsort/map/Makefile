
PROG=test/_nD test/remap

all : $(PROG)

% : %.cpp
	$(CXX) -Wall $< -o $@ \
	-I../../../ \
	/usr/lib/libboost_unit_test_framework.a 
	$@

clean :
	rm -f $(PROG)
