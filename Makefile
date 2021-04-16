MAKEFLAGS+=-j4
CFLAGS=-Wall -Wextra -Wpedantic -Isrc -DFLOAT_TYPE=double
TEST=$(patsubst %.c, %, $(wildcard test/*.c))
EXAMPLE=$(patsubst %.c, %, $(wildcard example/*.c))

.PHONY: all clean example test

all: example test

src/%.o: src/%.c src/%.h src/block.h
	cc $(CFLAGS) -c -o $@ $< $(LDFLAGS)

lib/%.o: lib/%.c lib/%.h
	cc $(CFLAGS) -c -o $@ $< $(LDFLAGS)

test/integrator: test/integrator.c src/block_basic.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test/first_order_lag: test/first_order_lag.c src/block_basic.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test/block_system: test/block_system.c src/block_basic.o src/block_system.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test/euler: test/euler.c src/block_basic.o src/block_solver.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

test/rk4: test/rk4.c src/block_basic.o src/block_solver.o
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

example/mass_spring_damper: example/mass_spring_damper.c src/block_solver.o lib/ascii_plot.o
	cc $(CFLAGS) -Ilib -o $@ $^ $(LDFLAGS) -lm

example/block_system: example/block_system.c src/block_solver.o src/block_system.o src/block_basic.o lib/ascii_plot.o
	cc $(CFLAGS) -Ilib -o $@ $^ $(LDFLAGS) -lm

test: CFLAGS+=-O3
test: $(TEST)
	for x in $^; do ./$$x; done

example: $(EXAMPLE)

clean:
	rm -rf */*.o */*~ $(TEST) $(EXAMPLE)
