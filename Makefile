include config.mak

DIRS = csim2 \
	example1 \
	example2

all: $(DIRS)
.PHONY: $(DIRS)

demo: csim2

release:
	$(MAKE) CONFIG=Release

$(DIRS):
	$(MAKE) -C $@

clean:
	$(foreach DIR,$(DIRS),$(MAKE) -C $(DIR) $@;)

dist-clean:
	if [ -d "$(BUILD_DIR)" ]; then \
		rm -r "$(BUILD_DIR)"; \
	fi
