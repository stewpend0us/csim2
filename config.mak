ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
TARGET_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

CONFIG ?= Debug

ifeq ($(patsubst /%,/,$(O)),/)
 BUILD_DIR := $(abspath $(O)/$(CONFIG))
else
 ifeq ($(O),)
  BUILD_DIR := $(abspath $(ROOT_DIR)/$(CONFIG))
 else
  BUILD_DIR := $(abspath $(ROOT_DIR)/$(O)/$(CONFIG))
 endif
endif

ifeq ($(CONFIG),Debug)
	CFLAGS += -g
	CXXFLAGS += -g
else
	CFLAGS += -O3
	CXXFLAGS += -O3
endif

CFLAGS += -Wall -MMD
CXXFLAGS += -Wall -MMD

ifeq ($(OS),)
	OS := $(shell uname -s)
endif

ifeq ($(OS),Windows_NT)
	LIB_PREFIX =
	STATIC_LIB_EXT = .lib
	SHARED_LIB_EXT = .dll
	EXE_EXT = .exe
else
	LIB_PREFIX = lib
	STATIC_LIB_EXT = .a
	SHARED_LIB_EXT = .so
	EXE_EXT =
endif