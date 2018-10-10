CFLAGS=-O2 -Wall -Wextra -Wpedantic -Iblock -DFLOAT_TYPE=double

BLOCK_C=$(wildcard block/*.c)
SOLVER_C=$(wildcard solver/*.c)
EXAMPLE_C=$(wildcard example/*.c)

EXAMPLE=$(patsubst %.c, %, $(EXAMPLE_C))

.PHONY: all clean test example

all: test

BLOCKS=integrator firstOrderLag
$(BLOCKS): %: test/test_%.c block/%.c
	cc $(CFLAGS) -o test_$@ $^ $(LDFLAGS)

test: $(BLOCKS)
	for x in $^; do ./test_$$x; done

example: $(EXAMPLE)
	for x in $^; do ./$$x; done

$(EXAMPLE): CFLAGS+=-Iexample
$(EXAMPLE): LDFLAGS+=-lm
$(EXAMPLE): %: %.c $(BLOCK_C) $(SOLVER_C)
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf *.o *~ $(patsubst %, test_%, $(BLOCKS)) $(EXAMPLE)
