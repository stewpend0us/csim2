TARGET := $(BUILD_DIR)/$(LIB_PREFIX)$(TARGET_NAME)$(STATIC_LIB_EXT)
all: $(TARGET)

include $(ROOT_DIR)/common_header.mak

CFLAGS += -fPIC
CXXFLAGS += -fPIC

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(AR) qs $@ $^

include $(ROOT_DIR)/common_footer.mak