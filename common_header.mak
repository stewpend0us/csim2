C_SRCS = $(wildcard *.c)
CXX_SRCS = $(wildcard *.cpp)
OBJ_DIR := $(BUILD_DIR)/$(TARGET_NAME)
OBJS = $(addprefix $(OBJ_DIR)/,$(C_SRCS:.c=.o) $(CXX_SRCS:.cpp=.o))
DEPENDS = $(OBJS:.o=.d)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<