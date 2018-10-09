CFLAGS=-O2 -Wall -Wextra -Iblock -Isolver

BLOCK_C=$(wildcard block/*.c)
SOLVER_C=$(wildcard solver/*.c)
TEST_C=$(wildcard test/*.c)
EXAMPLE_C=$(wildcard example/*.c)

TEST=$(patsubst %.c, %, $(TEST_C))
EXAMPLE=$(patsubst %.c, %, $(EXAMPLE_C))

.PHONY: all clean test example

all: test

test: $(TEST)
	for x in $^; do ./$$x; done

$(TEST): CFLAGS+=-Itest
$(TEST): LDFLAGS+=-lm
$(TEST): %: %.c $(BLOCK_C) $(SOLVER_C)
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

example: $(EXAMPLE)
	for x in $^; do ./$$x; done

$(EXAMPLE): CFLAGS+=-Iexample
$(EXAMPLE): LDFLAGS+=-lm
$(EXAMPLE): %: %.c $(BLOCK_C) $(SOLVER_C)
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf *.o *~ $(TEST) $(EXAMPLE)
