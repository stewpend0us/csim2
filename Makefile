CFLAGS=-g -O2 -Wall -Wextra -Isrc -DNDEBUG $(OPTFLAGS)
LDFLAGS=$(OTPLIBS)
PREFIX?=/usr/local

SRC_C=$(wildcard src/**/*.c src/*.c)
SRC_O=$(patsubst %.c,%.o,$(SRC_C))

TEST_C=$(wildcard tests/**/*_test.c tests/*_test.c)
TEST=$(patsubst %.c,%,$(TEST_C))

EXAMPLE_C=$(wildcard examples/**/*_example.c examples/*_example.c)
EXAMPLE=$(patsubst %.c,%,$(EXAMPLE_C))

TARGET=build/libcsim2.a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

# The Target Build
all: $(TARGET) $(SO_TARGET) tests examples

dev: CFLAGS=-g -Wall -Wextra -Isrc $(OPTFLAGS)
dev: all

$(TEST): LDFLAGS += -lm
$(TEST): $(TARGET) $(SO_TARGET)
	$(CC) $(CFLAGS) -o $@ $@.c $(SRC_O) $(LDFLAGS)

$(EXAMPLE): LDFLAGS += -lm
$(EXAMPLE): $(TARGET) $(SO_TARGET)
	$(CC) $(CFLAGS) -o $@ $@.c $(SRC_O) $(LDFLAGS)

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(SRC_O)
	ar rcs $@ $(SRC_O)
	ranlib $@

$(SO_TARGET): $(TARGET) $(SRC_O)
	$(CC) -shared -o $@ $(SRC_O)

build:
	@mkdir -p build
	@mkdir -p bin

# The Unit Tests
.PHONY: tests
tests: CFLAGS += -Isrc/blocks -Isrc/solvers $(TARGET)
tests: $(TEST)
	sh ./tests/runtests.sh

.PHONY: examples
examples: CFLAGS += -Isrc/blocks -Isrc/solvers $(TARGET)
examples: $(EXAMPLE)

# The Cleaner
clean:
	rm -rf build $(SRC_O) $(TEST) $(EXAMPLE)
	rm -f tests/test.log
	find . -name "*.gc" -exec rm {} \;

# The Install
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions.
	@egrep $(BADFUNCS) $(SRC_C) || true
