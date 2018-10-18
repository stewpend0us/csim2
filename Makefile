CFLAGS=-Wall -Wextra -Wpedantic -Iblock -DFLOAT_TYPE=double

.PHONY: all clean test profile status

all: test

test_integrator: test/test_integrator.c block/integrator.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_firstOrderLag: test/test_firstOrderLag.c block/firstOrderLag.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

CFLAGS+=-Wno-missing-field-initializers
test_blockSystem: test/test_blockSystem.c block/blockSystem.o block/integrator.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_euler: test/test_euler.c block/solver.o block/integrator.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_rk4: test/test_rk4.c block/solver.o block/integrator.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

TEST=test_integrator test_firstOrderLag test_blockSystem test_euler test_rk4
test: CFLAGS+=-O3
test: $(TEST)
	for x in $^; do echo "./$$x"; ./$$x; done

profile_euler: example/profile_euler.c block/solver.o block/firstOrderLag.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

profile_euler_inline: example/profile_euler_inline.c block/firstOrderLag.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

profile_rk4: example/profile_rk4.c block/solver.o block/firstOrderLag.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

PROFILE=profile_euler profile_euler_inline profile_rk4
profile: CFLAGS+=-pg -O3
profile: clean $(PROFILE)
	./profile_euler
	gprof profile_euler -b
	./profile_rk4
	gprof profile_rk4 -b

status: clean
	git status
clean:
	rm -rf *.o *~ block/*.o block/*~ test/*.o test/*~ example/*.o example/*~ $(TEST) $(PROFILE) gmon.* *.profile
