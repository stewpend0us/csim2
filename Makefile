CFLAGS=-O2 -Wall -Wextra -Wpedantic -Iblock -DFLOAT_TYPE=double

.PHONY: all clean test

all: test

test_integrator: test/test_integrator.c block/integrator.c
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_firstOrderLag: test/test_firstOrderLag.c block/firstOrderLag.c
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

TEST=test_integrator test_firstOrderLag
test: $(TEST)
	for x in $^; do ./$$x; done

clean:
	rm -rf *.o *~ block/*.o block/*~ test/*.o test/*~ example/*.o example/*~ $(TEST)
