CFLAGS=-O2 -Wall -Wextra -Iblock -Isolver

BLOCK_C=$(wildcard block/*.c)
SOLVER_C=$(wildcard solver/*.c)
TEST_C=$(wildcard test/*.c)
TEST=$(patsubst %.c, %, $(TEST_C))

.PHONY: all clean test

all: test

test: $(TEST)
	for x in $(TEST); do ./$(x); done

$(TEST): CFLAGS+=-Itest
$(TEST): LDFLAGS+=-lm
$(TEST): %: %.c $(BLOCK_C) $(SOLVER_C)
	cc $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf *.o *~ $(TEST)
