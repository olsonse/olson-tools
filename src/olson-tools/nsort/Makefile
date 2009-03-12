
PROG=test/NSort

all : $(PROG)

% : %.cpp
	$(CXX) -Wall $< -o $@ \
	-I../../ \
	/usr/lib/libboost_unit_test_framework.a 
	$@

clean :
	rm -f $(PROG)
