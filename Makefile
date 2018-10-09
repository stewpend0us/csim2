CFLAGS=-O2 -Wall -Wextra -Wpedantic -Iblock -Isolver -DFLOAT_TYPE=double

BLOCK_C=$(wildcard block/*.c)
SOLVER_C=$(wildcard solver/*.c)
EXAMPLE_C=$(wildcard example/*.c)

EXAMPLE=$(patsubst %.c, %, $(EXAMPLE_C))

.PHONY: all clean test example

all: test

test: test_integrator
test_integrator: test/test_integrator.c block/integrator.c
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

example: $(EXAMPLE)
	for x in $^; do ./$$x; done

$(EXAMPLE): CFLAGS+=-Iexample
$(EXAMPLE): LDFLAGS+=-lm
$(EXAMPLE): %: %.c $(BLOCK_C) $(SOLVER_C)
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf *.o *~ test_integrator $(EXAMPLE)
