TARGET := $(BUILD_DIR)/$(TARGET_NAME)$(EXE_EXT)
all: $(TARGET)

include $(ROOT_DIR)/common_header.mak

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

include $(ROOT_DIR)/common_footer.mak