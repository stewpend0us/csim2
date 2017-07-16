TARGET := $(BUILD_DIR)/$(LIB_PREFIX)$(TARGET_NAME)$(SHARED_LIB_EXT)
all: $(TARGET)

include $(ROOT_DIR)/common_header.mak

CFLAGS += -fPIC
CXXFLAGS += -fPIC
LDFLAGS += -shared

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

include $(ROOT_DIR)/common_footer.mak