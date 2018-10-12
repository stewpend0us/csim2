CFLAGS=-O2 -Wall -Wextra -Wpedantic -Iblock -I. -DFLOAT_TYPE=double

.PHONY: all clean test

all: test

test_integrator: test/test_integrator.c block/integrator.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_firstOrderLag: test/test_firstOrderLag.c block/firstOrderLag.c
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_euler: test/test_euler.c solver.o block/integrator.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_rk4: test/test_rk4.c solver.o block/integrator.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

TEST=test_integrator test_firstOrderLag test_euler
test: $(TEST)
	for x in $^; do ./$$x; done

clean:
	rm -rf *.o *~ block/*.o block/*~ test/*.o test/*~ example/*.o example/*~ $(TEST)
