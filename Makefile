CFLAGS=-Wall -Wextra -Wpedantic -Iblock -DFLOAT_TYPE=double

.PHONY: all clean test

all: test example_mass_spring_damper

test_integrator: test/test_integrator.c block/integrator.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_firstOrderLag: test/test_firstOrderLag.c block/firstOrderLag.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

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

example_mass_spring_damper: example/mass_spring_damper.c block/solver.o example/ascii_plot.o
	cc $(CFLAGS) -Iexample -o $@ $^ $(LDFLAGS) -lm

example_block_system: example/block_system.c block/solver.o example/ascii_plot.o block/blockSystem.o block/firstOrderLag.o block/secondOrderSystem.o
	cc $(CFLAGS) -Iexample -o $@ $^ $(LDFLAGS) -lm

EXAMPLE=example_mass_spring_damper example_block_system

clean:
	rm -rf */*.o */*~ $(TEST) $(EXAMPLE)
