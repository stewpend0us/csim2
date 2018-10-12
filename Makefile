CFLAGS=-Wall -Wextra -Wpedantic -Iblock -I. -DFLOAT_TYPE=double

.PHONY: all clean test profile

all: test

test_integrator: test/test_integrator.c block/integrator.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_firstOrderLag: test/test_firstOrderLag.c block/firstOrderLag.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_euler: test/test_euler.c solver.o block/integrator.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_rk4: test/test_rk4.c solver.o block/integrator.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

TEST=test_integrator test_firstOrderLag test_euler
test: CFLAGS+=-O3
test: $(TEST)
	for x in $^; do ./$$x; done

profile_euler: example/profile_euler.c solver.o block/firstOrderLag.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

profile: CFLAGS+=-pg -O3
profile: clean profile_euler
	./profile_euler
	gprof profile_euler -b

clean:
	rm -rf gmon.* *.bin *.o *~ block/*.o block/*~ test/*.o test/*~ example/*.o example/*~ $(TEST) profile_euler
